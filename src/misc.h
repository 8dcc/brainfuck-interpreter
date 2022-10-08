
int get_digits(const int target) {
    int n   = target;
    int ret = 1;

    while (n > 9) {
        n /= 10;
        ret++;
    }

    return ret;
}

void die(const char* s) {
    endwin();   // End ncurses

    fwrite(s, sizeof(char), strlen(s), stderr);

    exit(1);
}

char* first_word(const char* s) {
    const int len = strlen(s);
    char* buff    = malloc(len);
    strcpy(buff, s);

    for (int n = 0; n < len; n++) {
        if (buff[n] == ' ') {
            buff[n] = '\0';
            return buff;
        }
    }

    return buff;
}

char* second_word(const char* s) {
    const int len = strlen(s);
    char* buff    = malloc(len);
    strcpy(buff, s);

    // Change ptr to seccond word
    for (int n = 0; n < len; n++)
        if (buff[n] == ' ' && buff[n + 1] != '\0') buff = &buff[n + 1];

    // End the str when the word ends
    for (int n = 0; n < len; n++) {
        if (buff[n] == ' ') {
            buff[n] = '\0';
            return buff;
        }
    }

    return buff;
}

