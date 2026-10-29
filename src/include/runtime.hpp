#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "game.hpp"
#include <atomic>
#include <mutex>

namespace runtime {
struct Params {
    unsigned int level = 0;
    // More binary params will be added here
};

extern std::mutex MUTEX;
extern std::atomic<bool> UPDATE_INPUT;
extern std::atomic<bool> TERMINATE;
extern std::atomic<bool> DEAD;

// ----------------------------------------------------------------------

void init_bootstrap(game::Table *table);

void loop_runtime(game::Table *table);

void cleanup_bootstrap(game::Table const *table);

void update_timer(
    std::chrono::time_point<std::chrono::steady_clock> start_timer);

void arg_parser(std::span<const char *> &args, Params &params);
} // namespace runtime

#endif // GLOBAL_HPP
