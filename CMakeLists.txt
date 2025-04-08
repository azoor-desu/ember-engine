# https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html
# https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
# https://cmake.org/cmake/help/latest/command/add_library.html
# Targets are either LIBS or EXES or a bunch of header-only files. Each target can contain compilation settings for that build.
# It's also possible to chain multiple targets together so that when you build your main project, the dependencies get compiled as well.

cmake_minimum_required(VERSION 3.30.0)
project(EmberEngine 
        VERSION 0.1.0
        DESCRIPTION "Real Time simulation engine...?"
        LANGUAGES C CXX
        )

# CPP language version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Make CMAKE generate compile_commands.json in /out folder
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# sets the CWD of exec to some other place.
set(WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# throw all .cmake files into /cmake/
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH}) 

# set build output directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# disable exceptions. Yeet.
add_compile_options(-fno-exceptions)

# Create EXE, set some properties.
add_executable(MainExe)
set_target_properties(
    MainExe 
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR} # sets the output dir of the exec to root folder
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
        OUTPUT_NAME "EmberEngine"
        #SUFFIX ".out"
)

# enable pch
target_precompile_headers(MainExe PRIVATE src/pch.h)

# ======================== START EXE SRC FILE INCLUSION ========================
# List source files in this dir
# target_sources(
#     MainExe
#     PRIVATE 
#         main.cpp
# )

# add all direct subdirs here
add_subdirectory(src)

# Adding include dirs (if needed):
# PUBLIC: Any other targets that link to this target needs to have this include dir too
# PRIVATE: Any other targets that link to this target will ignore this dir
# INTERFACE: Don't apply to this target, apply for others.
target_include_directories(
    MainExe
    PUBLIC
        src
)
#...
# ======================== END EXE SRC FILE INCLUSION ========================

#message(STATUS "ehe")

# ======================== START CREATE LIBS ========================
# STATIC - LIB. Requires header files to use.
# SHARED - DLLs with symbols.
# MODULE - DLLs without symbols. Cannot be used to build.

# Library number 1
#add_library(Lib1 STATIC)
# target_sources(Lib1
#     PRIVATE 
#         main.cpp
# )

# Extern/package libs.
if (UNIX)
# Uses libraries/packages from built-in package manager.
# Finds the pkg-config executable, adds the pkg_check_modules() function, among others.
find_package(PkgConfig REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(vulkan REQUIRED IMPORTED_TARGET vulkan)
# target_include_directories(
#    PkgConfig::glfw3
#    PRIVATE 
#        ${OPENGL_INCLUDE_DIRS}
# )
endif()
# ======================== END CREATE LIBS ========================

# ======================== START LINKING ========================
# Allows setting library dependencies.
# PRIVATE - [ LibA [LibB]] >> LibA links to PRIVATE LibB. Other linking Libs cannot use LibB
# PUBLIC - [ LibA ][ LibB ] >> LibA links to PUBLIC LibB. Other linking Libs can use LibB
# INTERFACE - [ LibA ][ LibB (Partially) ] >> LibA links to LibB, but does not require B internally. Used for header-only library.
if (UNIX)
target_link_libraries(
    MainExe
    PUBLIC 
        PkgConfig::vulkan 
        PkgConfig::glfw3
)
endif()

# Add other links here...
#target_link_libraries(
#MainExe
#    PUBLIC Lib1
#    PRIVATE Lib2
#)

# ======================== END LINKING ========================

# pulls git?
#execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                RESULT_VARIABLE GIT_SUBMOD_RESULT)

# creates symlink of compile_commands.json in root.
if (UNIX)
file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)
endif()

# copies the exec from build folder to root
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}
#)
