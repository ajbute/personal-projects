#include <ncurses.h>

/*
    This is a simple program that uses the ncurses library to create a window in the terminal and allow the user to move the cursor within the window
    with the arrow keys. In ncurses the terminal is categorized into different "windows", which are just 2d arrays of characters. By default a window
    called "stdscr", which is the size of the terminal screen, is displayed. ncurses provides the ability to create new windows seperate from the
    stdscr, allowing you to display different characters in each of them. Each window is comprised of a y and x axis of cells. A cell is just a region
    of space that can display one character. The origin of every window, (0, 0), is at the top left corner of the window. So for example if you were to
    create a window of size (30, 30), it would be able to store 30 characters along the x axis and 30 characters along the y axis. (**NOTE** ncurses uses
    a (y, x) format rather than an (x, y) format for all of its functions). In this program a rectangular window is created in the center of the terminal
    and given a border to visualize the bounds of the window. User input is then taken, and if any of the arrow keys are pressed, the corresponding movement
    is done with the cursor. (So if the user presses the left arrow key, the cursor will move to the left respectively). ncurses handles checking whether or
    not the cursor movement will be outside of the bounds of the window, however the program checks to make sure the cursor movement will stay in the bounds
    of the windows border. This is done because even though a window may be of size (3, 3), the characters that make up the border of the window are within
    the window itself. So for example this is what the window would look like:
    ┌─┐
    │x│
    └─┘
    Because the window is only 3 cells wide and 3 cells tall, if a border is drawn on the window, it leaves only one empty cell (Visualized by the 'x').
    Here are the different coordinates of the window, and what character each cell stores (The 'x' represents an empty cell):
    (0,0) = '┌' (0,1) = '─' (0,2) = '┐'
    (1,0) = '│' (1,1) = 'x' (1,2) = '│'
    (2,0) = '└' (2,1) = '─' (2,2) = '┘'
    
    If the program didn't account for this, the cursor would be able to move on the border of the window, and it wouldn't look right.
    Another thing the program checks for is whether or not the terminal is a certain size. This is done so that the terminal isn't resized to a size smaller
    than the window itself, which wouldn't display the window at all.

    (One last thing to note is that LINES and COLS are 2 global variables that are included with ncurses. They are the current height (LINES) and width (COLS) in cells of the terminal; 
    They are automatically set when initscr() is called or the terminal is resized and getch() returns KEY_RESIZE)
*/

WINDOW *create_newwin(int height, int width, int start_y, int start_x);
void destroy_win(WINDOW *local_win);
void print_values(int local_win_starty, int local_win_startx, int local_cursor_y, int local_cursor_x);
void print_resize_message();
WINDOW *handle_resize(WINDOW *passed_win, int local_cursor_y, int local_cursor_x);

// Defining Constants

// The height and width of the window; Unit is cells;
const int WIN_HEIGHT = 33;    
const int WIN_WIDTH = 88;

// The minimum amount of cells on the x and y axis of the terminal needed to display the window properly
const int TERMINAL_MIN_LINES = 47;
const int TERMINAL_MIN_COLS = 120;

// Initial cursor x and y position when the window is created, (1, 1); Unit is cells
const int INITIAL_CURSOR_X = 1;
const int INITIAL_CURSOR_Y = 1;

