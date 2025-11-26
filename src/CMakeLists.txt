add_library(src)
target_sources(src PUBLIC
        main.cpp
)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)