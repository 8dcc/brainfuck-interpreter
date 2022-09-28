
void cmd_help();

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
    } else if (strstr(cmd, "quit") || strstr(cmd, "exit")) {
        return CMD_QUIT;
    } else if (strstr(cmd, "help")) {
        cmd_help();
    } else {
        cmd_output("Invalid command!");
    }

    return CMD_OKAY;
}

void cmd_help() {
    cmd_output(
            "Displaying help:\n"
            "    help | Shows this help\n"
            "    quit | Exit the program\n"
            "    ref  | Calls refresh()\n"
            );
}
