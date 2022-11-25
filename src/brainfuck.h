
// Chcek if we want to print and refresh or not
#define CMD_OUTPUT_CHECK(...) \
    if (print_output) cmd_output(__VA_ARGS__);

int bf_step(int print_output);
void bf_run();
