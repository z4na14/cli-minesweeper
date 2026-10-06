add_library(src)
target_sources(src PUBLIC
        main.cpp
        common.cpp
)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)