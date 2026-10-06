add_executable(cli_minesweeper
                main.cpp
                common.cpp
                render.cpp)

target_include_directories(cli_minesweeper PRIVATE include)
