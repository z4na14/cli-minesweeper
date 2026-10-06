#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

// ANSI escape codes for background and foreground
typedef enum {
    Foreground = 38,
    Background = 48
} Layer;

namespace common {
    constexpr std::string colorReset{"\x1b[0m"};

    template<Layer L>
    struct color {
        uint8_t r, g, b;
        bool transparent;

        // ---

        color(const uint8_t r,
              const uint8_t g,
              const uint8_t b) : r(r), g(g), b(b), transparent(false) {
        }

        color() : r(255), g(255), b(255), transparent(true) {
        }

        // ---

        friend std::ostream &operator<<(std::ostream &os, color const &c) {
            // If we are printing a transparent color, just return nothing
            if (c.transparent) return os;

            return os << "\x1b[" << L << ";2;"
                   << static_cast<int>(c.r) << ";"
                   << static_cast<int>(c.g) << ";"
                   << static_cast<int>(c.b) << "m";
        }
    };

    struct cell {
        std::string val{};
        color<Foreground> fg{};
        color<Background> bg{};

        // ---

        cell(std::string val,
             color<Foreground> const &fg,
             color<Background> const &bg) : val(std::move(val)), fg(fg), bg(bg) {
        }

        explicit cell(std::string val,
                      color<Background> const &bg) : val(std::move(val)), bg(bg) {
        }

        explicit cell(std::string val) : val(std::move(val)), fg({255, 255, 255}), bg({} /*Transparent bg*/) {
        }

        // ---

        friend std::ostream &operator<<(std::ostream &os, cell const &c) {
            return os << c.fg << c.bg << c.val << colorReset;
        }
    };

    void print_error(const std::string &msg);
}

#endif //COMMON_HPP
