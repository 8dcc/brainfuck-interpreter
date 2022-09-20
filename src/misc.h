
int get_digits(const int target) {
    int n = target;
    int ret = 1;

    while (n > 9) {
        n /= 10;
        ret++;
    }

    return ret;
}
