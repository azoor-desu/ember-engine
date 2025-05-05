target_sources(
    MainExe
    PRIVATE 
        #main.cpp
        pch.cpp
)

add_subdirectory(util)
add_subdirectory(debug)