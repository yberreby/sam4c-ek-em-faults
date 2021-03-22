int main(void) {
    int volatile * const dst = (int *)0x20001968;
    *dst = 1337;
    return 0;
}