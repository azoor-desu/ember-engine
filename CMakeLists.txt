# https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html
# https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
# https://cmake.org/cmake/help/latest/command/add_library.html
# Targets are either LIBS or EXES or a bunch of header-only files. Each target can contain compilation settings for that build.
# It's also possible to chain multiple targets together so that when you build your main project, the dependencies get compiled as well.

cmake_minimum_required(VERSION 3.29.5)
project(EmberEngine 
        VERSION 0.1.0
        DESCRIPTION "Real Time simulation engine...?"
        LANGUAGES C CXX
        )

# CPP language version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Make CMAKE generate compile_commands.json in /out folder
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# sets the CWD of exec to some other place.
set(WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# throw all .cmake files into /cmake/
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH}) 

# set build output directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if (MSVC)
    add_compile_options(/W4) # warning level 4
    add_compile_options(/MP /INCREMENTAL) # parallel compile, incremental linking
    add_compile_definitions(NOMINMAX)
    add_compile_options(/wd4099 /wd4477 /wd4100 /wd4305)
else()
    add_compile_options(-fno-exceptions) # disable exceptions globally. Yeet.
    add_compile_options(-Wall -Wextra -Wpedantic) # additional warnings
endif()

# ======================== BUILD TOGGLES ========================
# Moved to cmake-variants.yaml and toggled via cmake UI
#set(CMAKE_BUILD_TYPE Release)
#set(PROJ_BUILD_TYPE Editor) # Editor || Engine

# ======================== START CREATE TARGETS ========================
# STATIC - LIB. Requires header files to use.
# SHARED - DLLs with symbols.
# MODULE - DLLs without symbols. Cannot be used to build.
add_library(UtilsLib STATIC)
add_library(EngineLib STATIC)
add_executable(MainExe)

if (PROJ_BUILD_TYPE MATCHES Engine)
    set(PROJ_OUTPUT_NAME "EmberEngine")
else()
    set(PROJ_OUTPUT_NAME "EmberEditor")
endif()

if (PLATFORM_TYPE MATCHES Windows)
    set(PROJ_OUTPUT_SUFFIX ".exe")
else()
    set(PROJ_OUTPUT_SUFFIX ".out")
endif()

set_target_properties(
    MainExe
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR} # sets the output dir of the exec to root folder
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
        OUTPUT_NAME ${PROJ_OUTPUT_NAME}-${CMAKE_BUILD_TYPE}
        SUFFIX ${PROJ_OUTPUT_SUFFIX}
)

set_target_properties(
    UtilsLib
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}Utils${CMAKE_BUILD_TYPE}
)

set_target_properties(
    EngineLib
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}${CMAKE_BUILD_TYPE}
)

# enable pch
target_precompile_headers(EngineLib PRIVATE src/pch-engine.h) 
if (PROJ_BUILD_TYPE MATCHES Editor)
    target_precompile_headers(MainExe PRIVATE src/pch-editor.h)
endif()

# forward definitions to preprocessor
if (PLATFORM_TYPE MATCHES Linux)
    target_compile_definitions(EngineLib PRIVATE EMB_PLATFORM_LINUX)
elseif(PLATFORM_TYPE MATCHES Windows)
    target_compile_definitions(MainExe PRIVATE EMB_PLATFORM_WINDOWS)
endif()

#target_compile_definitions(MainExe PRIVATE EMB_USE_VULKAN)
#target_compile_definitions(MainExe PRIVATE EMB_USE_OPENGL)

# ======================== END CREATE TARGETS ========================

# ======================== START TARGET SOURCES ========================
# Adding source files (if any)
# target_sources(
#     MainExe
#     PRIVATE 
#         src/main.cpp
# )

# Adding include dirs (if any):
# PUBLIC: Any other targets that link to this target needs to have this include dir too
# PRIVATE: Any other targets that link to this target will ignore this dir
# INTERFACE: Don't apply to this target, apply for others.
target_include_directories(
    EngineLib
    PUBLIC
        src # For source file includes
)

target_include_directories(
    MainExe
    PUBLIC
        src # For source file includes
)

# add all direct subdirs here
add_subdirectory(src)
add_subdirectory(lib)
#...
# ======================== END TARGET SOURCES ========================

# ======================== START LINKING ========================
# Allows setting library dependencies.
# PRIVATE - [ LibA [LibB]] >> LibA links to PRIVATE LibB. Other linking Libs cannot use LibB
# PUBLIC - [ LibA ][ LibB ] >> LibA links to PUBLIC LibB. Other linking Libs can use LibB
# INTERFACE - [ LibA ][ LibB (Partially) ] >> LibA links to LibB, but does not require B internally. Used for header-only library.
target_link_libraries(
    EngineLib
    PUBLIC
        UtilsLib
)
target_link_libraries(
    MainExe
    PUBLIC
        EngineLib
)
# ======================== END LINKING ========================


# ======================== MISC COMMANDS ========================
# pulls git?
#execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                RESULT_VARIABLE GIT_SUBMOD_RESULT)

# creates symlink of compile_commands.json in root.
# EDIT: Not needed right now. clangd auto searches the BINARY_DIR then the root, so redundant.
# if (UNIX)
# file(CREATE_LINK
#   "${CMAKE_BINARY_DIR}/compile_commands.json"
#   "${CMAKE_SOURCE_DIR}/compile_commands.json"
#   SYMBOLIC
# )
# endif()

# copies the exec from build folder to root
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}
#)
