
int get_digits(const int target) {
    int n   = target;
    int ret = 1;

    while (n > 9) {
        n /= 10;
        ret++;
    }

    return ret;
}

// Returns the max number with the specified digits (4 -> 9999)
long digit2maxnum(int digits) {
    int ret = 10;

    // Digit - 1 because we already start at 10
    for (int n = 0; n < digits - 1; n++) ret *= 10;

    // -1 because we dont want 100, but 99
    return ret - 1;
}

void die(const char* s) {
    endwin();    // End ncurses

    fwrite(s, sizeof(char), strlen(s), stderr);

    exit(1);
}

char* first_word(const char* s) {
    const int len = strlen(s);
    char* buff    = calloc(len, sizeof(char));

    for (int n = 0; n < len; n++) {
        buff[n] = s[n];

        if (buff[n] == ' ') {
            buff[n] = '\0';
            break;
        }
    }

    return buff;
}

char* second_word(const char* s) {
    const int len = strlen(s);
    char* buff    = calloc(len, sizeof(char));
    int buff_p    = 0;

    int spaces = 0;
    for (int n = 0; n < len; n++) {
        if (s[n] == ' ') {
            spaces++;
        } else if (spaces == 1) {
            buff[buff_p++] = s[n];
        } else if (spaces == 2) {
            buff[buff_p] = '\0';
            break;
        }
    }

    return buff;
}

