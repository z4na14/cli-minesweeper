#include <sstream>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>

#include "runtime.hpp"
#include "render.hpp"
#include "common.hpp"
#include "game.hpp"


namespace runtime {
    // Shared info between main thread and character reader
    std::mutex mutex;
    bool update_input{false};
    std::atomic<bool> terminate{false};

    void init_bootstrap() {
        try {
            // Empty screen before doing anything
            std::cout << render::controls::CLEAR_BUFFER
                    << render::controls::CLEAR_SCREEN
                    << render::controls::HIDE_CURSOR
                    << render::controls::ENABLE_MOUSE
                    << render::controls::ENABLE_SGR1006 << std::flush;

            render::termSize = render::get_term_size();

#ifdef NDEBUG
            if (render::termSize.cols < 70 || render::termSize.rows < 30)
                throw std::runtime_error(
                    "Terminal size is too small!\nConsider making it bigger");
#endif

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

    void loop_runtime(game::Table *table) {
        std::thread inputThread(render::get_term_input);

        std::cout << render::controls::CLEAR_SCREEN;
        table->print_table();

        while (true) {
            // Update term size
            if (render::termSize_t newSize = render::get_term_size(); newSize != render::termSize) {
                render::termSize = newSize;
                std::cout << render::controls::CLEAR_SCREEN;
                table->print_table();
            }

            // Local thread events
            update_timer(table->start_timer);

            // Reading input events
            {
                // Locked while in scope. I had no fucking idea cpp allowed this
                std::unique_lock lock(mutex);
                if (update_input) {
                    // Leer las weadas
                    //std::cout << render::pressed_x << ";" << render::pressed_y;

                    if (render::pressed_button == render::click_buttons::LEFT_BUTTON) {
                        if (table->modify_cell(render::pressed_x - table->top_x,
                                               render::pressed_y - table->top_y,
                                               " ",
                                               {},
                                               {
                                                   render::colors::WHITE[0], render::colors::WHITE[1],
                                                   render::colors::WHITE[2]
                                               })
                            != 0) {
                            continue;
                        }
                    }

                    update_input = false;
                }
            }
            // Check at the end of the mutex if the program terminated
            if (terminate) break;
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
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
        const auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() - seconds * 1000;

        render::move_cursor(render::termSize.cols - 10, render::termSize.rows - 3);
        std::cout << seconds << ":" << miliseconds << std::flush;
    }
}
