# Dump all the LIB stuff here

# throw error if var is not defined for whatever reason (happened once >:()
if (NOT DEFINED EMB_DEF_PLATFORM OR EMB_DEF_PLATFORM STREQUAL "")
    message(FATAL_ERROR "EMB_DEF_PLATFORM is not defined!")
endif()

if (EMB_DEF_PLATFORM MATCHES Linux)
    # Uses libraries/packages from built-in package manager.
    # Finds the pkg-config executable, adds the pkg_check_modules() function, among others.
    find_package(PkgConfig REQUIRED)
endif()

# ======================= GLFW =======================
if (EMB_DEF_PLATFORM MATCHES Linux)
    pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
    target_link_libraries(EngineLib PUBLIC PkgConfig::glfw3)

elseif(EMB_DEF_PLATFORM MATCHES Windows)
    target_include_directories(EngineLib PUBLIC glfw/include)
    target_link_directories(EngineLib PUBLIC glfw/lib-vc2022) # build static lib and dump here
    target_link_libraries(EngineLib PUBLIC glfw3) # name of static lib

endif()

# ======================= GLEW =======================
if (EMB_DEF_PLATFORM MATCHES Linux)
    pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)
    target_link_libraries(EngineLib PUBLIC PkgConfig::glew)

elseif(EMB_DEF_PLATFORM MATCHES Windows)
    target_include_directories(EngineLib PUBLIC glew/include)
    target_link_directories(EngineLib PUBLIC glew/lib/x64)  # build static lib and dump here
    target_link_libraries(EngineLib PUBLIC glew32) # name of static lib
    # also requires .dll beside the exe...
    
endif()

# ======================= GLM =======================
if (EMB_DEF_PLATFORM MATCHES Linux)
    #pkg_check_modules(glm REQUIRED IMPORTED_TARGET glm)
    #target_link_libraries(UtilsLib PUBLIC PkgConfig::glm) # doesn't seem to be required...? Builds without including glm

elseif(EMB_DEF_PLATFORM MATCHES Windows)
    target_include_directories(UtilsLib PUBLIC glm)
    target_include_directories(EngineLib PUBLIC glm)

endif()

# ======================= OpenGL =======================
if (EMB_DEF_PLATFORM MATCHES Linux)
    #pkg_check_modules(vulkan REQUIRED IMPORTED_TARGET vulkan)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    target_link_libraries(EngineLib PUBLIC OpenGL::OpenGL)

elseif(EMB_DEF_PLATFORM MATCHES Windows)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    target_link_libraries(EngineLib PUBLIC OpenGL::GL)

endif()

# ======================= IMGUI =======================
# No packages avail, manually download and include headers.
if (EMB_DEF_BUILD_APP_TYPE MATCHES Editor)
    # Adding header dirs (if any)
    target_include_directories(
        MainExe
        PUBLIC
            imgui  # for imgui internal headers to work
    )

    # Adding source files (if any)
    target_sources(
        MainExe
        PRIVATE 
            imgui/imgui.cpp
            imgui/imgui_demo.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_tables.cpp
            imgui/imgui_widgets.cpp
            imgui/backends/imgui_impl_glfw.cpp
            imgui/backends/imgui_impl_opengl3.cpp
    )
endif()