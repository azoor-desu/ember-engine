target_sources(
    MainExe
    PRIVATE 
        
)

# Create uutils lib
add_library(UtilsLib STATIC)

target_sources(
    UtilsLib
    PRIVATE 
        vec_base.cpp
)