int main() {
    // Starts ncurses
    initscr();
    // Line buffering disabled, retrieves input from user instantly without pressing enter
    cbreak();
    // Doesn't display what user inputs
    noecho();
    // Allows getting the input from the keyboard
    keypad(stdscr, TRUE);
    // Updating display
    refresh();
    // Declaring variables
    WINDOW *my_win;
    int ch, cursor_x, cursor_y;
    bool has_cursor_moved = false;

    // The start x and y position of the window relative to the origin (0, 0) which is the top left corner of the terminal; Unit is cells
    int win_starty = (LINES - WIN_HEIGHT) / 2;
    int win_startx = (COLS - WIN_WIDTH) / 2;

    // Printing useful information to the top of the terminal
    print_values(win_starty, win_startx, INITIAL_CURSOR_Y, INITIAL_CURSOR_X);
    // Creating a new window
    my_win = create_newwin(WIN_HEIGHT, WIN_WIDTH, win_starty, win_startx);

    // LINES and COLS are the current height and width in cells of the terminal; 
    // They are automatically set when initscr() is called or the terminal is resized and getch() returns KEY_RESIZE
    // Checks if the terminal size is greater than the minimum size
    if (LINES < TERMINAL_MIN_LINES || COLS < TERMINAL_MIN_COLS) {
        // Prints an error message telling the user to resize the terminal
        print_resize_message();
        // Runs while the terminal is smaller than the minimum size
        while (LINES < TERMINAL_MIN_LINES || COLS < TERMINAL_MIN_COLS) {
            ch = getch();
            switch(ch) {
                 // Displays updated terminal size when its resized
                case KEY_RESIZE:
                    move(0, 0);
                    printw("Lines: %i\n", LINES);
                    printw("Cols: %i\n", COLS); 
                break;
                // Ends the program when F1 is pressed
                case KEY_F(1):
                    // Destroying the window pointer and erasing the box
                    destroy_win(my_win);
                    // Cleaning up ncurses
                    endwin();
                    return 0;
                break;

            }
        }
        // Terminal is the correct size and the window can be updated
        my_win = handle_resize(my_win, INITIAL_CURSOR_Y, INITIAL_CURSOR_X);
    }
    // Move cursor to the initial cursor position; in this case the top left of the newly created window
    wmove(my_win, INITIAL_CURSOR_X, INITIAL_CURSOR_Y);
    // Updating window
    wrefresh(my_win);

    // Getting user input 
    while ((ch = getch()) != KEY_F(1)) {
        // Gets the current x and y position of the cursor; Unit is cells
        getyx(my_win, cursor_y, cursor_x);
        
        switch (ch) {
            // Left arrow key is pressed
            case KEY_LEFT:
                if (cursor_x - 1 != 0) {
                    cursor_x--;
                    has_cursor_moved = true;
                }
            break;
            // Right arrow key is pressed
            case KEY_RIGHT:
                if (cursor_x + 1 != WIN_WIDTH - 1) {
                    cursor_x++;
                    has_cursor_moved = true;
                }
            break;
            // Up arrow key is pressed
            case KEY_UP:
                if (cursor_y - 1 != 0) {
                    cursor_y--;
                    has_cursor_moved = true;
                }
            break;
            // Down arrow key is pressed
            case KEY_DOWN:
                if (cursor_y + 1 != WIN_HEIGHT - 1) {
                    cursor_y++;
                    has_cursor_moved = true;
                }
            break;
            // Terminal is resized
            case KEY_RESIZE:
                has_cursor_moved = false;
                // Checks if the terminal size is greater than the minimum size
                if (LINES < TERMINAL_MIN_LINES || COLS < TERMINAL_MIN_COLS) {
                    // Prints an error message telling the user to resize the terminal
                    print_resize_message();
                    continue;
                }
                else {
                    my_win = handle_resize(my_win, cursor_y, cursor_x);
                }
            break;
            default:
                has_cursor_moved = false;
            break;
        }

        // Only updates the current cursor y, x position when it changes 
        if (has_cursor_moved) {
            // Moves cursor to the line "Cursor Y: " is on
            move(5, 0);
            // Clears the current line the cursor is on
            clrtoeol();
            // Updates the cursor y value
            printw("Cursor Y: %i\n", cursor_y);
            // Clears the current line the cursor is on
            clrtoeol();
            // Updates the cursor x value
            printw("Cursor X: %i\n", cursor_x);
            // Updating screen
            refresh();
            // Setting to false for the next loop
            has_cursor_moved = false;
            // Moves cursor back to the window
            wmove(my_win, cursor_y, cursor_x);
            // Updating window
            wrefresh(my_win);
        }
    }

    // Destroying the window pointer and erasing the box
    destroy_win(my_win);
    // Cleaning up ncurses
    endwin();
    return 0;
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

void print_values(int local_win_starty, int local_win_startx, int local_cursor_y, int local_cursor_x) {
    // Moves cursor to the top left of the terminal
    move(0, 0);
    // We don't need to call clear() because we're printing over the text thats already there 
    // (Also when print_values() is called at the beginning of the program, clear() doesn't need to be called)
    printw("Lines: %i\n", LINES);
    printw("Cols: %i\n", COLS);
    printw("Starty: %i\n", local_win_starty);
    printw("Startx: %i\n\n", local_win_startx);
    printw("Cursor Y: %i\n", local_cursor_y);
    printw("Cursor X: %i\n\n", local_cursor_x);
    printw("Press F1 to exit");
    refresh();
}

void print_resize_message() {
    clear();
    printw("Lines: %i\n", LINES);
    printw("Cols: %i\n", COLS); 
    printw("(Minimum Lines: %i)\n", TERMINAL_MIN_LINES);
    printw("(Minimum Cols: %i)\n", TERMINAL_MIN_COLS);
    printw("Error: Terminal too small. Please resize. (Or press F1 to exit)\n");
    refresh();
}

WINDOW *handle_resize(WINDOW *passed_win, int local_cursor_y, int local_cursor_x) {
    WINDOW *local_win;
    // Calculating the new win start y, x
    int local_win_starty = (LINES - WIN_HEIGHT) / 2;
    int local_win_startx = (COLS - WIN_WIDTH) / 2;

    // Displaying the current values
    print_values(local_win_starty, local_win_startx, local_cursor_y, local_cursor_x);

    // Destroying the current window
    destroy_win(passed_win);
    refresh();

    // Creating a new window
    local_win = create_newwin(WIN_HEIGHT, WIN_WIDTH, local_win_starty, local_win_startx);
    // Move cursor to the passed cursor position
    wmove(local_win, local_cursor_y, local_cursor_x);
    wrefresh(local_win);

    return local_win;
}