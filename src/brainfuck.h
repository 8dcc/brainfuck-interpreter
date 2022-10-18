/*
 * Brainfuck related functions and macros.
 */

// Chcek if we want to print and refresh or not
#define CMD_OUTPUT_CHECK(...) \
    if (print_output) cmd_output(__VA_ARGS__);

int bf_step(int print_output) {
    // Used to track loop nesting for '[' and ']'
    static int loop_nesting = 0;
    int cur_loop_count      = 0;

    if (!file_loaded) {
        CMD_OUTPUT_CHECK("Can't step. No file loaded.");
        return BF_NOFILE;
    }
    // If we reached EOF. +1 because we haven't increased fpos yet
    if (fpos + 1 >= strlen(bf)) {
        CMD_OUTPUT_CHECK("Can't step. End of file reached.\n"
                         "Type reload to start processing the file from the start.");
        return BF_EOF;
    }

    int char_buff = bf[fpos++];
    switch (char_buff) {
        case '>':
            if (cur_cell < GRID_C) cur_cell++;
            CMD_OUTPUT_CHECK("Stepped: >");
            break;
        case '<':
            if (cur_cell > 0) cur_cell--;
            CMD_OUTPUT_CHECK("Stepped: <");
            break;
        case '-':
            if (cell_arr[cur_cell] > 0) cell_arr[cur_cell]--;
            CMD_OUTPUT_CHECK("Stepped: -");
            break;
        case '+':
            if (cell_arr[cur_cell] < digit2maxnum(MAX_GRID_CW)) cell_arr[cur_cell]++;
            CMD_OUTPUT_CHECK("Stepped: +");
            break;
        case ',':
            // Call cmd_output instead of macro because we always need to print this
            cmd_output("Stepped: ,\nScanning brainfuck character...");
            cell_arr[cur_cell] = getch();
            CMD_OUTPUT_CHECK("Got brainfuck character: %c (%d)", cell_arr[cur_cell],
                             cell_arr[cur_cell]);
            break;
        case '.':
            CMD_OUTPUT_CHECK("Stepped: .\nPrinting brainfuck character: %c (%d)",
                             cell_arr[cur_cell], cell_arr[cur_cell]);
            break;
        case '[':
            loop_nesting++;
            CMD_OUTPUT_CHECK("Stepped [");
            break;
        case ']':
            // First check if the current cell is 0
            if (cell_arr[cur_cell] == 0) {
                loop_nesting--;    // Exit loop
                CMD_OUTPUT_CHECK("Stepped ]\nExiting loop");
            } else {
                for (int n = 0; n < fpos; n++) {
                    if (bf[n] == '[') {
                        cur_loop_count++;

                        // If we encountered the same loop openings as the stored
                        // nesting, jump there
                        if (cur_loop_count >= loop_nesting) fpos = n + 1;
                    }
                }
                CMD_OUTPUT_CHECK("Stepped ]\nReturning to last [");
            }
            break;
        default:
            if (skip_comments) return BF_UNKNOWN;

            if (char_buff == '\n') {
                CMD_OUTPUT_CHECK("Stepped invalid char: '\\n'");
            } else {
                CMD_OUTPUT_CHECK("Stepped invalid char: '%c'", char_buff);
            }

            if (print_output) REFRESH_0();
            return BF_UNKNOWN;
    }

    if (print_output) REFRESH_0();
    return BF_OKAY;
}

void bf_run() {
    if (!file_loaded) {
        cmd_output("Can't run. No file loaded.");
        return;
    }

    // Used to store the return of bf_step(). We initialize it with bf_step so we can
    // check if the first step is EOF, and print a custom error.
    int bf_step_buff = bf_step(0);
    if (bf_step_buff == BF_EOF) {
        cmd_output("Can't run. End of file reached.\n"
                   "Type reload to start processing the file from the start.");
        return;
    }

    // Print before the loop in case it takes some seconds
    // cmd_output("Running...");

    // Call bf_step() while we are not finished and while we have our file loaded
    while (bf_step_buff != BF_EOF && bf_step_buff != BF_NOFILE)
        bf_step_buff = bf_step(0);

    cmd_output("Ran file succesfully.");
}
