#ifndef RENDER_HPP
#define RENDER_HPP

#include <string_view>
#include "common.hpp"

namespace render {
    // 32 Hz
    constexpr uint8_t FRAMERATE_PERIOD{1 / 64 * 100'000};

    struct controls {
        static constexpr std::string_view CLEAR_SCREEN = "\x1b[2J\x1b[H";
        static constexpr std::string_view CLEAR_LINE = "\x1b[2K";
        static constexpr std::string_view CLEAR_BUFFER = "\x1b[3J\x1b[H";
        static constexpr std::string_view SHOW_CURSOR = "\x1b[?25h";
        static constexpr std::string_view HIDE_CURSOR = "\x1b[?25l";
        static constexpr std::string_view ENABLE_MOUSE = "\033[?1000h";
        static constexpr std::string_view ENABLE_SGR1006 = "\033[?1006h";
        static constexpr std::string_view DISABLE_MOUSE = "\033[?1000l";
        static constexpr std::string_view DISABLE_SGR1006 = "\033[?1006l";
    };

    struct interfaceChars {
        // Corners
        static constexpr std::string_view LT_CORNER{"┌"};
        static constexpr std::string_view LB_CORNER{"└"};
        static constexpr std::string_view RT_CORNER{"┐"};
        static constexpr std::string_view RB_CORNER{"┘"};

        // Bars
        static constexpr std::string_view H_BAR{"─"};
        static constexpr std::string_view V_BAR{"│"};

        // T-Junctions
        static constexpr std::string_view T_TOP{"┬"};
        static constexpr std::string_view T_BOTTOM{"┴"};
        static constexpr std::string_view T_LEFT{"├"};
        static constexpr std::string_view T_RIGHT{"┤"};
        static constexpr std::string_view CROSS{"┼"};
    };

    struct colors {
        static constexpr std::array<uint8_t, 3> WHITE{255, 255, 255};
        static constexpr std::array<uint8_t, 3> RED{255, 0, 0};
        static constexpr std::array<uint8_t, 3> GREEN{0, 255, 0};
        static constexpr std::array<uint8_t, 3> BLUE{0, 0, 255};
    };

    struct termSize_t {
        int rows;
        int cols;

        friend bool operator==(const termSize_t &lhs, const termSize_t &rhs) {
            return lhs.rows == rhs.rows && lhs.cols == rhs.cols;
        }
    };

    void init_bootstrap();

    void loop_runtime();

    void cleanup_bootstrap();

    termSize_t get_termSize();

    void setRawMode(bool enable);

    // Threading function to get input
    void get_term_input_(std::string &buff);
}

#endif //RENDER_HPP
