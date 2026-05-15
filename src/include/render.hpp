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
        // White Fades
        static constexpr std::array<uint8_t, 3> WHITE{255, 255, 255};
        static constexpr std::array<uint8_t, 3> WHITE_75{191, 191, 191};
        static constexpr std::array<uint8_t, 3> WHITE_50{127, 127, 127};
        static constexpr std::array<uint8_t, 3> WHITE_25{64, 64, 64};
        static constexpr std::array<uint8_t, 3> WHITE_10{25, 25, 25};

        // Red Fades
        static constexpr std::array<uint8_t, 3> RED{255, 0, 0};
        static constexpr std::array<uint8_t, 3> RED_75{191, 0, 0};
        static constexpr std::array<uint8_t, 3> RED_50{127, 0, 0};
        static constexpr std::array<uint8_t, 3> RED_25{64, 0, 0};
        static constexpr std::array<uint8_t, 3> RED_10{25, 0, 0};

        // Green Fades
        static constexpr std::array<uint8_t, 3> GREEN{0, 255, 0};
        static constexpr std::array<uint8_t, 3> GREEN_75{0, 191, 0};
        static constexpr std::array<uint8_t, 3> GREEN_50{0, 127, 0};
        static constexpr std::array<uint8_t, 3> GREEN_25{0, 64, 0};
        static constexpr std::array<uint8_t, 3> GREEN_10{0, 25, 0};

        // Blue Fades
        static constexpr std::array<uint8_t, 3> BLUE{0, 0, 255};
        static constexpr std::array<uint8_t, 3> BLUE_75{0, 0, 191};
        static constexpr std::array<uint8_t, 3> BLUE_50{0, 0, 127};
        static constexpr std::array<uint8_t, 3> BLUE_25{0, 0, 64};
        static constexpr std::array<uint8_t, 3> BLUE_10{0, 0, 25};
    };

    struct palette {
        static constexpr std::array<uint8_t, 3> MINES{220, 53, 69};
        static constexpr std::array<uint8_t, 3> FLAGS{255, 193, 7};
        static constexpr std::array<uint8_t, 3> TERRAIN{108, 117, 125};
        static constexpr std::array<uint8_t, 3> TEXT{230, 230, 230};

        static constexpr std::array<std::array<uint8_t, 3>, 8> MINES_COLOR_ARR {
            colors::BLUE,
            colors::GREEN_50,
            colors::RED,
            colors::BLUE_50,
            colors::RED_50,
            {0, 127, 127},
            {0, 0, 0},
            colors::WHITE_50
        };
    };

    enum class click_buttons {
        LEFT_BUTTON = 0,
        MIDDLE_BUTTON = 1,
        RIGHT_BUTTON = 2,
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
