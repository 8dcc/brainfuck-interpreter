
void draw_grid() {
    // Define in case we want to pass this as args in the future
    const int x      = GRID_X;
    const int y      = GRID_Y;
    const int cell_w = GRID_CW;
    int term_w       = getmaxx(stdscr);    // Get terminal width

    // (terminal width - margin * 2) / (cell width + cell border)
    const int cells = (term_w - GRID_X * 2) / (cell_w + 1);
    if (cells < 1) return;
    GRID_CPP = cells;

    // First clear the cell lines to remove residual chars from resizes, etc.
    clr_line(y);
    clr_line(y + 1);
    clr_line(y + 2);
    clr_line(y + 3);    // Page number line
    if (PRINT_CHARS)
        clr_line(y + 4);    // If we want to print chars, the page line is at pos 4

    // Initial left border
    mvprintw(y, x, "+");
    mvprintw(y + 1, x, "|");
    mvprintw(y + 2, x, "+");

    for (int n = 0; n < cells; n++) {
        /* Center of each cell. We use +1 because of the initial left border and
         * 2*n because its the space of a cell times the current cell number:
         *   -+-+-+
         *    | | |
         *   -+-+-+
         *   ^^
         *
         * We iterate the cell w.
         */
        for (int i = 0; i < cell_w; i++) {
            // (base x pos + left border) + ((cell width + right border) * cell
            // number) + inner cell position
            mvprintw(y, (x + 1) + ((cell_w + 1) * n) + i, "-");
            mvprintw(y + 2, (x + 1) + ((cell_w + 1) * n) + i, "-");
        }

        // Right border
        // base x pos + cell inner width + right border * current cell number
        mvprintw(y, x + (cell_w + 1) * n, "+");
        mvprintw(y + 1, x + (cell_w + 1) * n, "|");
        mvprintw(y + 2, x + (cell_w + 1) * n, "+");
    }

    // Rightmost border
    mvprintw(y, x + (cell_w + 1) * cells, "+");
    mvprintw(y + 1, x + (cell_w + 1) * cells, "|");
    mvprintw(y + 2, x + (cell_w + 1) * cells, "+");

    REFRESH_0();
}

void clr_line(int y) {
    // Save originals
    int ox = 0, oy = 0;
    getyx(stdscr, oy, ox);

    move(y, 0);
    clrtoeol();

    move(oy, ox);
}

void fill_cell(int idx, const char* str) {
    const int gx   = GRID_X;
    const int gy   = GRID_Y;
    const int gcpp = GRID_CPP;
    const int gcw  = GRID_CW;

    // Return if we are out of bounds
    if (idx >= gcpp) return;

    // Center of y, base x pos + leftmost border + (right border of the cell +
    // cell width) * cell number
    mvprintw(gy + 1, gx + 1 + (1 + gcw) * idx, "%s", str);

    // Print the chars behind the cells (centered)
    if (PRINT_CHARS) {
        const int converted_char = int2char(atoi(str));
        if (converted_char)
            mvprintw(gy + 3, (gx + 1 + (1 + gcw) * idx) + (gcw / 2 - 1), "'%c'",
                     converted_char);
    }

    REFRESH_0();
}

int int2char(int num) {
    if (num >= 32 && num <= 126) return num;
    return 0;
}

// Calls fill_cell for the necesary items to fill the whole grid. Also prints
// the page number
void fill_grid(int* cell_arr, int page) {
    char* buff   = calloc(255, sizeof(char));
    int real_idx = 0;

    for (int n = 0; n < GRID_C; n++) {
        // cells per page * current page + pos indide page
        real_idx = GRID_CPP * page + n;

        // Save int at real array pos to buff as long as we are in bounds
        sprintf(buff, "%d", (real_idx < GRID_C) ? cell_arr[real_idx] : 0);

        // Print the buff str at the screen pos
        fill_cell(n, buff);
    }

    free(buff);

    // Print page numbers. Keep in mind that we add one so its '1/5' instead of
    // '0/4'
    mvprintw((PRINT_CHARS) ? GRID_Y + 4 : GRID_Y + 3, GRID_X, "[Page %d/%d]",
             page + 1, GRID_C / GRID_CPP + 1);

    REFRESH_0();
}

// Draws '>' + cmd
void draw_cmd_input(const char* cmd) {
    clr_line(INPUT_Y);
    mvprintw(INPUT_Y, GRID_X, "> %s", cmd);

    refresh();    // We dont call REFRESH_0() cuz we want to show the cursor after
                  // the cmd
}

void clr_cmd_output() {
    for (int n = 0; n < OUTPUT_ROWS; n++) clr_line(OUTPUT_Y + n);
}

void cmd_output(const char* str, ...) {
    va_list va;
    va_start(va, str);

    // Get terminal and output region widths
    const int term_w = getmaxx(stdscr);
    const int out_w  = (term_w - GRID_X * 2);
    const int out_h  = OUTPUT_ROWS - 1;

    char buff[FILE_BUFF_SIZE] = { 0 };
    int buff_pos              = 0;
    int line                  = 1;    // Lines printed
    int line_p                = 0;    // Char pos in the current line

    clr_cmd_output();

    // Loop and load into buff for printing on the same Y and check stuff
    for (int n = 0; buff_pos < FILE_BUFF_SIZE && n < FILE_BUFF_SIZE &&
                    str[n] != '\0' && line < out_h;
         n++) {
        buff[buff_pos++] = str[n];
        line_p++;

        if (line_p > out_w) buff[buff_pos++] = '\n';

        // After putting newline on the string, add indentation
        if (str[n] == '\n' || line_p > out_w) {
            line++;
            line_p = 0;

            for (int i = 0; i < OUTPUT_X && buff_pos < FILE_BUFF_SIZE; i++)
                buff[buff_pos++] = ' ';

            // Make sure we are inside the output w
            if (line >= out_h) {
                buff[buff_pos++] = '.';
                buff[buff_pos++] = '.';
                buff[buff_pos++] = '.';
                break;
            }
        }
    }

    if (log_output) {
        FILE* fd = fopen(LOG_NAME, "a");

        if (LOG_TIME) fprintf(fd, "[%ld] ", time(NULL));

        vfprintf(fd, str, va);

        // Print newline if str doesn't have one
        if (str[strlen(str)-1] != '\n') fprintf(fd, "\n");

        fclose(fd);
    }

    move(OUTPUT_Y, OUTPUT_X);
    vw_printw(stdscr, buff, va);
    va_end(va);

    refresh();
}
