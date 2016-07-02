
local action = _ACTION or ""

solution "nanovg"
	location ( "build" )
	configurations { "Debug", "Release" }
	platforms {"native", "x64", "x32"}

	project "vpl-ide"
		kind "ConsoleApp"
		language "C"
		files { "ide/*.c",
            "common/*.c",
            "lang/*.c",
            "deps/vec/vec.c" }
		includedirs { "common", "ide", "lang", "deps" }
		targetdir("build")
		links { "nanovg" }

		configuration { "linux" }
			 -- linkoptions { "`pkg-config --libs SDL2`" }
			 links { "SDL2", "GL", "GLU", "m", "GLEW" }
			 defines { "NANOVG_GLEW" }

		configuration { "windows" }
			 links { "SDL2", "gdi32", "winmm", "user32", "GLEW", "glu32","opengl32", "kernel32" }
			 defines { "NANOVG_GLEW", "_CRT_SECURE_NO_WARNINGS" }

		configuration { "macosx" }
			links { "SDL2" }
			linkoptions { "-framework OpenGL", "-framework Cocoa", "-framework IOKit", "-framework CoreVideo" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols", "ExtraWarnings"}

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize", "ExtraWarnings"}
