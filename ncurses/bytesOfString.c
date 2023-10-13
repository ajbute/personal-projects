#include <ncurses.h>
#include <stdio.h>

int get_str_length(char str[]) {
    int count;
    for (count = 0; str[count] != '\0'; ++count);
    return count;
}

const int MAX_CHAR_LIMIT = 10;

int main() {
    initscr();
    printw("Enter a string: ");
    char user_str[MAX_CHAR_LIMIT];
    // getnstr will beep and not allow the user to type any more characters than the passed limit
    getnstr(user_str, MAX_CHAR_LIMIT);
    printw("'%s' is %i byte(s).", user_str, get_str_length(user_str));
    getch();

    endwin();
    return 0;
}