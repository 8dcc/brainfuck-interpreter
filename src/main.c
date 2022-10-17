#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>      // For time()
#include <stdarg.h>    // For va_list
#include <ctype.h>     // For tolower()
#include <ncurses.h>

#include "defines.h"
#include "misc.h"
#include "interface.h"
#include "brainfuck.h"
#include "commands.h"

int main() {
    initscr();               // Init ncurses
    raw();                   // Scan input without pressing enter
    noecho();                // Don't print when typing
    keypad(stdscr, TRUE);    // Enable keypad (arrow keys)

    mvprintw(1, GRID_X, "Type 'help' for a list of available commands...");

    // Initialize as 0 the cell array for the memory simulation
    for (int n = 0; n < GRID_C; n++) cell_arr[n] = 0;

    // Used to store the current command
    char cmd[BUFF_SIZE] = { '\0' };
    int cmd_code        = CMD_OKAY;    // All defined in defines.h
    int cmd_pos         = 0;

    // Max pages of the grid. Updated each loop because the terminal size might
    // change
    int max_pages = 0;
    int cur_page  = 0;

    // Main loop. If cmd is quit it will exit
    while (cmd_code != CMD_QUIT) {
        max_pages = GRID_C / GRID_CPP;

        // First set GRID_CW to the max str len
        for (int n = 0; n < GRID_C; n++) {
            int digits = get_digits(cell_arr[n]);
            if (digits > GRID_CW) GRID_CW = digits;
        }

        draw_grid();
        fill_grid(cell_arr, cur_page);

        /*
         * Get command input after drawing everything. Store it in the ptr and get
         * the return code (for stuff like the arrows, etc.)
         * The cmd line is refreshed inside scan_command() because of noecho()
         *
         * Once scan_command returns we check the cmd code it returned, if its a
         * specific key, act, if it returned normally, parse the cmd string inside
         * another switch. parse_command() will either act directly or return the
         * code so we can act inside main (aka calling other funcs).
         */
        cmd_code = scan_command(cmd, &cmd_pos, BUFF_SIZE);
        switch (cmd_code) {
            case KEY_RARROW:
                if (cur_page < max_pages) cur_page++;
                break;
            case KEY_LARROW:
                if (cur_page > 0) cur_page--;
                break;
            case KEY_CTRLC:
                cmd_code = CMD_QUIT;
                break;
            case CMD_OKAY:
            default:
                // If scan_command returned fine, parse the command we just got and
                // do what we need
                switch (parse_command(cmd)) {
                    case CMD_QUIT:
                        cmd_code = CMD_QUIT;
                        break;
                    case CMD_OKAY:
                    default:
                        // Reset cmd after input
                        memset(cmd, 0, BUFF_SIZE);
                        cmd_pos = 0;
                        break;    // End parse_command switch
                }
                break;    // End cmd_code switch
        }
    }    // End of main loop

    // End ncurses window
    endwin();
    return 0;
}
