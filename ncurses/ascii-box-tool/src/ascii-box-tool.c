// Have to define _XOPEN_SOURCE to 700 in order to use the ncurses wide library functions
#define _XOPEN_SOURCE 700
// Including the ncurses wide header
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdlib.h>

/*
    This is a simple program that uses the ncurses wide library to write an ascii box to a text file. User input is taken for the length and width of the box, then those
    numbers are error checked to make sure they are valid dimensions, and the box is drawn in the text file. 
    (unit is characters, so a 3x3 box would be: ┌─┐ )
                                                │ │ )
                                                └─┘ )
*/

// Defining the different unicode characters to draw the box
const wchar_t HORIZONTAL_LINE_UNICODE = L'\u2500';
const wchar_t VERTICAL_LINE_UNICODE = L'\u2502';
const wchar_t TOP_LEFT_CORNER_UNICODE = L'\u250C';
const wchar_t TOP_RIGHT_CORNER_UNICODE = L'\u2510';
const wchar_t BOTTOM_LEFT_CORNER_UNICODE = L'\u2514';
const wchar_t BOTTOM_RIGHT_CORNER_UNICODE = L'\u2518';

// Maximum number of characters that the user can enter
const int MAX_NUM_OF_CHARS = 3;
// Minimum length of the box (Since 2 corners have to be used, and 1 line has to be used ┌ )
//                                                                                       │ )
//                                                                                       └ )
const int MINIMUM_LENGTH = 3;
// Minimum width of the box (Since 2 corners have to be used, and 1 line has to be used ┌ ─ ┐)
const int MINIMUM_WIDTH = 3;

bool get_length_width(int* local_length, int* local_width);
bool str_only_contains_digits(const char* str);
bool write_box_to_file(const char* path_to_file, int local_length, int local_width);
void exit_with_error(const char* error);

int main() {
    // Allows the program to correctly interpret and display characters based on the locale settings of the system or user's environment
    setlocale(LC_ALL, "");
    // Starts ncurses
    initscr();
    // Updating display
    refresh();

    int length, width;
    // Trys to get user input for length and width of the box
    // If an invalid number is entered, exits with error message
    if (!get_length_width(&length, &width)) {
        exit_with_error("Invalid number entered");
    }

    // Trys to draw a box with the passed length and width in a file
    // If the file can't be opened, exits with error message
    if (!write_box_to_file("../resources/frame.txt", length, width)) {
        // Exiting with error message
        exit_with_error("Failed to open file");
    }

    printw("Successfully wrote to the file\n(Press any key to exit)\n");
    // Waiting for user input before closing the program
    getch();

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

// Gets the length and width from the user, while making sure the input is a valid number
bool get_length_width(int* local_length, int* local_width) {
    // Strings to hold user input
    char str_length[MAX_NUM_OF_CHARS];
    char str_width[MAX_NUM_OF_CHARS];

    // Initializing strings with null characters
    // (Using a for loop because you can't initialize a variable sized array)
    for (int i = 0; i < MAX_NUM_OF_CHARS; i++) {
        str_length[i] = '\0';
        str_width[i] = '\0';
    }

    printw("Enter a Length: ");
    // Getting length from user (if input == "1" then the array == {'1', '\0', '\0'}, if input == "123" then array == {'1', '2', '3'})
    getnstr(str_length, MAX_NUM_OF_CHARS);
    // Returns false if string doesn't only contain digits
    if (!str_only_contains_digits(str_length)) {
        return false;
    }
    // Storing all the digits in an int
    sscanf(str_length, "%d", local_length);

    // Returns false if length is less than the minimum
    if (*local_length < MINIMUM_LENGTH) {
        return false;
    }

    printw("Enter a Width: ");
    // Getting width from user
    getnstr(str_width, MAX_NUM_OF_CHARS);
    // Returns false if string doesn't only contain digits
    if (!str_only_contains_digits(str_width)) {
        return false;
    }
    // Storing all the digits in an int
    sscanf(str_width, "%d", local_width);

    // Returns false if width is less than the minimum
    if (*local_width < MINIMUM_WIDTH) {
        return false;
    }

    return true;
}

// Returns true if passed string only contains digits; otherwise returns false
bool str_only_contains_digits(const char* str) {
    // Iterates through each character of the string
    for (int i = 0; i < MAX_NUM_OF_CHARS; i++) {
        // If there are no more user inputted characters return true
        if (str[i] == '\0') {
            return true;
        }
        // If the character is outside of the ascii range 48-57 (digits 0-9) return false
        else if (str[i] < 48 || str[i] > 57) {
            return false;
        }
    }
    return true;
}

// Draws an ascii box of passed length and width with unicode box drawing characters to a passed text file
bool write_box_to_file(const char* path_to_file, int length, int width) {
    FILE* src;

    // Returns false if it can't open the file
    if ((src = fopen(path_to_file, "w")) == NULL) {
        // Closing the file
        fclose(src);
        return false;
    }

    // Adding the top row of the box
    fputwc(TOP_LEFT_CORNER_UNICODE, src);
    for (int i = 0; i < length - 2; i++) {
        fputwc(HORIZONTAL_LINE_UNICODE, src);
    }
    fputwc(TOP_RIGHT_CORNER_UNICODE, src);

    // Adding a newline
    fwprintf(src, L"\n");
    // Adding all of the vertical lines to the box
    for (int i = 0; i < width - 2; i++) {
        fputwc(VERTICAL_LINE_UNICODE, src);
        // Adds all of the spaces between each vertical line
        for (int j = 0; j < length - 2; j++) {
            // Adding a space
            fputwc(L' ', src);
        }
        fputwc(VERTICAL_LINE_UNICODE, src);
        // Adding a newline
        fwprintf(src, L"\n");
    }

    // Adding the bottom row of the box
    fputwc(BOTTOM_LEFT_CORNER_UNICODE, src);
    for (int i = 0; i < length - 2; i++) {
        fputwc(HORIZONTAL_LINE_UNICODE, src);
    }
    fputwc(BOTTOM_RIGHT_CORNER_UNICODE, src);

    // Closing the file
    fclose(src);
    return true;
}

void exit_with_error(const char* error) {
    // Printing error to the terminal
    printw("Error: %s\n(Press any key to exit)\n", error);
    // Waiting for user input before closing the program
    getch();
    // Cleaning up ncurses
    endwin();
    // Exiting with EXIT_FAILURE exit code
    exit(EXIT_FAILURE);
}