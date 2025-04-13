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

# disable exceptions globally. Yeet.
add_compile_options(-fno-exceptions)

# ======================== START CREATE TARGETS ========================

# Main EXE target
add_executable(MainExe)
set_target_properties(
    MainExe 
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR} # sets the output dir of the exec to root folder
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
        OUTPUT_NAME "EmberEngine"
        #SUFFIX ".out"
)
target_precompile_headers(MainExe PRIVATE src/pch.h) # enable pch

# STATIC - LIB. Requires header files to use.
# SHARED - DLLs with symbols.
# MODULE - DLLs without symbols. Cannot be used to build.

# Custom LIB targets
add_library(UtilsLib STATIC)

# Extern LIB Targets
if (UNIX)
# Uses libraries/packages from built-in package manager.
# Finds the pkg-config executable, adds the pkg_check_modules() function, among others.
find_package(PkgConfig REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(vulkan REQUIRED IMPORTED_TARGET vulkan)
pkg_check_modules(glm REQUIRED IMPORTED_TARGET glm)
# target_include_directories(
#    PkgConfig::glfw3
#    PRIVATE 
#        ${OPENGL_INCLUDE_DIRS}
# )
endif()
# ======================== END CREATE TARGETS ========================

# ======================== START TARGET SOURCES ========================
# Adding source files (if any)
# target_sources(
#     MainExe
#     PRIVATE 
#         main.cpp
# )

# Adding include dirs (if any):
# PUBLIC: Any other targets that link to this target needs to have this include dir too
# PRIVATE: Any other targets that link to this target will ignore this dir
# INTERFACE: Don't apply to this target, apply for others.
target_include_directories(
    MainExe
    PUBLIC
        src
)

# add all direct subdirs here
add_subdirectory(src)
#...
# ======================== END TARGET SOURCES ========================

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
        PkgConfig::glm
        UtilsLib
)
endif()
# ======================== END LINKING ========================


# ======================== MISC COMMANDS ========================
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
