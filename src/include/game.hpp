#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>

#include "common.hpp"
#include "render.hpp"


namespace game {
    class Table {
    public:
        Table(const int x, const int y) : size_x(x), size_y(y), real_index_x(x / 2), real_index_y(y),
                                          top_x(0), top_y(0),
                                          start_timer(std::chrono::steady_clock::now()),
                                          play_site({}) {
            // Reserve the play site size to avoid reallocations and out-of-bounds errors
            // Must be divisible by 2 to avoid strange allocation errors
            if (x%2!=0 or y%2!=0) throw std::invalid_argument("Precondition failed for x and y size");

            play_site.reserve(real_index_x * real_index_y);
            for (int i = 0; i < real_index_y; i++) {
                for (int j = 0; j < real_index_x; j++) {
                    play_site.push_back((common::cell){
                        " ", {render::colors::BLUE[0], render::colors::BLUE[1], render::colors::BLUE[2]}
                    });
                }
            }
        }

        virtual ~Table() = default;

        // ----------------------------------------------------------------------

        int size_x, size_y;
        // Each square will occupy 2 cells, as for keeping a more
        // square section for each block
        int real_index_x, real_index_y;
        int top_x, top_y;
        const std::chrono::time_point<std::chrono::steady_clock> start_timer;

        std::vector<common::cell> play_site;

        // ----------------------------------------------------------------------

        void print_table();

        void print_cell(int x, int y) const;

        int modify_cell(int x, int y, const std::string &val, common::color<Foreground> fg,
                        common::color<Background> bg);
    };


    // SIZE HAS TO BE DIVISIBLE BY 2
    class EasyGame : public Table {
    public:
        EasyGame() : Table(46, 18) {
        }
    };

    class MediumGame : public Table {
    public:
        MediumGame() : Table(56, 26) {
        }
    };

    class HardGame : public Table {
    public:
        HardGame() : Table(86, 32) {
        }
    };
}


#endif // GAME_HPP
