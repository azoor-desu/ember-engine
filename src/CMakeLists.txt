target_sources(
    MainExe
    PRIVATE 
        main.cpp
        pch.cpp
        defines.h
)

add_subdirectory(util)
add_subdirectory(debug)