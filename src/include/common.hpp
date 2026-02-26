#ifndef COMMON_HPP
#define COMMON_HPP
#include <iostream>

typedef enum {
    Foreground = 38,
    Background = 48
} Layer;

namespace common {
    constexpr std::string color_reset {"\x1b[0m"};

    template <Layer L>
    struct color {
        uint8_t r, g, b;
        color(const uint8_t r,
              const uint8_t g,
              const uint8_t b): r(r), g(g), b(b) {}
        color(): r(255), g(255), b(255) {}

        friend std::ostream& operator<< (std::ostream& os, color const& c);
    };

    struct cell {
        std::string_view val {};
        color<Foreground> fg {};
        color<Background> bg {};

        cell(std::string const& val,
             color<Foreground> const& fg,
             color<Background> const& bg): val(val), fg(fg), bg(bg) {}

        friend std::ostream& operator<< (std::ostream& os, cell const& c);
    };
}

#endif //COMMON_HPP