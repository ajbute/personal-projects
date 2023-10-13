// Have to define _XOPEN_SOURCE to 700 in order to use the ncurses wide library functions
#define _XOPEN_SOURCE 700
// Including the ncurses wide header
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

/*
    This is a simple program that uses the ncurses wide library to create a window and print the contents of a text file (in this case frame.txt) 
    inside of the window. One of the biggest features of the program is being UTF-8 compatible, meaning the text file can contain any UTF-8 characters, 
    and the code will print them properly. The program prints the text file by iterating through each character of the file and printing them as wide 
    characters, until the end of the file is reached, or the cursor is at the bottom of the window's border. The program also takes into account that frame.txt 
    may contain more characters than can be printed inside of the window, and handles this case by simply only printing the maximum amount of characters that 
    can fit inside of the window, and ignoring the rest.
*/

bool print_file_in_window(const char* filename, WINDOW* local_win);
WINDOW *create_newwin(int height, int width, int start_y, int start_x);
void destroy_win(WINDOW *local_win);
void print_error_message(const char* error_message, const char* file, int line);

// Defining constants

// The height and width of the window; Unit is cells;
const int WIN_HEIGHT = 33;    
const int WIN_WIDTH = 88;

int main() {
    // Allows the program to correctly interpret and display characters based on the locale settings of the system or user's environment
    setlocale(LC_ALL, "");
    // Starts ncurses
    initscr();
    // Line buffering disabled, retrieves input from user instantly without pressing enter
    cbreak();
    // Doesn't display what user inputs
    noecho();
    // Updating display
    refresh();

    // Declaring variables
    WINDOW* display_port;

    // The start x and y position of the window relative to the origin (0, 0) which is the top left corner of the terminal; Unit is cells
    int win_starty = (LINES - WIN_HEIGHT) / 2;
    int win_startx = (COLS - WIN_WIDTH) / 2;

    // Creating a new window
    display_port = create_newwin(WIN_HEIGHT, WIN_WIDTH, win_starty, win_startx);

    // If print_file fails, exit the program with EXIT_FAILURE flag
    if (!print_file_in_window("../resources/frame.txt", display_port)) {
        // Waiting for user input before closing the program
        getch();
        // Destroying the window
        destroy_win(display_port);
        // Cleaning up ncurses
        endwin();
        // Exiting with EXIT_FAILURE exit code
        exit(EXIT_FAILURE);
    }

    // Waiting for user input before closing the program
    getch();

    // Destroying the window
    destroy_win(display_port);
    // Cleaning up ncurses
    endwin();

    /*
        "Returning from the the main function, either by a return statement or by reaching the end of the function, 
        executes exit(), passing the argument of the return statement (or ​0​ if implicit return was used) as exit_code."
        https://en.cppreference.com/w/c/program/exit
    */
    // Returning the EXIT_SUCCESS exit code
    return EXIT_SUCCESS;
}

bool print_file_in_window(const char* filename, WINDOW* local_win) {
	FILE* src;
    // Using wint_t rather than wchar_t to account for special returns from fgetwc() such as WEOF
    wint_t wc;
    int cursor_y, cursor_x;
    // Max y and x value of the window
    int max_y = WIN_HEIGHT - 1;
    int max_x = WIN_WIDTH - 1;

    // Attempts to open file and returns false if it can't
    if ((src = fopen(filename, "r")) == NULL) {
        // Prints error message with file name and the line that fopen() failed
        print_error_message("fopen() failed", __FILE__, __LINE__ - 1);
        // Closing file
        fclose(src);
        return false;
    }
    // Moving cursor to top left of the window
    wmove(local_win, 1, 1);

    // Initializing errno to 0 in case there's an error
    errno = 0;
    // Getting every wide character from the file until end of file is reached
    while ((wc = fgetwc(src)) != WEOF) {
        // Get the current cursor position
        getyx(local_win, cursor_y, cursor_x);
        // Cursor is on the window's bottom border
        if (cursor_y >= max_y) {
            // Stop printing
            break;
        }

        // Current character is new line
        if (wc == L'\n') {
            // Manually moving the cursor one line down because printing a new line messes up the windows border
            wmove(local_win, ++cursor_y, 1);
            continue;
        }
        // Ignoring carriage returns because wadd_wch clears to the end of the line
        // Also only printing if cursor is not on the windows right border edge
        else if (wc != L'\r' && cursor_x < max_x) {
            // wadd_wch() takes a cchar_t
            cchar_t wchar;
            // Setting wchar to wc, the current wide character from the file
            setcchar(&wchar, &wc, 0, 0, NULL);
            // Printing the current character from the file
            wadd_wch(local_win, &wchar);

            // wadd_wch() moves the cursor over one so we check if it is on the windows right border edge
            if (cursor_x >= max_x) {
                // Manually moving the cursor one line down because printing a new line messes up the windows border
                wmove(local_win, ++cursor_y, 1);
                // Cursor is on the window's bottom border
                if (cursor_y >= max_y) {
                    // Stop printing
                    break;
                }
            }
        }
    }
    // EILSEQ = Code for error in a multibyte or wide character sequence
    // Printing error message if errno is set to EILSEQ
    if (errno == EILSEQ) {
        print_error_message("Invalid or incomplete multibyte or wide character encountered", __FILE__, 86);
        // Closing file
        fclose(src);
        return false;
    }
    // Updating window
    wrefresh(local_win);
    // Closing file
    fclose(src);
    return true;
}

WINDOW *create_newwin(int height, int width, int start_y, int start_x) {
    WINDOW* local_win;

    // Creates a new pointer to a window
    local_win = newwin(height, width, start_y, start_x);
    // Creates a box in the window with 0,0 for default vertical and horizontal lines
    box(local_win, 0, 0);
    //wborder(local_win, 0, 0, 0, 0, '#', '#', '#', '#');
    // Updating window
    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW *local_win) {
    // Setting the box and border to empty characters to make the window disappear
    box(local_win, ' ', ' ');
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
    // Updating window
    wrefresh(local_win);
    // Deleting the window
    delwin(local_win);
}

void print_error_message(const char* error_message, const char* file, int line) {
    // Clears the terminal
    clear();
    // Moving cursor to the top left of the terminal
    move(0, 0);
    // Printing error message
    printw("Error: %s in file %s at line # %d\n", error_message, file, line);
    printw("(Press any key to exit)\n");
    refresh();
}