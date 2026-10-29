#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <format>
#include <mutex>
#include <stdlib.h>
#include <thread>
#include <unistd.h>

#include "common.hpp"
#include "game.hpp"
#include "render.hpp"
#include "runtime.hpp"

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

        if (render::TERM_SIZE.cols < 70 || render::TERM_SIZE.rows < 30)
            throw std::runtime_error(
                "Terminal size is too small!\nConsider making it bigger");

        render::set_raw_mode(true);
    } catch (std::exception const &e) {
        const common::cell error{e.what(),
                                 {render::colors::RED[0],
                                  render::colors::RED[1],
                                  render::colors::RED[2]},
                                 {}};
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
        if (render::termSize_t newSize = render::get_term_size();
            newSize != render::TERM_SIZE) {
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
                common::print_logs(
                    std::format("Parsed Button: {}, X: {}, Y: {}",
                                (int)render::PRESSED_BUTTON, render::PRESSED_X,
                                render::PRESSED_Y));

                if (render::PRESSED_BUTTON ==
                    render::click_buttons::LEFT_BUTTON) {
                    table->send_left_click(render::PRESSED_X,
                                           render::PRESSED_Y);
                } else if (render::PRESSED_BUTTON ==
                           render::click_buttons::RIGHT_BUTTON) {
                    table->send_right_click(render::PRESSED_X,
                                            render::PRESSED_Y);
                }

                UPDATE_INPUT = false;
            }
        }
        // Check at the end of the mutex if the program terminated
        if (TERMINATE)
            break;
        usleep(render::FRAMERATE_PERIOD);
    }

    inputThread.join();
    cleanup_bootstrap(table);
}

void cleanup_bootstrap(game::Table const *table) {
    std::cout << render::controls::CLEAR_SCREEN << render::controls::SHOW_CURSOR
              << render::controls::DISABLE_MOUSE
              << render::controls::DISABLE_SGR1006 << std::flush;
    render::set_raw_mode(false);

    delete table;

    exit(EXIT_SUCCESS);
}

void update_timer(
    const std::chrono::time_point<std::chrono::steady_clock> start_timer) {
    const auto diff = std::chrono::steady_clock::now() - start_timer;
    const auto seconds =
        std::chrono::duration_cast<std::chrono::seconds>(diff).count();
    const auto miliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() -
        seconds * 1000;

    render::move_cursor(render::TERM_SIZE.cols - 10,
                        render::TERM_SIZE.rows - 3);
    std::cout << seconds << ":" << miliseconds << std::flush;
}

void arg_parser(std::span<const char *> &args, runtime::Params &params) {
    if (args.size() % 2 != 1) {
        common::print_error("Invalid number of arguments");
        return;
    }

    for (std::size_t i = 1; i < args.size(); i += 2) {
        if (std::strcmp(args[i], "--mode") == 0) {
            params.level = static_cast<unsigned int>(std::stoi(args[i + 1]));
        }
    }
}
} // namespace runtime
