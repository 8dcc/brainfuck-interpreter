// Interface
#define GRID_X      5    // Horizontal margin for the cell grid
#define GRID_Y      3
#define GRID_C      200    // Max number of items (Not items per page)
#define MIN_GRID_CW 1      // Min width of the cells

#define OUTPUT_X    GRID_X
#define OUTPUT_Y    GRID_Y + 8
#define OUTPUT_ROWS 20    // Space reserved for output

// Cmd codes
#define CMD_OKAY 0
#define CMD_QUIT 1

// Cmd keys
#define KEY_LARROW 260
#define KEY_RARROW 261
#define KEY_DEL    263
#define KEY_CTRLC  3

// Misc
#define BUFF_SIZE   255
#define PRINT_CHARS 1    // Will print 'c' bellow the cells if true

/*--------------------------------------------------------------------------------*/
// Macros

#define REFRESH_0() \
    move(0, 0);     \
    refresh();

