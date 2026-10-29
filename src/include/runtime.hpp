#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "game.hpp"
#include <atomic>
#include <mutex>

namespace runtime {
enum game_status {
    SELECTION = 0,
    GAME = 1,
    END = 2,
};
struct Params {
    unsigned int level = 0;
    // More binary params will be added here
};

extern std::mutex MUTEX;
extern bool UPDATE_INPUT;
extern std::atomic<bool> TERMINATE;
extern game_status GAME_STATUS;

// ----------------------------------------------------------------------

void init_bootstrap();

void loop_runtime(game::Table *table);

void cleanup_bootstrap(game::Table const *table);

void update_timer(
    std::chrono::time_point<std::chrono::steady_clock> start_timer);

void arg_parser(std::span<const char *> &args, Params &params);
} // namespace runtime

#endif // GLOBAL_HPP
