#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>      // For tolower()
#include <ncurses.h>

#include "misc.h"
#include "interface.h"

int main() {
    initscr();      // Init ncurses

    // Declare and initialize as -1 the cell array for the memory simulation
    int* cell_arr = calloc(GRID_C, sizeof(int));

    for (int n = 0; n < GRID_C; n++) {
        cell_arr[n] = 100 + n;
    }

    mvprintw(1, GRID_X, "Press 'q' to exit...");

    // Loop while we are not pressing 'q'
    do {
        // First set GRID_CW to the max str len
        for (int n = 0; n < GRID_C; n++) {
            int digits = get_digits(cell_arr[n]);
            if (digits > GRID_CW)
                GRID_CW = digits;
        }

        draw_grid();
        
        char* buff= calloc(255, sizeof(char));
        for (int n = 0; n < GRID_C; n++) {
            sprintf(buff, "%d", cell_arr[n]);
            fill_cell(n, buff);
        }
    } while (tolower(getchar()) != 'q');
    
    endwin();       // End ncurses window
    return 0;
}

