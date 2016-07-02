#include <stdio.h>
#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#ifdef __APPLE__
#  define GLFW_INCLUDE_GLCOREARB
#endif
#include <SDL2/SDL.h>
#include <time.h>
#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"
#include "demo.h"
#include "perf.h"
#include "render.h"
#include "prelude.h"
#include "ide.h"
#include "node.h"

static int sdl_init( SDL_Window **w );

void errorcb(int error, const char* desc)
{
  printf("GLFW error %d: %s\n", error, desc);
}


void
ide_initializate(struct vpl_ide *ide, int width, int height) {
  int i = 0;
  int j = 0;
  int pins_used = 0;

  static const int pin_size = 15;
  static struct vpl_node nodes[10];
  static struct vpl_pin pins[1024];
  static struct vpl_pin *pin;


  vpl_ide_init(ide);

  for (i = 0; i < ARRAY_SIZE(pins); ++i) {
    vpl_pin_init(&pins[i]);
  }

  for (i = 0; i < ARRAY_SIZE(nodes); ++i) {
    struct vpl_node *node = &nodes[i];
    pin = &pins[pins_used];

    vpl_node_init(node);

    node->x = rand() % width;
    node->y = rand() % height;
    node->w = 200;
    node->h = 150;

    node->border_color.r = (rand() % 255) / 255.f;
    node->border_color.g = (rand() % 255) / 255.f;
    node->border_color.b = (rand() % 255) / 255.f;

    for (j = 0; j < 2; j++) {
      pin[j].size = pin_size;
      pin[j].border_color.r = 1;
      pin[j].border_color.g = 1;
      pin[j].border_color.b = 0;
      pin[j].parent = node;
    }

    node->left_pins = pin;
    pins_used += 2;
    node->left_pin_count = 2;

    pin = &pins[pins_used];

    for (j = 0; j < 3; j++) {
      pin[j].size = pin_size;
      pin[j].border_color.r = 1;
      pin[j].border_color.g = 0;
      pin[j].border_color.b = 0;
      pin[j].parent = node;
    }

    node->right_pins = pin;
    pins_used += 3;
    node->right_pin_count = 3;
  }

  ide->nodes = nodes;
  ide->num_nodes = ARRAY_SIZE(nodes);
}


int main()
{
  SDL_Window* window = NULL;

  if (sdl_init(&window) != 0) {
    printf( "Couldn't init SDL: %s\n", SDL_GetError() );
    return 1;
  }

  DemoData data;
  NVGcontext* vg = NULL;
  struct vpl_ide ide;
  GPUtimer gpuTimer;
  PerfGraph fps, cpuGraph, gpuGraph;
  u32 prevt = 0, cpuTime = 0;
  int quit = 0;
  static const int width = 1024;
  static const int height = 720;
  srand(time(NULL));

  ide_initializate(&ide, width, height);

  initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
  initGraph(&cpuGraph, GRAPH_RENDER_MS, "CPU Time");
  initGraph(&gpuGraph, GRAPH_RENDER_MS, "GPU Time");

#ifdef NANOVG_GLEW
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) {
    printf("Could not init glew.\n");
    return -1;
  }
  // GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll
  // consume it here.
  glGetError();
#endif

#ifdef DEMO_MSAA
  vg = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
