
void cmd_help();
void load_file(const char* name);
void unload(char* buff, size_t buff_size);
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

    // Store the word ptrs here so we can free them later. Feel free to PR if there
    // is a better way.
    char* fw = first_word(cmd);
    char* sw = second_word(cmd);

    if (!strcmp(fw, "refresh")) {
        refresh();
    } else if (!strcmp(fw, "quit") || !strcmp(fw, "exit")) {
        return CMD_QUIT;
    } else if (!strcmp(fw, "help")) {
        cmd_help();
        // Commands with arguments
    } else if (!strcmp(fw, "load")) {
        load_file(sw);
    } else if (!strcmp(fw, "reload")) {
        fpos = 0;
    } else if (!strcmp(fw, "unload")) {
        unload(bf, FILE_BUFF_SIZE);
    } else if (!strcmp(fw, "step")) {
        bf_step();
    } else if (!strcmp(fw, "print")) {
        print_file();
    } else {
        cmd_output("Invalid command!");
    }

    free(fw);
    free(sw);

    return CMD_OKAY;
}

void cmd_help() {
    cmd_output("Displaying help:\n"
               "    help            | Shows this help\n"
               "    quit            | Exit the program\n"
               "    load <filename> | Loads the contents of the specified file\n"
               "    reload          | Reloads the current file (start processing "
               "file from start)\n"
               "    unload          | Clears the loaded file's buffer\n"
               "    print           | Prints the contents of the loaded file\n"
               "    step            | Processes the current brainfuck char from the "
               "buffer\n"
               "    refresh         | Calls refresh()\n");
}

void unload(char* buff, size_t buff_size) {
    for (int n = 0; n < buff_size; n++) buff[n] = '\0';

    file_loaded = 0;
}

void load_file(const char* name) {
    FILE* fd = fopen(name, "r");
    if (!fd) {
        char* buff = calloc(23 + strlen(name), sizeof(char));

        sprintf(buff, "Could not open file: %s\n", name);
        fail_cmd(buff);

        free(buff);
        return;
    }

    // Call unload to clear the buffer
    unload(bf, FILE_BUFF_SIZE);

    int c = 0;
    for (int n = 0; n < FILE_BUFF_SIZE && (c = fgetc(fd)) != EOF; n++)
        bf[n] = c;    // Get char from file and save it to bf buffer

    fpos        = 0;    // Reset file pos
    file_loaded = 1;
    cmd_output("File loaded.");
}

// Prints the contents of the loaded file
void print_file() {
    if (!file_loaded) {
        cmd_output("Can't print. No loaded file.");
        return;
    }

    cmd_output(bf);
}
