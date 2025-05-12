if (PROJ_BUILD_TYPE MATCHES Engine)
    target_sources(
        MainExe
        PRIVATE 
            main-engine.cpp
            pch-engine.cpp
    )
    add_subdirectory(engine)
else()
    target_sources(
        MainExe
        PRIVATE 
            main-editor.cpp
            pch-editor.cpp
    )
endif()

add_subdirectory(util)
add_subdirectory(tools)
