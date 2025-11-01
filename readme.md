# Ember Engine

Experimental real-time 2D game engine, created as a hobby.  
Goal is to implement my knowledge so far into an application and create a game from this.  
Engine will be built to be cross-platform because I am daily driving Linux as an OS.  

## Features

- OpenGL rendering backend
- Fixed and variable timestep

TBD

## Build
> Note: This project is developed and tested with Clang 20.1.0 x86_64-suse-linux (OpenSUSE Tumbleweed). Minor testing on Windows will be carried out from time to time but no gurantees that it will be fully working at all times.

The project is compilable cross-platform (Windows/Linux) with a focus on Linux.

Download or clone the project, and open the project in VScode.  
Use the CMake extension available to configure the project.  
Navigate to the "Configure" option in the CMake extension, and select the corresponding build variant. (Debug/Release, Windows/Linux)  
Build the project and it should run.  
> Note: There should be a proper way to make and configure the project with the CMake CLI directly but I'm developing on VScodium and using the CMake extension which does all of the stuff for me under the hood, so I'm too lazy to go and figure out the exact commands used.  

### Linux specific libraries
To build the project, these packages need to be installed (non-exhaustive):  
- libasan (for Address Sanitizer)
- libglfw3
- glew-devel
- glm-devel
- gdb (for debugging... maybe?)

List is based on packages available on OpenSUSE Tumbleweed, and may differ slightly depending on Linux distro. 

To have Address Sanitizer produce usable outputs, please add these options to your environment variables before running the executable:  
- `ASAN_SYMBOLIZER_PATH=/usr/local/bin/llvm-symbolizer`
- `LSAN_OPTIONS=suppressions=LSAN.supp`

You can add these lines before calling the executable like so: `ASAN_SYMBOLIZER_PATH=/usr/local/bin/llvm-symbolizer ./a.out`

Another way to easily add it is to append the lines to your `.bashrc` file like so:  
```
...
export ASAN_SYMBOLIZER_PATH=/usr/local/bin/llvm-symbolizer
export LSAN_OPTIONS=suppressions=LSAN.supp
```
However, this adds the env variables system-wide. This makes it easier to work with launching my builds with no debugger and have these exports be available without fiddling with CMake launch options.

### Windows specific build
Windows can use whatever compiler is available (MSVC etc). However, some debug checks like Address Sanitizer won't be available.  
I did not rely on a package manager like conan or vcpkg for Windows. External libraries are manually downloaded and dumped into `/lib` folder.  
There is no auto-download mechanism or auto-build mechanism written (although it can be done).  
- glm
- glfw
- glew

Glfw needs to have its static libraries built and placed in `/lib/glfw/lib-vc2022`, with the library named `glfw3.lib`  
Glew needs to have its static libraries built and placed in `lib/glew/lib/x64`, with the library named `glew32.lib`  
Glew also needs to have a dynamic library (.dll) built and placed next to the output executable (at the root of the project)

## Other Settings
The project is built using VSCodium (OSS version of VSCode without the telemetry).  
I make use of a certain few core extensions for development as listed below:  
- clangd (llvm-vscode-extensions)
- CMake Tools (ms-vscode)
- Native Debug (webfreak)

### Intellisense replacement with clangd
Because I run VSCodium, I do not have access to VSCode's C/C++ extension, and hence am missing out on the Intellisense. An alternative (that can arguably be equivalent) to Intellisense is clangd. By default, no further setup should be required to have the clangd parser work out of the box.  

Before the parser can be fully operational, run the CMake configuration and build once. A `compile_commands.json` should have been generated in `/build` folder. By default, clang looks into this folder for the `compile_commands.json`. If you are still getting errors complaining that certain .h files cannot be found, please check if clang is looking in the right places for this file. 

### Linter with clangd
editor.DefaultFormatter can be set to `clang` to enable the linter to auto-format source code neatly. The formatting options are in the file `/.clang-format`. You can use this website to further customize the format options: https://clang-format-configurator.site/  

### Debugging
When running the project using CMake's build and run, debug features like break points don't work because there is no debugger attached to the executable. Use the extension `Native Debug`'s window `Run and Debug` to run the project with debug capabilities. Running the Editor and Engine executables should be already set up.  

Features such as break points, callstack and register values should be visible in the window for debugging.