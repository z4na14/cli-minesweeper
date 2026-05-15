#include <unistd.h>

#include "game.hpp"
#include "runtime.hpp"


int main(int const argc, char const **argv) {
    //std::span<const char*> args(argv, argc);

    runtime::init_bootstrap();

    game::Table *game = new game::EasyGame{};

    runtime::loop_runtime(game);
}
