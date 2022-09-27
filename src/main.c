#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>      // For tolower()
#include <ncurses.h>

#include "misc.h"
#include "interface.h"

int main() {
    initscr();              // Init ncurses
    raw();                  // Scan input without pressing enter
    noecho();               // Don't print when typing
    keypad(stdscr, TRUE);   // Enable keypad (arrow keys)

    // Declare and initialize as 0 the cell array for the memory simulation
    int* cell_arr = malloc(GRID_C * sizeof(int));
    for (int n = 0; n < GRID_C; n++)
        cell_arr[n] = n;

    cell_arr[5] = 13370;
    cell_arr[130] = 1337;

    mvprintw(1, GRID_X, "Press 'q' to exit...");

    int cur_page  = 0;
    int max_pages = 0;      // Max pages of the grid. Updated each loop because the terminal size might change

    int c = 0;
    // Loop while we are not pressing 'q'
    do {
        max_pages = GRID_C/GRID_CPP;

        // First set GRID_CW to the max str len
        for (int n = 0; n < GRID_C; n++) {
            int digits = get_digits(cell_arr[n]);
            if (digits > GRID_CW)
                GRID_CW = digits;
        }

        draw_grid();
        fill_grid(cell_arr, cur_page);

        // Get the pressed key after drawing everything
        c = getch();
        switch (c) {
            case 261:       // Right arrow
                if (cur_page < max_pages)
                    cur_page++;
                break;
            case 260:       // Left arrow
                if (cur_page > 0)
                    cur_page--;
                break;
            default:
                break;
        }

        // DELME
        clr_line(8);
        mvprintw(8, GRID_X, "[Debug] Pressed key: %d", c);
        refresh();
    } while (tolower(c) != 'q');
    
    endwin();       // End ncurses window
    return 0;
}

