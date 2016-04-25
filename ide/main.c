#include <stdio.h>
#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#ifdef __APPLE__
#  define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
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


void errorcb(int error, const char* desc)
{
  printf("GLFW error %d: %s\n", error, desc);
}


int main()
{
  GLFWwindow* window;
  DemoData data;
  NVGcontext* vg = NULL;
  struct vpl_ide ide;
  GPUtimer gpuTimer;
  PerfGraph fps, cpuGraph, gpuGraph;
  double prevt = 0, cpuTime = 0;
  static const int width = 1024;
  static const int height = 720;
  srand(time(NULL));

  vpl_ide_init(&ide);

  struct vpl_node nodes[10];
  struct vpl_pin pin;
  vpl_pin_init(&pin);
  for (int i = 0; i < ARRAY_SIZE(nodes); ++i) {
    struct vpl_node *node = &nodes[i];
    vpl_node_init(node);
    node->x = rand() % width;
    node->y = rand() % height;
    node->w = 200;
    node->h = 150;
    pin.border_color.r = 1;
    pin.border_color.g = 1;
    pin.border_color.b = 0;

    node->border_color.r = 1;
    node->border_color.g = 105.f / 255.f;
    node->border_color.b = 0;
    struct vpl_pin left_pins[] = {pin, pin};
    node->left_pins = left_pins;
    node->left_pin_count = ARRAY_SIZE(left_pins);
    pin.border_color.r = 1;
    pin.border_color.g = 0;
    pin.border_color.b = 0;
    struct vpl_pin right_pins[] = {pin, pin, pin};
    node->right_pins = right_pins;
    node->right_pin_count = ARRAY_SIZE(right_pins);
  }

  ide.nodes = nodes;
  ide.num_nodes = ARRAY_SIZE(nodes);

  if (!glfwInit()) {
    printf("Failed to init GLFW.");
    return -1;
  }

  initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
  initGraph(&cpuGraph, GRAPH_RENDER_MS, "CPU Time");
  initGraph(&gpuGraph, GRAPH_RENDER_MS, "GPU Time");

  glfwSetErrorCallback(errorcb);
#ifndef _WIN32 // don't require this on win32, and works with more cards
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

#ifdef DEMO_MSAA
  glfwWindowHint(GLFW_SAMPLES, 4);
#endif
  window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
//  window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* glfwSetKeyCallback(window, key); */

  glfwMakeContextCurrent(window);
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

  glfwSwapInterval(0);

  initGPUTimer(&gpuTimer);

  glfwSetTime(0);
  prevt = glfwGetTime();

  while (!glfwWindowShouldClose(window))
  {
    double mx, my, t, dt;
    int m1;
    int winWidth, winHeight;
    int fbWidth, fbHeight;
    float pxRatio;
    float gpuTimes[3];
    int i, n;

    t = glfwGetTime();
    dt = t - prevt;
    prevt = t;

    startGPUTimer(&gpuTimer);

    glfwGetCursorPos(window, &mx, &my);
    m1 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    pxRatio = (float)fbWidth / (float)winWidth;

    // Update and render
    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

    vpl_ide_interact(&ide, m1, mx, my);
    vpl_ide_draw(&ide, nodes, ARRAY_SIZE(nodes));

    renderGraph(vg, 5,5, &fps);
    renderGraph(vg, 5+200+5,5, &cpuGraph);
    if (gpuTimer.supported)
      renderGraph(vg, 5+200+5+200+5,5, &gpuGraph);

    nvgEndFrame(vg);

    // Measure the CPU time taken excluding swap buffers (as the swap may wait
    // for GPU)
    cpuTime = glfwGetTime() - t;

    updateGraph(&fps, dt);
    updateGraph(&cpuGraph, cpuTime);

    // We may get multiple results.
    n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
    for (i = 0; i < n; i++)
      updateGraph(&gpuGraph, gpuTimes[i]);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  freeDemoData(vg, &data);

  nvgDeleteGL3(vg);

  printf("Average Frame Time: %.2f ms\n", getGraphAverage(&fps) * 1000.0f);
  printf("          CPU Time: %.2f ms\n", getGraphAverage(&cpuGraph) * 1000.0f);
  printf("          GPU Time: %.2f ms\n", getGraphAverage(&gpuGraph) * 1000.0f);

  glfwTerminate();
  return 0;
}
