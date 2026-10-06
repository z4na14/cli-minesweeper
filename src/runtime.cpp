#include <sstream>
#include <mutex>
#include <chrono>
#include <thread>

#include "runtime.hpp"
#include "render.hpp"
#include "common.hpp"
#include "game.hpp"


namespace runtime {
    // Shared info between main thread and character reader
    std::mutex mutex;
    bool update_input{false};
    bool terminate{false};

    void init_bootstrap() {
        try {
            // Empty screen before doing anything
            std::cout << render::controls::CLEAR_BUFFER
                    << render::controls::CLEAR_SCREEN
                    << render::controls::HIDE_CURSOR
                    << render::controls::ENABLE_MOUSE
                    << render::controls::ENABLE_SGR1006 << std::flush;

            render::termSize = render::get_term_size();
            if (render::termSize.cols < 70 || render::termSize.rows < 30)
                throw std::runtime_error(
                    "Terminal size is too small!\nConsider making it bigger");

            render::set_raw_mode(true);
        } catch (std::exception const &e) {
            const common::cell error{
                e.what(),
                {render::colors::RED[0], render::colors::RED[1], render::colors::RED[2]}, {}
            };
            std::cerr << error << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void loop_runtime(game::Table const *table) {
        std::thread inputThread(render::get_term_input);

        while (true) {
            // Update term size
            if (render::termSize_t newSize = render::get_term_size(); newSize != render::termSize) {
                render::termSize = newSize;
                std::cout << render::controls::CLEAR_SCREEN;
            }

            // Local thread events
            update_timer(table->start_timer);

            // Reading input events
            {
                // Locked while in scope. I had no fucking idea cpp allowed this
                std::unique_lock lock(mutex);
                if (update_input) {
                    // Leer las weadas
                    render::move_cursor(render::termSize.cols / 2, render::termSize.rows / 2);
                    std::cout << render::pressed_x << ";" << render::pressed_y;

                    update_input = false;
                }

                // Check at the end of the mutex if the program terminated
                if (terminate) break;
            }
            usleep(render::FRAMERATE_PERIOD);
        }

        inputThread.join();
        cleanup_bootstrap(table);
    }

    void cleanup_bootstrap(game::Table const *table) {
        std::cout << render::controls::CLEAR_SCREEN
                << render::controls::SHOW_CURSOR
                << render::controls::DISABLE_MOUSE
                << render::controls::DISABLE_SGR1006 << std::flush;
        render::set_raw_mode(false);

        delete table;

        exit(EXIT_SUCCESS);
    }

    inline void update_timer(const std::chrono::time_point<std::chrono::steady_clock> start_timer) {
        const auto diff = std::chrono::steady_clock::now() - start_timer;
        render::move_cursor(render::termSize.cols - 10, render::termSize.rows - 3);
        std::cout << std::chrono::duration_cast<std::chrono::seconds>(diff) << std::flush;
    }
}
