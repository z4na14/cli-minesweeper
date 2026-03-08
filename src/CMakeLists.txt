add_executable(cli_minesweeper
                main.cpp
                common.cpp
                render.cpp
                runtime.cpp
                game.cpp)

target_include_directories(cli_minesweeper PRIVATE include)
