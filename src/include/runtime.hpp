#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <mutex>
#include "game.hpp"


namespace runtime {
    extern std::mutex mutex;
    extern bool update_input;
    extern bool terminate;

    // ----------------------------------------------------------------------

    void init_bootstrap();

    void loop_runtime(game::Table const *table);

    void cleanup_bootstrap(game::Table const *table);

    inline void update_timer(std::chrono::time_point<std::chrono::steady_clock> start_timer);
}

#endif //GLOBAL_HPP
