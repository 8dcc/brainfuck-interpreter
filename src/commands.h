
void cmd_help();
void load_file(const char* name);
void unload(char* buff, size_t buff_size);
void reset_grid();
inline void print_file();
inline void toggle_log();
inline void toggle_skip_comments();

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

/*
 * Parses the commands, does what it needs to do depending on the command and returns
 * CMD_CODE for special cases.
 * cmd argument is not const because we will replace cmd with the last used command
 * if cmd is just '\n'.
 */
int parse_command(char* cmd) {
    static char last_cmd[BUFF_SIZE] = { '\0' };

    draw_cmd_input("");    // Clear the cmd line
    clr_cmd_output();

    // If user cmd is empty, use last cmd if its not empty as well. If is not,
    // overwrite the last cmd.
    if (cmd[0] == '\0') {
        if (last_cmd[0] == '\0') {
            cmd_output("No previous command!");
            return CMD_OKAY;
        } else {
            cmd = last_cmd;
        }
    } else {
        strcpy(last_cmd, cmd);
    }

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
    } else if (!strcmp(fw, "load")) {
        load_file(sw);
    } else if (!strcmp(fw, "reload")) {
        fpos = 0;
    } else if (!strcmp(fw, "unload")) {
        unload(bf, FILE_BUFF_SIZE);
    } else if (!strcmp(fw, "reset")) {
        reset_grid();
    } else if (!strcmp(fw, "step")) {
        if (skip_comments)
            while (bf_step(1) == BF_UNKNOWN)
                ;
        else
            bf_step(1);
    } else if (!strcmp(fw, "run")) {
        bf_run();
    } else if (!strcmp(fw, "print")) {
        print_file();
    } else if (!strcmp(fw, "log")) {
        toggle_log();
    } else if (!strcmp(fw, "skip_comments") || !strcmp(fw, "ignore_comments") ||
               !strcmp(fw, "toggle_comments")) {
        toggle_skip_comments();
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
               "    reset           | Resets the brainfuck grid\n"
               "    step            | Processes the current brainfuck char from the "
               "buffer\n"
               "    run             | Runs step until the bf buffer is executed\n"
               "    log             | Toggles output logging to file\n"
               "    skip_comments   | Toggles comment skipping when processing bf\n"
               "    refresh         | Calls refresh()\n"
               "\n"
               "Tip: You can press enter to execute the last command.\n");
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
        fclose(fd);
        return;
    }

    // Call unload to clear the buffer
    unload(bf, FILE_BUFF_SIZE);

    int c = 0;
    for (int n = 0; n < FILE_BUFF_SIZE && (c = fgetc(fd)) != EOF; n++)
        bf[n] = c;    // Get char from file and save it to bf buffer

    fclose(fd);

    fpos        = 0;    // Reset file pos
    file_loaded = 1;
    cmd_output("File '%s' loaded.", name);
}

void reset_grid() {
    for (int n = 0; n < GRID_C; n++) cell_arr[n] = '\0';
    cmd_output("Reset grid.");
}

// Prints the contents of the loaded file
void print_file() {
    if (!file_loaded) {
        cmd_output("Can't print. No loaded file.");
        return;
    }

    cmd_output(bf);
}

void toggle_log() {
    log_output = !log_output;

    // Clear file
    if (log_output) {
        FILE* fd = fopen(LOG_NAME, "w");
        fclose(fd);
    }
    cmd_output((log_output) ? "Logging enabled...\n" : "Logging disabled...\n");
}

void toggle_skip_comments() {
    skip_comments = !skip_comments;
    cmd_output((skip_comments) ? "Ingnoring comments...\n"
                               : "Printing comments...\n");
}

