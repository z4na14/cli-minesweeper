# cli-minesweeper

## Architecture

When something fails and the program needs
to terminate, you can catch the error, print it
with the helper function `common::print_error("");`,
set `runtime::TERMINATE = true;` to terminate the
input thread and let the program exit.