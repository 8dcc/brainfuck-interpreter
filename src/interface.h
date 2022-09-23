
#define REFRESH_0() move(0,0); refresh();

#define GRID_X 5
#define GRID_Y 3
#define GRID_C 30           // Max number of items. Should change depending on the terminal size
#define MIN_GRID_CW 1       // Min width of the cells 
int GRID_CW = MIN_GRID_CW;  // Default width of cells. Will change if we try to display large numbers

void draw_grid() {
    // Define in case we want to pass this as args in the future
    const int x      = GRID_X;
    const int y      = GRID_Y;
    const int cells  = GRID_C;
    const int cell_w = GRID_CW;

    if (cells < 1) return;

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
    const int gx  = GRID_X;
    const int gy  = GRID_Y;
    const int gc  = GRID_C;
    const int gcw = GRID_CW;

    // Return if we are out of bounds
    if (idx >= gc) return;

    // Center of y, base x pos + leftmost border + (right border of the cell + cell width) * cell number
    mvprintw(gy+1, gx+1 + (1+gcw) * idx, "%s", str);

    REFRESH_0();
}