#else
  vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
  if (vg == NULL) {
    printf("Could not init nanovg.\n");
    return -1;
  }

  ide.nvg = vg;

  if (loadDemoData(vg, &data) == -1)
    return -1;

  initGPUTimer(&gpuTimer);

  prevt = SDL_GetTicks();

  while (quit == 0)
  {
    double mx, my;
    u32 t, dt;
    int m1;
    int winWidth, winHeight;
    /* int fbWidth, fbHeight; */
    /* float pxRatio; */
    float gpuTimes[3];
    int i, n;

    t = SDL_GetTicks();
    dt = t - prevt;
    prevt = t;

    SDL_Event e;
    while ( SDL_PollEvent( &e ) )
    {
      switch( e.type )
        {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          break;
        case SDL_MOUSEBUTTONDOWN:
          m1 = 1;
          break;
        case SDL_MOUSEBUTTONUP:
          m1 = 0;
          break;
        case SDL_MOUSEMOTION:
          mx = e.motion.x;
          my = e.motion.y;
          break;
        case SDL_MOUSEWHEEL:
          break;
        case SDL_QUIT:
          quit = 1;
          break;
        }
    }

    startGPUTimer(&gpuTimer);

    SDL_GetWindowSize(window, &winWidth, &winHeight);
    /* glfwGetFramebufferSize(window, &fbWidth, &fbHeight); */
    /* pxRatio = (float)fbWidth / (float)winWidth; */
    // Calculate pixel ration for hi-dpi devices.

    // Update and render
    /* glViewport(0, 0, fbWidth, fbHeight); */
    glViewport(0, 0, winWidth, winHeight);
    glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    // TODO: retina
    nvgBeginFrame(vg, winWidth, winHeight, 1.0 /* pxRatio */);

    vpl_ide_interact(&ide, m1, mx, my);
    vpl_ide_draw(&ide, mx, my);

    renderGraph(vg, 5,5, &fps);
    renderGraph(vg, 5+200+5,5, &cpuGraph);
    if (gpuTimer.supported)
      renderGraph(vg, 5+200+5+200+5,5, &gpuGraph);

    nvgEndFrame(vg);

    // Measure the CPU time taken excluding swap buffers (as the swap may wait
    // for GPU)
    cpuTime = SDL_GetTicks() - t;

    updateGraph(&fps, dt);
    updateGraph(&cpuGraph, cpuTime);

    // We may get multiple results.
    n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
    for (i = 0; i < n; i++)
      updateGraph(&gpuGraph, gpuTimes[i]);

    SDL_GL_SwapWindow( window );
  }

  vpl_ide_destroy(&ide);
  freeDemoData(vg, &data);

  nvgDeleteGL3(vg);

  printf("Average Frame Time: %.2f ms\n", getGraphAverage(&fps) * 1000.0f);
  printf("          CPU Time: %.2f ms\n", getGraphAverage(&cpuGraph) * 1000.0f);
  printf("          GPU Time: %.2f ms\n", getGraphAverage(&gpuGraph) * 1000.0f);

  SDL_Quit();
  return 0;
}

int sdl_init( SDL_Window **w )
{
  if ( SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS ) == 0 )
  {

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,
                         SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

    *w = SDL_CreateWindow( "VPL", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  1024, 768, SDL_WINDOW_OPENGL );
    if ( *w )
    {
      SDL_GL_CreateContext( *w );

      // Activate glew
      glewExperimental = GL_TRUE;
      GLenum err = glewInit();
      if ( err == GLEW_OK )
      {
        glEnable(GL_STENCIL_TEST);

        return 0;
      }
      else
      {
        // Send GLEW error as an SDL Error
        // SDL's error should be clear, since any previous error would stop further initialization
        SDL_SetError( "GLEW Reported an Error: %s", glewGetErrorString(err) );
      }
    }
  }
  if ( *w ) SDL_DestroyWindow( *w );
  *w = NULL;
  return 1;
}

int loadDemoData(NVGcontext* vg, DemoData* data)
{
	int i;

	if (vg == NULL)
		return -1;

	for (i = 0; i < 12; i++) {
		char file[128];
		snprintf(file, 128, "../example/images/image%d.jpg", i+1);
		data->images[i] = nvgCreateImage(vg, file, 0);
		if (data->images[i] == 0) {
			printf("Could not load %s.\n", file);
			return -1;
		}
	}

	data->fontIcons = nvgCreateFont(vg, "icons", "../example/entypo.ttf");
	if (data->fontIcons == -1) {
		printf("Could not add font icons.\n");
		return -1;
	}
	data->fontNormal = nvgCreateFont(vg, "sans", "../example/Roboto-Regular.ttf");
	if (data->fontNormal == -1) {
		printf("Could not add font italic.\n");
		return -1;
	}
	data->fontBold = nvgCreateFont(vg, "sans-bold", "../example/Roboto-Bold.ttf");
	if (data->fontBold == -1) {
		printf("Could not add font bold.\n");
		return -1;
	}

	return 0;
}

void freeDemoData(NVGcontext* vg, DemoData* data)
{
	int i;

	if (vg == NULL)
		return;

	for (i = 0; i < 12; i++)
		nvgDeleteImage(vg, data->images[i]);
}
