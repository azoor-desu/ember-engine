target_sources(
    EngineLib
    PRIVATE 
        pch-engine.cpp
)

if (EMB_DEF_BUILD_APP_TYPE MATCHES Editor)
    target_sources(
        MainExe
        PRIVATE 
            main-editor.cpp
            pch-editor.cpp
    )
else()
    target_sources(
        MainExe
        PRIVATE 
            main-engine.cpp
    )
endif()

add_subdirectory(engine)
add_subdirectory(util)
