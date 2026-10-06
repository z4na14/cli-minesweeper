#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

#include "render.hpp"


namespace render {
    termSize_t termSize;

    void init_runtime() {
        try {
            // Empty screen before doing anything
            std::cout << controls::clear_buffer <<
                         controls::clear_screen <<
                         controls::hide_cursor  << std::endl;

            sleep(3);

            termSize = get_termSize();
            if (termSize.cols < 70 || termSize.rows < 30)
                throw std::runtime_error(
                    "Terminal size is too small!\nConsider making it bigger");
        }

        catch (std::exception const& e) {
            const common::cell error {e.what(),
                {colors::red[0], colors::red[1], colors::red[2]}, {}};
            std::cerr << error << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    termSize_t get_termSize() {
        winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0)
            throw std::runtime_error("Unable to get term size");

        return {w.ws_row, w.ws_col};
    }
}
