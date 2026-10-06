#ifndef RENDER_HPP
#define RENDER_HPP

#include <string_view>
#include "common.hpp"

namespace render {
    struct interface_chars {
        // Corners
        const std::string_view lt_corner {"┌"};
        const std::string_view lb_corner {"└"};
        const std::string_view rt_corner {"┐"};
        const std::string_view rb_corner {"┘"};

        // Bars
        const std::string_view h_bar {"─"};
        const std::string_view v_bar {"│"};

        // T-Junctions
        const std::string_view t_top    {"┬"};
        const std::string_view t_bottom {"┴"};
        const std::string_view t_left   {"├"};
        const std::string_view t_right  {"┤"};
        const std::string_view cross    {"┼"};
    };

    struct colors {
        const std::array<uint8_t, 3> white  {255,255,255};
        const std::array<uint8_t, 3> red    {255,0  ,0  };
        const std::array<uint8_t, 3> green  {0  ,255,0  };
        const std::array<uint8_t, 3> blue   {0  ,0  ,255};
    };
}

#endif //RENDER_HPP