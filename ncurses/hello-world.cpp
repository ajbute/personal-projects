#include <ncurses.h>

int main() {
    // Intitializes ncurses
    initscr();
    // Prints to the terminal
    printw("Hello World");
    // Updates terminal
    refresh();
    // Waits for the user to type a character
    getch();

    // Cleans up ncurses
    endwin();
    return 0;
}