#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <span>
#include <string>

#include "common.hpp"
#include "render.hpp"

int main(int const argc, char const **argv) {
    //std::span<const char*> args(argv, argc);

    render::init_runtime();

    return 0;
}