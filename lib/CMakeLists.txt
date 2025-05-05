# Dump all the LIB stuff here

#========= LINUX ONLY =========
# Looks for linux packages. For windows, auto download them or something.
if (UNIX)
    # Uses libraries/packages from built-in package manager.
    # Finds the pkg-config executable, adds the pkg_check_modules() function, among others.
    find_package(PkgConfig REQUIRED)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)
    #pkg_check_modules(vulkan REQUIRED IMPORTED_TARGET vulkan)
    pkg_check_modules(glm REQUIRED IMPORTED_TARGET glm)

    # do linkage here too, cos the scope of these packages are lost outside of this file.
    target_link_libraries(
        UtilsLib
        PUBLIC 
            PkgConfig::glm # doesn't seem to be required...? Builds without including glm
    )
    target_link_libraries(
        MainExe
        PUBLIC
            PkgConfig::glm
            OpenGL::OpenGL
            PkgConfig::glfw3
    )
endif()

# ========= IMGUI =========
# No packages avail, manually download and include headers.
if (PROJ_BUILD_TYPE MATCHES Editor)
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