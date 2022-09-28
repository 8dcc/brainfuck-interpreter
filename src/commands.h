
/* Scans the command input by the user to cmd, if a special key (like the arrows) is
 * pressed, returns that integer so main can act. Return codes are in src/defines.h
 *
 * cmd is the command string ptr, pos is a pointer to the variable that stores the
 * last char, max len is the cmd character limit.
 */
int scan_command(char* cmd, int* pos, int max_len) {
    int c = 0;

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

        /*
        // DELME
        clr_line(10);
        mvprintw(10, GRID_X, "Char: %d", c);
        refresh();
        */
    }

    return CMD_OKAY;
}

// Scans for commands
int parse_command(char* cmd) {
    if (strstr(cmd, "ref")) {
        refresh();
    } else if (strstr(cmd, "quit")) {
        return CMD_QUIT;
    }

    return CMD_OKAY;
}
