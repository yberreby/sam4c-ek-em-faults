#ifndef TEST_AES_H
#define TEST_AES_H

#include <stdbool.h>

void init_aes(void);
bool check_ecb_encryption(void);
bool check_ecb_decryption(void);

#endif