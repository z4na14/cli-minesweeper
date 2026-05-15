#ifndef RENDER_HPP
#define RENDER_HPP

#include <array>
#include <string_view>
#include "common.hpp"

namespace render {
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

        // Corners (Double Line)
        static constexpr std::string_view D_LT_CORNER{"╔"};
        static constexpr std::string_view D_LB_CORNER{"╚"};
        static constexpr std::string_view D_RT_CORNER{"╗"};
        static constexpr std::string_view D_RB_CORNER{"╝"};

        // Bars (Double Line)
        static constexpr std::string_view D_H_BAR{"═"};
        static constexpr std::string_view D_V_BAR{"║"};

        // T-Junctions
        static constexpr std::string_view T_TOP{"┬"};
        static constexpr std::string_view T_BOTTOM{"┴"};
        static constexpr std::string_view T_LEFT{"├"};
        static constexpr std::string_view T_RIGHT{"┤"};
        static constexpr std::string_view CROSS{"┼"};
    };

struct colors {
        static constexpr std::array<uint8_t, 3> WHITE{248, 250, 252};
        static constexpr std::array<uint8_t, 3> WHITE_75{203, 213, 225};
        static constexpr std::array<uint8_t, 3> WHITE_50{148, 163, 184};
        static constexpr std::array<uint8_t, 3> WHITE_25{71, 85, 105};
        static constexpr std::array<uint8_t, 3> WHITE_10{15, 23, 42};

        static constexpr std::array<uint8_t, 3> RED{244, 63, 94};
        static constexpr std::array<uint8_t, 3> RED_75{225, 29, 72};
        static constexpr std::array<uint8_t, 3> RED_50{190, 18, 60};
        static constexpr std::array<uint8_t, 3> RED_25{159, 18, 57};
        static constexpr std::array<uint8_t, 3> RED_10{76, 5, 25};

        static constexpr std::array<uint8_t, 3> GREEN{16, 185, 129};
        static constexpr std::array<uint8_t, 3> GREEN_75{5, 150, 105};
        static constexpr std::array<uint8_t, 3> GREEN_50{4, 120, 87};
        static constexpr std::array<uint8_t, 3> GREEN_25{6, 95, 70};
        static constexpr std::array<uint8_t, 3> GREEN_10{2, 44, 34};

        static constexpr std::array<uint8_t, 3> BLUE{59, 130, 246};
        static constexpr std::array<uint8_t, 3> BLUE_75{37, 99, 235};
        static constexpr std::array<uint8_t, 3> BLUE_50{29, 78, 216};
        static constexpr std::array<uint8_t, 3> BLUE_25{30, 64, 175};
        static constexpr std::array<uint8_t, 3> BLUE_10{23, 37, 84};
    };

    struct palette {
        // Core game elements
        static constexpr std::array<uint8_t, 3> MINES{239, 68, 68};
        static constexpr std::array<uint8_t, 3> FLAGS{245, 158, 11};
        static constexpr std::array<uint8_t, 3> TERRAIN{51, 65, 85};
        static constexpr std::array<uint8_t, 3> TEXT{241, 245, 249};

        static constexpr std::array<std::array<uint8_t, 3>, 8> MINES_COLOR_ARR {
            colors::BLUE,
            colors::GREEN,
            colors::RED,
            {139, 92, 246},
            {249, 115, 22},
            {6, 182, 212},
            colors::WHITE_10,
            colors::WHITE_50
        };
    };

    enum class click_buttons {
        LEFT_BUTTON = 0,
        RIGHT_BUTTON = 1,
        MIDDLE_BUTTON = 2,
    };

    struct termSize_t {
        int rows;
        int cols;

        friend bool operator==(const termSize_t &lhs, const termSize_t &rhs) {
            return lhs.rows == rhs.rows && lhs.cols == rhs.cols;
        }
    };

    extern termSize_t TERM_SIZE;
    extern int PRESSED_X, PRESSED_Y;
    extern click_buttons PRESSED_BUTTON;
    // 32 Hz
    constexpr uint8_t FRAMERATE_PERIOD{1 / 64 * 100'000};

    // ----------------------------------------------------------------------

    termSize_t get_term_size();

    void move_cursor(int x, int y);

    void set_raw_mode(bool enable);

    // Threading function to get input
    void get_term_input();
}

#endif //RENDER_HPP
