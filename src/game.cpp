#include <format>
#include <random>
#include <algorithm>
#include <numeric>

#include "render.hpp"
#include "common.hpp"
#include "runtime.hpp"


namespace game {
    void Table::print_table() {
        // +2 to account for the borders on both sides
        const int total_width = size_x + 2;
        const int total_height = size_y + 2;
        // /2 to center that boi
        top_x = (render::TERM_SIZE.cols - total_width) / 2;
        top_y = (render::TERM_SIZE.rows - total_height) / 2;

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
            for (int j = 0; j < size_x; j += 2) {
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
        auto lobotomized_cell = current_cell;

        // Remove the value from the first cell to be printed
        lobotomized_cell.val = " ";

        std::cout << lobotomized_cell << current_cell;
    }

    int Table::modify_cell(const int x, const int y, const std::string &val,
                           const common::color<Foreground> fg, const common::color<Background> bg) {
        if (x < 0 || x >= size_x ||
            y < 0 || y >= size_y) {
            return -1;
        }
        auto& cell {retrieve_cell(x, y)};
        cell.val = val;
        cell.fg = fg;
        cell.bg = bg;

        print_table();
        return 0;
    }

    common::cell &Table::retrieve_cell(const int x, const int y) {
        if (x < 0 || x >= size_x ||
            y < 0 || y >= size_y) {
            static common::cell return_cell(" ");
            return return_cell;
        }

        // Same shit as in print_cell
        const auto transformed_x = static_cast<int>(x / 2);
        return play_site[(y) * real_index_x + transformed_x];
    }

    void Table::send_left_click(const int x, const int y) {
        switch (runtime::GAME_STATUS) {
          /*case runtime::SELECTION:
              break;*/

            case runtime::GAME:
                auto &current_cell {retrieve_cell(x, y)};

                if (current_cell.type == CellType::Bomb)
                    modify_cell(x, y, " ", {},
                                {render::colors::RED_50[0],
                                render::colors::RED_50[1],
                                render::colors::RED_50[2]});

                else if (!current_cell.cleared && !current_cell.flagged) {
                    const std::uint8_t nearby_mines{check_nearby_mines(x, y)};
                    modify_cell(
                        x, y, std::to_string(nearby_mines),
                        {render::palette::MINES_COLOR_ARR[nearby_mines][0],
                         render::palette::MINES_COLOR_ARR[nearby_mines][1],
                         render::palette::MINES_COLOR_ARR[nearby_mines][2]},
                        {render::palette::TERRAIN[0],
                         render::palette::TERRAIN[1],
                         render::palette::TERRAIN[2]});
                    current_cell.cleared = true;

                    if (nearby_mines == 0) clear_nearby_empty_cells(x, y);
                }

                break;

            /*case runtime::END:
                break;*/

            /*default:
                common::print_error("Internal error: Exiting");
                runtime::TERMINATE = true;*/
        }
    }

    void Table::send_right_click(const int x, const int y) {
        auto &current_cell{retrieve_cell(x, y)};

        if (current_cell.cleared) {
            return;
        } else if (current_cell.flagged) {
            modify_cell(x, y, " ", {},
                        {render::palette::TERRAIN[0],
                        render::palette::TERRAIN[1],
                        render::palette::TERRAIN[2]});
        } else {
            modify_cell(x, y, "⚑",
                        {render::palette::FLAGS[0],
                        render::palette::FLAGS[1],
                        render::palette::FLAGS[2]},
                        {render::palette::TERRAIN[0],
                        render::palette::TERRAIN[1],
                        render::palette::TERRAIN[2]});
        }

        current_cell.flagged = !current_cell.flagged;
    }

    void Table::distribute_mines(const int mine_count) {
        // Validate mine count
        const int max_cells = real_index_x * real_index_y;
        if (mine_count < 0 || mine_count > max_cells) {
            common::print_error("Invalid mine count");
            return;
        }

        // Create a vector of all possible cell indices
        std::vector<int> indices(max_cells);
        std::iota(indices.begin(), indices.end(), 0);

        // Shuffle the indices to randomize mine positions
        std::random_device rd;
        std::mt19937 gen(rd());
        std::ranges::shuffle(indices, gen);

        // Place mines in the first mine_count cells
        for (int i = 0; i < mine_count; ++i) {
            play_site[indices[i]].type = CellType::Bomb;
        }
    }

std::uint8_t Table::check_nearby_mines(const int x, const int y) {
        if (x < 0 || x >= size_x ||
            y < 0 || y >= size_y) {
            return -1;
        }

        std::uint8_t mine_count = 0;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                // Aplicar el offset, ya que son 2 minas por espacio
                int new_x = x + (dx * 2);
                int new_y = y + dy;

                if (new_x < 0 || new_x >= size_x ||
                    new_y < 0 || new_y >= size_y ||
                    (dx == 0 && dy == 0)) {
                    continue;
                }
                mine_count += retrieve_cell(new_x, new_y).type == CellType::Bomb;
            }
        }

        return mine_count;
    }

    void Table::clear_nearby_empty_cells(const int x, const int y) {
        if (x < 0 || x >= size_x ||
            y < 0 || y >= size_y) {
            return;
        }

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                // Aplicar el offset, ya que son 2 minas por espacio
                int new_x = x + (dx * 2);
                int new_y = y + dy;

                if (new_x < 0 || new_x >= size_x ||
                    new_y < 0 || new_y >= size_y ||
                    (dx == 0 && dy == 0)) {
                    continue;
                }

                auto& current_cell = retrieve_cell(new_x, new_y);
                if (current_cell.cleared) continue;

                const auto nearby_mines = check_nearby_mines(new_x, new_y);
                modify_cell(new_x, new_y, std::to_string(nearby_mines),
                            {render::palette::MINES_COLOR_ARR[nearby_mines][0],
                             render::palette::MINES_COLOR_ARR[nearby_mines][1],
                             render::palette::MINES_COLOR_ARR[nearby_mines][2]},
                            {render::palette::TERRAIN[0],
                             render::palette::TERRAIN[1],
                             render::palette::TERRAIN[2]});
                current_cell.cleared = true;

                if (nearby_mines == 0) clear_nearby_empty_cells(new_x, new_y);
            }
        }
    }
}
