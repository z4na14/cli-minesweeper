cmake_minimum_required(VERSION 4.0)
project("CLI Minesweeper" LANGUAGES CXX)

add_subdirectory(src)
set_property(TARGET src PROPERTY CXX_STANDARD 23)