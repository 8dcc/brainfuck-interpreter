
/*--------------------------------------------------------------------------------*/
// Defines

// Interface
#define GRID_X      5    // Horizontal margin for the cell grid
#define GRID_Y      3
#define GRID_C      200    // Max number of items (Not items per page)
#define MIN_GRID_CW 1      // Min width of the cells
#define MAX_GRID_CW 4      // Max width of the cells. Controlled by bf_step()

#define INPUT_Y     GRID_Y + 6
#define OUTPUT_X    GRID_X
#define OUTPUT_Y    GRID_Y + 8
#define OUTPUT_ROWS 40    // Space reserved for output

// Bf codes
enum { BF_OKAY = 0, BF_UNKNOWN, BF_EOF, BF_NOFILE };

// Cmd codes
enum { CMD_OKAY = 0, CMD_QUIT };

// Cmd keys
#define KEY_LARROW  260
#define KEY_RARROW  261
#define KEY_DEL     263
#define KEY_CTRLDEL 8
#define KEY_CTRLC   3

// Misc
#define PRINT_CHARS    1    // Will print 'c' bellow the cells if true
#define BUFF_SIZE      255
#define FILE_BUFF_SIZE 2000    // Max brainfuck file size. Method should be improved
#define LOG_NAME       "bf-interpreter.log"
#define LOG_TIME       1

/*--------------------------------------------------------------------------------*/
// Macros

#define REFRESH_0() \
    {               \
        move(0, 0); \
        refresh();  \
    }

#define fail_cmd(s) cmd_output(s)

/*--------------------------------------------------------------------------------*/
// Globals

// Will store the loaded file's contents
char bf[FILE_BUFF_SIZE] = { 0 };

// Will be 1 if we have loaded a file
int file_loaded = 0;

// Current character in the loaded file
int fpos = 0;

// Brainfuck cell array
int cell_arr[GRID_C * sizeof(int)];

// Current brainfuck cell
int cur_cell = 0;

// Enable logging
int log_output = 0;

// Skip bf comments
int skip_comments = 0;
