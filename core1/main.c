#include <test_seq.h>

int main(void) {
    // int volatile * const dst = (int *)0x20001900;
    // *dst = 1337;
    
    run_test_seq();

    return 0;
}
