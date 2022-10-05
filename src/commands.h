
void cmd_help();
void load_file(const char* name);
void print_file();

/* Scans the command input by the user to cmd, if a special key (like the arrows) is
 * pressed, returns that integer so main can act. Return codes are in src/defines.h
 *
 * cmd is the command string ptr, pos is a pointer to the variable that stores the
 * last char, max len is the cmd character limit.
 */
int scan_command(char* cmd, int* pos, int max_len) {
    int c = 0;

    // Move cursor to pos (just visual). +2 because of '> '
    draw_cmd_input(cmd);

    // While we are typing
    while ((c = getch()) != '\0' && c != '\n' && *pos < max_len - 1) {
        switch (c) {
            case KEY_RARROW:
            case KEY_LARROW:
            case KEY_CTRLC:
                // Special keys are just returned to main
                return c;
            case KEY_DEL:
                // We subract one from pos and clear that char
                if (*pos > 0) cmd[--(*pos)] = '\0';
                draw_cmd_input(cmd);
                continue;
            default:
                break;
        }

        // Append to cmd the char we just got
        cmd[(*pos)++] = c;

        // Refresh cmd line
        draw_cmd_input(cmd);
    }

    return CMD_OKAY;
}

// Scans for commands
int parse_command(const char* cmd) {
    draw_cmd_input("");    // Clear the cmd line
    clr_cmd_output();

    if (strstr(cmd, "ref")) {
        refresh();
    } else if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit")) {
        return CMD_QUIT;
    } else if (!strcmp(cmd, "help")) {
        cmd_help();
    // Commands with arguments
    } else if (!strcmp(first_word(cmd), "load")) {
        load_file(second_word(cmd));
    } else if (!strcmp(first_word(cmd), "print")) {
        print_file();
    } else {
        cmd_output("Invalid command!");
    }

    return CMD_OKAY;
}

void cmd_help() {
    cmd_output("Displaying help:\n"
               "    help | Shows this help\n"
               "    quit | Exit the program\n"
               "    ref  | Calls refresh()\n");
}

void load_file(const char* name) {
    FILE* fd = fopen(name, "r");
    if (!fd) die("load_file: could not open file");

    int c = 0;
    for (int n = 0; n < FILE_BUFF_SIZE && (c = fgetc(fd)) != EOF; n++) {
        bf[n] = c;
    }

    cmd_output("File loaded.");
    file_loaded = 1;
}

// Prints the contents of the loaded file
void print_file() {
    if (!file_loaded) return;

    cmd_output(bf);
}
