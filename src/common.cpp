#include "common.hpp"

namespace common {
    /* Overloading the color operators for both templates */
    template <Layer L>
    std::ostream& operator<< (std::ostream& os, color<L> const& c) {
        return os << "\x1b[" << L << ";2;"
              << static_cast<int>(c.r) << ";"
              << static_cast<int>(c.g) << ";"
              << static_cast<int>(c.b) << "m";
    }

    /* Overloading the cell struct using the previous overloads */
    std::ostream& operator<< (std::ostream& os, cell const& c) {
        os << c.fg << c.bg << c.val << color_reset;
        return os;
    }
}