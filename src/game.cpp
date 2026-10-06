#include "render.hpp"
#include "common.hpp"
#include "runtime.hpp"

namespace game {
    void Table::print_table() {
        // +2 to account for the borders on both sides
        const int total_width = size_x + 2;
        const int total_height = size_y + 2;
        // /2 to center that boi
        top_x = (render::termSize.cols - total_width) / 2;
        top_y = (render::termSize.rows - total_height) / 2;

        // Top border
        render::move_cursor(top_x, top_y);
        std::cout << render::interfaceChars::D_LT_CORNER;
        for (int j = 0; j < size_x; ++j) {
            std::cout << render::interfaceChars::D_H_BAR;
        }
        std::cout << render::interfaceChars::D_RT_CORNER;

        for (int i = 0; i < size_y; ++i) {
            // Left border
            render::move_cursor(top_x, top_y + 1 + i);
            std::cout << render::interfaceChars::D_V_BAR;

            // Print blocks
            for (int j = 0; j < size_x; j+=2) {
                print_cell(j, i);
            }

            // Right border
            std::cout << render::interfaceChars::D_V_BAR;
        }

        // Bottom border
        render::move_cursor(top_x, top_y + 1 + size_y);
        std::cout << render::interfaceChars::D_LB_CORNER;
        for (int j = 0; j < size_x; ++j) {
            std::cout << render::interfaceChars::D_H_BAR;
        }
        std::cout << render::interfaceChars::D_RB_CORNER;
    }

    void Table::print_cell(const int x, const int y) const {
        if (x < 0 || x > size_x ||
            y < 0 || y > size_y) {
            return;
        }

        // Apply conversion to print correctly the cells
        const auto transformed_x = static_cast<int>(x / 2);
        // A los arrays se accede sin condon
        const auto current_cell = play_site[(y) * real_index_x + transformed_x];

        std::cout << current_cell << current_cell;
    }

    int Table::modify_cell(const int x, const int y, const std::string &val,
                           const common::color<Foreground> fg, const common::color<Background> bg) {
        if (x < 0 || x >= size_x ||
            y < 0 || y >= size_y) {
            return -1;
        }

        // Same shit as in print_cell
        const auto transformed_x = static_cast<int>(x / 2);
        play_site[(y) * real_index_x + transformed_x] = (common::cell){val, fg, bg};
        print_table();

        return 0;
    }
}
