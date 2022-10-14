/*
 * Brainfuck related functions.
 */

void bf_step() {
    if (!file_loaded) {
        cmd_output("Can't step. No file loaded.");
        return;
    }
    // If we reached EOF. +1 because we haven't increased fpos yet
    if (fpos+1 >= strlen(bf)) {
        cmd_output("Can't step. End of file reached.");
        return;
    }

    switch (bf[fpos++]) {
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
            cmd_output("Scanning brainfuck character...");
            cell_arr[cur_cell] = getch();
            cmd_output("Got brainfuck character...");    // TODO: va_list
            break;
        case '.':
            cmd_output("Printing brainfuck character...");    // TODO: va_list
            break;
        // TODO: [ ]
        default:
            break;
    }

    REFRESH_0();
}
