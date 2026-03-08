#include "common.hpp"
#include "render.hpp"


namespace common {
    void print_error(const std::string &msg) {
        render::move_cursor(0,0);
        const cell error{
            msg,
            {render::colors::RED[0], render::colors::RED[1], render::colors::RED[2]}, {}
        };
        std::cerr << '\r' << error << std::endl;
    }
}
