#ifndef RENDER_HPP
#define RENDER_HPP

#include <string_view>
#include "common.hpp"

namespace render {
    struct controls {
        static constexpr std::string_view clear_screen = "\x1b[2J\x1b[H";
        static constexpr std::string_view clear_line   = "\x1b[2K";
        static constexpr std::string_view clear_buffer = "\x1b[3J\x1b[H";
        static constexpr std::string_view show_cursor  = "\x1b[?25h";
        static constexpr std::string_view hide_cursor  = "\x1b[?25l";
    };

    struct interfaceChars {
        // Corners
        static constexpr std::string_view lt_corner {"┌"};
        static constexpr std::string_view lb_corner {"└"};
        static constexpr std::string_view rt_corner {"┐"};
        static constexpr std::string_view rb_corner {"┘"};

        // Bars
        static constexpr std::string_view h_bar {"─"};
        static constexpr std::string_view v_bar {"│"};

        // T-Junctions
        static constexpr std::string_view t_top    {"┬"};
        static constexpr std::string_view t_bottom {"┴"};
        static constexpr std::string_view t_left   {"├"};
        static constexpr std::string_view t_right  {"┤"};
        static constexpr std::string_view cross    {"┼"};
    };

    struct colors {
        static constexpr std::array<uint8_t, 3> white  {255,255,255};
        static constexpr std::array<uint8_t, 3> red    {255,0  ,0  };
        static constexpr std::array<uint8_t, 3> green  {0  ,255,0  };
        static constexpr std::array<uint8_t, 3> blue   {0  ,0  ,255};
    };

    struct termSize_t {
        int rows;
        int cols;
    };

    void init_runtime();
    termSize_t get_termSize();
}

#endif //RENDER_HPP