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

    common::cell var {"uwu", {245, 73, 39}, {5, 219, 235}};
    std::cout << var << std::endl;

    return 0;
}