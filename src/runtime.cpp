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
    std::mutex MUTEX;
    bool UPDATE_INPUT{false};
    std::atomic<bool> TERMINATE{false};
    game_status GAME_STATUS{GAME};

    void init_bootstrap() {
        try {
            // Empty screen before doing anything
            std::cout << render::controls::CLEAR_BUFFER
                    << render::controls::CLEAR_SCREEN
                    << render::controls::HIDE_CURSOR
                    << render::controls::ENABLE_MOUSE
                    << render::controls::ENABLE_SGR1006 << std::flush;

            render::TERM_SIZE = render::get_term_size();

#ifdef NDEBUG
            if (render::TERM_SIZE.cols < 70 || render::TERM_SIZE.rows < 30)
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
        game::global_game_table = table;
        std::thread inputThread(render::get_term_input);

        std::cout << render::controls::CLEAR_SCREEN;
        table->print_table();

        while (true) {
            // Update term size
            if (render::termSize_t newSize = render::get_term_size(); newSize != render::TERM_SIZE) {
                render::TERM_SIZE = newSize;
                std::cout << render::controls::CLEAR_SCREEN;
                table->print_table();
            }

            // Local thread events
            update_timer(table->start_timer);

            // Reading input events
            {
                // Locked while in scope. I had no fucking idea cpp allowed this
                std::unique_lock lock(MUTEX);
                if (UPDATE_INPUT) {
                    // Leer las weadas
                    print_logs(std::format("Parsed Button: {}, X: {}, Y: {}",
                                (int)render::PRESSED_BUTTON, render::PRESSED_X, render::PRESSED_Y));

                    if (render::PRESSED_BUTTON == render::click_buttons::LEFT_BUTTON) {
                        table->send_left_click(render::PRESSED_X, render::PRESSED_Y);
                    } else if (render::PRESSED_BUTTON == render::click_buttons::RIGHT_BUTTON) {
                        table->send_right_click(render::PRESSED_X, render::PRESSED_Y);
                    }

                    UPDATE_INPUT = false;
                }
            }
            // Check at the end of the mutex if the program terminated
            if (TERMINATE) break;
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

    void update_timer(const std::chrono::time_point<std::chrono::steady_clock> start_timer) {
        const auto diff = std::chrono::steady_clock::now() - start_timer;
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
        const auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() - seconds *
                                 1000;

        render::move_cursor(render::TERM_SIZE.cols - 10, render::TERM_SIZE.rows - 3);
        std::cout << seconds << ":" << miliseconds << std::flush;
    }

    void print_logs(const std::string &message) {
#ifndef NDEBUG
        render::move_cursor(0, render::TERM_SIZE.rows);
        std::cerr << "[[ LOG: " << message << " ]]" << std::flush;
#endif
    }
}
