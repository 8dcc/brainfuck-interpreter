
inline void ctrl_del(char* str, int* cursor_pos);
void cmd_help();
void load_file(const char* name);
void unload(char* buff, size_t buff_size);
void reset_grid();
inline void print_file();
inline void toggle_log();
inline void toggle_skip_comments();

int scan_command(char* cmd, int* pos, int max_len);
int parse_command(char* cmd);
