#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <unistd.h>

#include "common.hpp"
#include "render.hpp"
#include "game.hpp"
#include "runtime.hpp"


int main(int const argc, char const **argv) {
    //std::span<const char*> args(argv, argc);

    runtime::init_bootstrap();

    game::Table *game = new game::HardGame{};

    runtime::loop_runtime(game);
}
