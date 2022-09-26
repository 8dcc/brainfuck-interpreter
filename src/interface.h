
#define REFRESH_0() move(0,0); refresh();
#define CLR_LINE(y) move(y,0); clrtoeol();      // TODO: Save old cursor pos and restore after clear

#define GRID_X 5            // Horizontal margin for the cell grid
#define GRID_Y 3
#define GRID_C 200          // Max number of items. Items per page should change depending on the terminal size
int GRID_CPP = 30;          // Cells displayed per page. Will be calculated depenging on terminal width in draw_grid()
#define MIN_GRID_CW 1       // Min width of the cells 
int GRID_CW = MIN_GRID_CW;  // Default width of cells. Will change if we try to display large numbers

void draw_grid() {
    // Define in case we want to pass this as args in the future
    const int x      = GRID_X;
    const int y      = GRID_Y;
    const int cell_w = GRID_CW;
    int term_w       = getmaxx(stdscr);     // Get terminal width

    // (terminal width - margin * 2) / (cell width + cell border)
    const int cells = (term_w - GRID_X * 2) / (cell_w + 1);
    if (cells < 1) return;
    GRID_CPP = cells;

    // TODO: DELME
    mvprintw(x, 10, "[Debug] W: %d | Cells: %d", term_w, cells);

    // First clear the cell lines to remove residual chars from resizes, etc.
    CLR_LINE(y);
    CLR_LINE(y+1);
    CLR_LINE(y+2);

    // Initial left border
    mvprintw(y,   x, "+");
    mvprintw(y+1, x, "|");
    mvprintw(y+2, x, "+");

    for (int n = 0; n < cells; n++) {
        /* Center of each cell. We use +1 because of the initial left border and 2*n because
         * its the space of a cell times the current cell number:
         *   -+-+-+
         *    | | |
         *   -+-+-+
         *   ^^
         *
         * We iterate the cell w.
         */
        for (int i = 0; i < cell_w; i++) {
            // (base x pos + left border) + ((cell width + right border) * cell number) + inner cell position
            mvprintw(y,   (x+1) + ((cell_w+1)*n) + i, "-");
            mvprintw(y+1, (x+1) + ((cell_w+1)*n) + i, " ");
            mvprintw(y+2, (x+1) + ((cell_w+1)*n) + i, "-");
        }

        // Right border
        // base x pos + cell inner width + right border * current cell number
        mvprintw(y,   x+(cell_w+1)*n, "+");
        mvprintw(y+1, x+(cell_w+1)*n, "|");
        mvprintw(y+2, x+(cell_w+1)*n, "+");
    }

    // Rightmost border
    mvprintw(y,   x+(cell_w+1)*cells, "+");
    mvprintw(y+1, x+(cell_w+1)*cells, "|");
    mvprintw(y+2, x+(cell_w+1)*cells, "+");

    REFRESH_0();
}


void fill_cell(int idx, const char* str) {
    const int gx   = GRID_X;
    const int gy   = GRID_Y;
    const int gcpp = GRID_CPP;
    const int gcw  = GRID_CW;

    // Return if we are out of bounds
    if (idx >= gcpp) return;

    // Center of y, base x pos + leftmost border + (right border of the cell + cell width) * cell number
    mvprintw(gy+1, gx+1 + (1+gcw) * idx, "%s", str);

    REFRESH_0();
}


// Calls fill_cell for the necesary items to fill the whole grid
void fill_grid(int* cell_arr, int page) {
    char* buff= calloc(255, sizeof(char));
    int real_idx = 0;

    for (int n = 0; n < GRID_C; n++) {
        // cells per page * current page + pos indide page
        real_idx = GRID_CPP * page + n;

        // Save int at real array pos to buff as long as we are in bounds
        sprintf(buff, "%d", (real_idx < GRID_C) ? cell_arr[real_idx] : 0);

        // Print the buff str at the screen pos
        fill_cell(n, buff);
    }

    // Print page numbers. Keep in mind that we add one so its '1/5' instead of '0/4'
    mvprintw(GRID_Y + 3, GRID_X, "[Page %d/%d]", page+1, GRID_C/GRID_CPP+1);

    REFRESH_0();
}
