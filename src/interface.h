
int GRID_CPP = 30;    // Cells displayed per page. Will be calculated depenging on
                      // terminal width in draw_grid()
int GRID_CW = MIN_GRID_CW;    // Default width of cells. Will change if we try to
                              // display large numbers

void clr_line(int y);     // Clears line at y, then returns cursor to previous pos
int int2char(int num);    // Returns valid chars to be printed

void draw_grid();
void fill_cell(int idx, const char* str);
void fill_grid(int* cell_arr, int page);
void draw_cmd_input(const char* cmd);
void clr_cmd_output();
void cmd_output(const char* str, ...);
