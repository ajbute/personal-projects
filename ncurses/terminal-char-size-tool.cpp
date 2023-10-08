#include <ncurses.h>

int main() {
    // Starts ncurses
    initscr();

    // LINES and COLS are the current height and width in characters of the terminal; The origin is the upper left corner (0, 0)
    // They are automatically set when initscr() is called or the terminal is resized and getch() returns KEY_RESIZE

    // The # is the origin (0, 0) of the terminal
    printw("#");
    // Adds a vertical bar for each column of the terminal
    for (int i = 0; i < COLS - 1; i++) {
        addch('|');
    }
    // Adds a dash for each line of the terminal
    for (int i = 0; i < LINES - 1; i++) {
        addch('-');
        addch('\n');
    }
    // Updating screen
    refresh();

    // Printing the amount of lines and columns
    mvaddstr(1, 2, "Lines: ");
    printw("%i\n", LINES);
    mvaddstr(2, 2, "Columns: ");
    printw("%i\n", COLS);
    // Waiting for a character from the user
    getch();

    // Cleaning up ncurses
    endwin();
    return 0;
}