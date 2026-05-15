#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <mutex>
#include <atomic>
#include "game.hpp"


namespace runtime {
    enum game_status {
        SELECTION = 0,
        GAME = 1,
        END = 2,
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

    void print_logs(const std::string &message);
}

#endif //GLOBAL_HPP
