#include <unistd.h>

#include "common.hpp"
#include "game.hpp"
#include "runtime.hpp"

int main(int const argc, char const **argv) {
    std::span<const char *> args(argv, argc);
    runtime::Params params{};
    runtime::arg_parser(args, params);

    game::Table *game;
    switch (params.level) {
    case 0:
        game = new game::EasyGame{};
        break;

    case 1:
        game = new game::MediumGame{};
        break;

    case 2:
        game = new game::HardGame{};
        break;

    default:
        common::print_error("Undefined difficulty param. Use between 0 and 2.");
        return -1;
    }

    runtime::init_bootstrap(game);
    runtime::loop_runtime(game);
}
