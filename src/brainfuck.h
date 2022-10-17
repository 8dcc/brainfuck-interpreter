/*
 * Brainfuck related functions.
 */

// TODO: Return char for skipping invalid chars
void bf_step() {
    // Used to track loop nesting for '[' and ']'
    static int loop_nesting = 0;
    int cur_loop_count      = 0;

    if (!file_loaded) {
        cmd_output("Can't step. No file loaded.");
        return;
    }
    // If we reached EOF. +1 because we haven't increased fpos yet
    if (fpos + 1 >= strlen(bf)) {
        cmd_output("Can't step. End of file reached.");
        return;
    }

    int char_buff = bf[fpos++];
    switch (char_buff) {
        case '>':
            if (cur_cell < GRID_C) cur_cell++;
            cmd_output("Stepped: >");
            break;
        case '<':
            if (cur_cell > 0) cur_cell--;
            cmd_output("Stepped: <");
            break;
        case '-':
            if (cell_arr[cur_cell] > 0) cell_arr[cur_cell]--;
            cmd_output("Stepped: -");
            break;
        case '+':
            if (cell_arr[cur_cell] < digit2maxnum(MAX_GRID_CW)) cell_arr[cur_cell]++;
            cmd_output("Stepped: +");
            break;
        case ',':
            cmd_output("Stepped: ,\nScanning brainfuck character...");
            cell_arr[cur_cell] = getch();
            cmd_output("Got brainfuck character: %c (%d)", cell_arr[cur_cell],
                       cell_arr[cur_cell]);
            break;
        case '.':
            cmd_output("Stepped: .\nPrinting brainfuck character: %c (%d)",
                       cell_arr[cur_cell], cell_arr[cur_cell]);
            break;
        case '[':
            loop_nesting++;
            cmd_output("Stepped [");
            break;
        case ']':
            // First check if the current cell is 0
            if (cell_arr[cur_cell] == 0) {
                loop_nesting--;    // Exit loop
                cmd_output("Stepped ]\nExiting loop");
            } else {
                for (int n = 0; n < fpos; n++) {
                    if (bf[n] == '[') {
                        cur_loop_count++;

                        // If we encountered the same loop openings as the stored
                        // nesting, jump there
                        if (cur_loop_count >= loop_nesting) fpos = n + 1;
                    }
                }
                cmd_output("Stepped ]\nReturning to last [");
            }
            break;
        default:
            if (char_buff == '\n')
                cmd_output("Stepped invalid char: '\\n'");
            else
                cmd_output("Stepped invalid char: '%c'", char_buff);
            break;
    }

    REFRESH_0();
}
