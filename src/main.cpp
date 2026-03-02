#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <span>
#include <string>

#include "common.hpp"
#include "render.hpp"

int main(int const argc, char const **argv) {
    //std::span<const char*> args(argv, argc);

    render::init_bootstrap();
    render::loop_runtime();

    return 0;
}

/*


// Helper to set raw mode (same as previous example)


int main() {
    setRawMode(true);

    char c;
    while (read(STDIN_FILENO, &c, 1) && c != 'q') {
        if (c == '\033') { // Escape character
            if (read(STDIN_FILENO, &c, 1) && c == '[') {
                if (read(STDIN_FILENO, &c, 1) && c == '<') {
                    // We are in an SGR sequence! Read until M or m
                    std::string buffer = "";
                    while (read(STDIN_FILENO, &c, 1) && c != 'M' && c != 'm') {
                        buffer += c;
                    }

                    // buffer now contains "button;x;y" (e.g., "0;45;12")
                    bool pressed = (c == 'M');
                    int button, x, y;
                    sscanf(buffer.c_str(), "%d;%d;%d", &button, &x, &y);

                    std::cout << "\r" << (pressed ? "Pressed " : "Released ")
                              << "Button " << button << " at [" << x << "," << y << "]       ";
                    std::cout.flush();
                }
            }
        }
    }

    // Clean up
    std::cout << "\033[?1006l\033[?1000l" << std::endl;
    setRawMode(false);
    return 0;
}*/