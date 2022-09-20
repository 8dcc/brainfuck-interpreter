#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // For tolower()
#include <ncurses.h>

#include "misc.h"
#include "interface.h"

int main() {
    initscr();      // Init ncurses

    // Declare and initialize as -1 the cell array for the memory simulation
    int* cell_arr = malloc(GRID_C * sizeof(int));
    memset(cell_arr, 0, GRID_C * sizeof(int));

    cell_arr[3] = 100;


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
        
        for (int n = 0; n < GRID_C; n++) {
            fill_cell(n, "ab");
        }
    } while (tolower(getchar()) != 'q');
    
    endwin();       // End ncurses window
    return 0;
}


