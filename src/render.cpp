#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <termios.h>
#include <sstream>

#include "render.hpp"

#include <mutex>


namespace render {
    termSize_t termSize;

    void init_bootstrap() {
        try {
            // Empty screen before doing anything
            std::cout << controls::CLEAR_BUFFER
                    << controls::CLEAR_SCREEN
                    << controls::HIDE_CURSOR
                    << controls::ENABLE_MOUSE
                    << controls::ENABLE_SGR1006 << std::flush;

            termSize = get_termSize();
            if (termSize.cols < 70 || termSize.rows < 30)
                throw std::runtime_error(
                    "Terminal size is too small!\nConsider making it bigger");

            setRawMode(true);
        } catch (std::exception const &e) {
            const common::cell error{
                e.what(),
                {colors::RED[0], colors::RED[1], colors::RED[2]}, {}
            };
            std::cerr << error << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void loop_runtime() {
        // Divided between actual buffer, and a view as an atomic type for
        // accesses between threads
        std::string buff{};
        bool update{false};
        bool terminate{false};

        std::mutex mutex;
        std::thread inputThread(get_term_input_, std::ref(buff));
        char c;

        while (!terminate) {
            // Update term size
            if (termSize_t newSize = get_termSize(); newSize != termSize) {
                termSize = newSize;
                std::cout << termSize.cols << "x" << termSize.rows << std::endl;
            } {
                // Locked while in scope. I had no fucking idea cpp allowed this
                std::unique_lock lock(mutex);
                if (update) {
                    // Leer las weadas
                }
            }

            usleep(FRAMERATE_PERIOD);
        }

        inputThread.join();
        cleanup_bootstrap();
    }

    void cleanup_bootstrap() {
        std::cout << controls::CLEAR_SCREEN
                << controls::SHOW_CURSOR
                << controls::DISABLE_MOUSE
                << controls::DISABLE_SGR1006 << std::flush;
        setRawMode(false);
        exit(EXIT_SUCCESS);
    }

    termSize_t get_termSize() {
        winsize w{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0)
            throw std::runtime_error("Unable to get term size");

        return {w.ws_row, w.ws_col};
    }

    void setRawMode(bool const enable) {
        static struct termios oldt, newt;
        if (enable) {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }

    /*
    void get_term_input_(std::string &buff) {
        if (std::cin.get(c) && c != 'q') {
        } else if (c == 'q')
    }*/
}
