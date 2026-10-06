#include "render.hpp"
#include "common.hpp"
#include "runtime.hpp"

namespace game {
    void Table::print_table() {
        const int total_width = size_x + 2;
        const int total_height = size_y + 2;

        top_x = (render::termSize.cols - total_width) / 2;
        top_y = (render::termSize.rows - total_height) / 2;

        // Top border
        render::move_cursor(top_x, top_y);
        std::cout << render::interfaceChars::D_LT_CORNER;
        for (int j = 0; j < size_x; ++j) {
            std::cout << render::interfaceChars::D_H_BAR;
        }
        std::cout << render::interfaceChars::D_RT_CORNER;

        for (int i = 1; i <= size_y; ++i) {
            // Left border
            render::move_cursor(top_x, top_y + i);
            std::cout << render::interfaceChars::D_V_BAR;

            // Print blocks
            for (int j = 0; j < size_x; ++j) {
                // -1 Because we begin right after top_y, but index overflows
                // in the vector
                print_cell(j, i - 1);
            }

            // Right border
            render::move_cursor(top_x + size_x + 1, top_y + i);
            std::cout << render::interfaceChars::D_V_BAR;
        }

        // Bottom border
        render::move_cursor(top_x, top_y + size_y + 1);
        std::cout << render::interfaceChars::D_LB_CORNER;
        for (int j = 0; j < size_x; ++j) {
            std::cout << render::interfaceChars::D_H_BAR;
        }
        std::cout << render::interfaceChars::D_RB_CORNER;
    }

    void Table::print_cell(const int x, const int y) const {
        std::cout << play_site[(y) * size_x + x];
    }

    int Table::modify_cell(const int x, const int y, const std::string &val,
                           const common::color<Foreground> fg, const common::color<Background> bg) {

        if (x < 0 || x >= size_x ||
            y < 0 || y >= size_y) {
            return -1;
        }


        play_site[y * size_x + x] = (common::cell){val, fg, bg};
        print_table();

        return 0;
    }
}
