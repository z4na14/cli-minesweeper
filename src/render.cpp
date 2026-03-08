#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <termios.h>
#include <sstream>
#include <mutex>
#include <chrono>

#include "render.hpp"
#include "runtime.hpp"


namespace render {
    termSize_t termSize;

    int pressed_x{}, pressed_y{};
    click_buttons pressed_button{};


    termSize_t get_term_size() {
        winsize w{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0)
            throw std::runtime_error("Unable to get term size");

        return {w.ws_row, w.ws_col};
    }

    void get_term_input() {
        char c;
        while (std::cin.get(c) && c != 'q') {
            // Getting the escape sequence right, and read the input from it
            if (c == '\033' and
                std::cin.get() == '[' and
                std::cin.get() == '<') {
                int button, x, y;
                char sep1, sep2, finalChar;

                // button pressed - separator - x - separator - y - end of line
                std::cin >> button >> sep1 >> x >> sep2 >> y >> finalChar;

                // The CLI returns when the button is pressed and released
                // True: Down / False: Up
                if (finalChar == 'M') {
                    std::unique_lock lock(runtime::mutex);

                    // Reported section is offset to what
                    // I would expect to "have clicked"
                    pressed_x = x - 1;
                    pressed_y = y - 1;
                    pressed_button = static_cast<click_buttons>(button);

                    runtime::update_input = true;
                }
            }
        } {
            std::unique_lock lock(runtime::mutex);
            runtime::terminate = true;
        }
    }

    void move_cursor(const int x, const int y) {
        if (x > termSize.cols or y > termSize.rows) {
            common::print_error("Term size too small, make it bigger");
            return;
        }
        std::cout << "\033[" << y << ";" << x << "H" << std::flush;
    }

    void set_raw_mode(bool const enable) {
        static termios oldt, newt;
        if (enable) {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }
}
