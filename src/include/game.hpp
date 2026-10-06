#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>

namespace game {
    class Table {
    public:
        Table(const int x, const int y) : size_x(x), size_y(y), start_timer(std::chrono::steady_clock::now()) {
        }
        virtual ~Table() = default;

        int size_x, size_y;
        const std::chrono::time_point<std::chrono::steady_clock> start_timer;
    };


    class EasyGame : public Table {
    public:
        EasyGame() : Table(10, 8) {
        }
    };

    class MediumGame : public Table {
    public:
        MediumGame() : Table(15, 12) {
        }
    };

    class HardGame : public Table {
    public:
        HardGame() : Table(30, 25) {
        }
    };
}


#endif // GAME_HPP
