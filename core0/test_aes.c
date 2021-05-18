#include "test_aes.h"
#include <aes.h>

#define AES_EXAMPLE_REFBUF_SIZE 4

/** Cipher 128 bits key array. */
const uint32_t key128[4] = {
    0x16157e2b,
    0xa6d2ae28,
    0x8815f7ab,
    0x3c4fcf09
};

extern volatile bool aes_output_ready;
extern volatile uint32_t aes_output_data[AES_EXAMPLE_REFBUF_SIZE];

uint32_t ref_plain_text[AES_EXAMPLE_REFBUF_SIZE] = {
    0xe2bec16b,
    0x969f402e,
    0x117e3de9,
    0x2a179373
};

/** Reference ECB cipher data. */
uint32_t ref_cipher_text_ecb[AES_EXAMPLE_REFBUF_SIZE] = {
    0xb47bd73a,
    0x60367a0d,
    0xf3ca9ea8,
    0x97ef6624
};


static void aes_callback(void) {
    /* Read the output. */
    aes_read_output_data(AES, aes_output_data);
    aes_output_ready = true;
}

struct aes_config g_aes_cfg;

void init_aes() {
    aes_get_config_defaults(&g_aes_cfg);
    aes_init(AES, &g_aes_cfg);
    aes_enable();


    /* Enable AES interrupt. */
    aes_set_callback(AES, AES_INTERRUPT_DATA_READY, aes_callback, 1);
}



bool check_ecb_encryption() {
    aes_output_ready = false;

    /* Configure the AES. */
    g_aes_cfg.encrypt_mode = AES_ENCRYPTION;
    g_aes_cfg.key_size = AES_KEY_SIZE_128;
    g_aes_cfg.start_mode = AES_MANUAL_START;
    g_aes_cfg.opmode = AES_ECB_MODE;
    g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
    g_aes_cfg.lod = false;
    aes_set_config(AES, &g_aes_cfg);

    /* Set the cryptographic key. */
    aes_write_key(AES, key128);

    /* The initialization vector is not used by the ECB cipher mode. */

    /* Write the data to be ciphered to the input data registers. */
    aes_write_input_data(AES, ref_plain_text);

    aes_start(AES);

    /* Wait for the end of the encryption process. */
    while (false == aes_output_ready) {
    }

    if ((ref_cipher_text_ecb[0] != aes_output_data[0]) ||
        (ref_cipher_text_ecb[1] != aes_output_data[1]) ||
        (ref_cipher_text_ecb[2] != aes_output_data[2]) ||
        (ref_cipher_text_ecb[3] != aes_output_data[3])) {
        return false;
    } else {
        return true;
    }
}


bool check_ecb_decryption() {
    aes_output_ready = false;

    /* Configure the AES. */
    g_aes_cfg.encrypt_mode = AES_DECRYPTION;
    g_aes_cfg.key_size = AES_KEY_SIZE_128;
    g_aes_cfg.start_mode = AES_MANUAL_START;
    g_aes_cfg.opmode = AES_ECB_MODE;
    g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
    g_aes_cfg.lod = false;
    aes_set_config(AES, &g_aes_cfg);

    /* Set the cryptographic key. */
    aes_write_key(AES, key128);

    /* The initialization vector is not used by the ECB cipher mode. */

    /* Write the data to be deciphered to the input data registers. */
    aes_write_input_data(AES, ref_cipher_text_ecb);


    aes_start(AES);

    /* Wait for the end of the decryption process. */
    while (false == aes_output_ready) {}

    /* check the result. */
    if ((ref_plain_text[0] != aes_output_data[0]) ||
        (ref_plain_text[1] != aes_output_data[1]) ||
        (ref_plain_text[2] != aes_output_data[2]) ||
        (ref_plain_text[3] != aes_output_data[3])) {
        return false;
    } else {
        return true;
    }
}