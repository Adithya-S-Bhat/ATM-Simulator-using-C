// Basic Implementation of AES
//

#include <stdio.h>   // for printf
#include <stdlib.h>  // for malloc, free

#include "server.h"
#define BLOCK_SIZE 16  // cannot be changed

enum ERROR_CODES {
    SUCCESS = 0,
    ERROR_AES_UNKNOWN_KEYSIZE,
    ERROR_MEMORY_ALLOCATION_FAILED,
};
enum KEY_SIZES { SIZE_16 = 16, SIZE_24 = 24, SIZE_32 = 32 };

// API Utils
//
void expand_key(unsigned char* expanded_key, unsigned char* key,
                enum KEY_SIZES key_size, size_t expanded_key_size);
char aes_encrypt(unsigned char* input, unsigned char* output,
                 unsigned char* key, enum KEY_SIZES key_size);
char aes_decrypt(unsigned char* input, unsigned char* output,
                 unsigned char* key, enum KEY_SIZES key_size);

int get_cipher_size() { return BLOCK_SIZE; }
int get_key_size() { return SIZE_16; }
void _padding(unsigned char* input, unsigned char* output) {
    int i = 0;
    for (; input[i] != 0x00; i++) output[i] = input[i];
    for (; i < BLOCK_SIZE; i++) output[i] = 0x00;
}

// API Calls
//
void encryptor(unsigned char* input, unsigned char* cipher) {
    unsigned char key[SIZE_16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                  0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                  0x09, 0xcf, 0x4f, 0x3c};
    unsigned char padded_input[BLOCK_SIZE];
    _padding(input, padded_input);
    aes_encrypt(padded_input, cipher, key, SIZE_16);
}
void decryptor(unsigned char* cipher, unsigned char* output) {
    unsigned char key[SIZE_16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
                                  0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88,
                                  0x09, 0xcf, 0x4f, 0x3c};
    unsigned char padded_output[BLOCK_SIZE];
    aes_decrypt(cipher, padded_output, key, SIZE_16);
    int i = 0;
    for (; padded_output[i] != 0x00; i++) output[i] = padded_output[i];
    output[i] = 0x00;
}

// DEMO
void demo() {
    /* NOTE: INPUT_SIZE MUST BE EQUAL TO OUTPUT SIZE */
    unsigned char input[10] = {'h', 'e', 'l', 'l', 'o', '\0'};
    int block_size = get_cipher_size();
    unsigned char cipher[block_size], output[10];
    printf("Input: %s\n", input);
    encryptor(input, cipher);
    printf("Cipher (HEX): ");
    for (int i = 0; i < block_size; i++) printf("%02x", cipher[i]);
    printf("\n");
    decryptor(cipher, output);
    printf("Output: %s\n", output);
}

// Substitution Box
unsigned char sbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // F

unsigned char rsbox[256] = {  // reverse
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
    0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32,
    0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50,
    0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41,
    0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b,
    0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
    0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
    0x55, 0x21, 0x0c, 0x7d};

// Round Constant
unsigned char rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c,
    0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a,
    0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6,
    0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72,
    0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e,
    0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5,
    0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02,
    0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
    0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d,
    0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb,
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c,
    0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a,
    0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb};

// Functional
//
unsigned char _get_sbox_value(unsigned char num) { return sbox[num]; }
unsigned char _get_inv_sbox_value(unsigned char num) { return rsbox[num]; }
unsigned char _get_rcon_value(unsigned char num) { return rcon[num]; }

unsigned char galois_multi(unsigned char a, unsigned char b) {
    unsigned char prod = 0x00, hi_bit_set;
    for (int i = 0; i < 8; i++) {
        if ((b & 0x01) == 0x01) prod ^= a;
        hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set == 0x80) a ^= 0x1b;
        b >>= 1;
    }
    return prod;
}

void output_state(unsigned char* state) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%2.2x%c", state[i + 4 * j], ((j + 1) % 4) ? ' ' : '\n');
        }
    }
}

// Expand key
//

void _rotate_word(unsigned char* word) {
    // Rotate the 32-bit word 8 bits to the left
    unsigned char c;
    int i;
    c = word[0];
    for (i = 0; i < 3; i++) word[i] = word[i + 1];
    word[3] = c;
}

void _rcon_emb(unsigned char* word, int iteration) {
    _rotate_word(word);
    for (int i = 0; i < 4; ++i) word[i] = _get_sbox_value(word[i]);
    word[0] = word[0] ^ _get_rcon_value(iteration);
}

void expand_key(unsigned char* expanded_key, unsigned char* key,
                enum KEY_SIZES key_size, size_t expanded_key_size) {
    int cur_size = 0, rcon_iter = 1, i;
    unsigned char tmp[4] = {0};

    // init
    for (i = 0; i < key_size; i++) expanded_key[i] = key[i];
    cur_size += key_size;

    while (cur_size < expanded_key_size) {
        // assign previous 4 bytes to tmp
        for (i = 0; i < 4; i++) tmp[i] = expanded_key[i + cur_size - 4];

        // apply _rcon_emb schedule per key_size
        if (cur_size % key_size == 0) _rcon_emb(tmp, rcon_iter++);

        // sbox for 256-bit key (for num of bits > 24 in facts)
        if (key_size == SIZE_32 && (cur_size % key_size) == 16)
            for (i = 0; i < 4; i++) tmp[i] = _get_sbox_value(tmp[i]);

        // XOR tmp with four-byte block key_size before the current
        for (i = 0; i < 4; i++) {
            expanded_key[cur_size] = expanded_key[cur_size - key_size] ^ tmp[i];
            cur_size++;
        }
    }
}

// AES Encryption
//

void substitute_bytes(unsigned char* state) {
    for (int i = 0; i < 16; i++) state[i] = _get_sbox_value(state[i]);
}

void _shift_row_once(unsigned char* state, int n_rows) {
    unsigned char c;
    for (int i = 0; i < n_rows; i++) {
        c = state[0];
        for (int j = 0; j < 3; j++) state[j] = state[j + 1];
        state[3] = c;
    }
}

void shift_rows(unsigned char* state) {
    for (int i = 0; i < 4; i++) _shift_row_once(state + i * 4, i);
}

void _mix_column_once(unsigned char* col) {
    unsigned char cpy[4];
    for (int i = 0; i < 4; i++) cpy[i] = col[i];
    col[0] = galois_multi(cpy[0], 2) ^ galois_multi(cpy[3], 1) ^
             galois_multi(cpy[2], 1) ^ galois_multi(cpy[1], 3);
    col[1] = galois_multi(cpy[1], 2) ^ galois_multi(cpy[0], 1) ^
             galois_multi(cpy[3], 1) ^ galois_multi(cpy[2], 3);
    col[2] = galois_multi(cpy[2], 2) ^ galois_multi(cpy[1], 1) ^
             galois_multi(cpy[0], 1) ^ galois_multi(cpy[3], 3);
    col[3] = galois_multi(cpy[3], 2) ^ galois_multi(cpy[2], 1) ^
             galois_multi(cpy[1], 1) ^ galois_multi(cpy[0], 3);
}

void mix_columns(unsigned char* state) {
    unsigned char cols[4] = {0};
    int i, j;

    // Similar to Matrix Multiplication
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) cols[j] = state[j * 4 + i];
        _mix_column_once(cols);
        for (j = 0; j < 4; j++) state[j * 4 + i] = cols[j];
    }
}

void add_round_key(unsigned char* state, unsigned char* round_key) {
    for (int i = 0; i < 16; i++) state[i] = state[i] ^ round_key[i];
}

void create_round_key(unsigned char* expanded_key, unsigned char* round_key) {
    /* Equivalent to Transpose */
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            round_key[i + j * 4] = expanded_key[j + i * 4];
}

void aes_backbone(unsigned char* state, unsigned char* expanded_key,
                  int n_rounds) {
    unsigned char round_key[16];

    // Main Body
    create_round_key(expanded_key, round_key);
    add_round_key(state, round_key);

    for (int i = 1; i < n_rounds; i++) {
        substitute_bytes(state);
        shift_rows(state);
        mix_columns(state);
        create_round_key(expanded_key + i * 16, round_key);
        add_round_key(state, round_key);
    }
    substitute_bytes(state);
    shift_rows(state);
    create_round_key(expanded_key + n_rounds * 16, round_key);
    add_round_key(state, round_key);
}

char aes_encrypt(unsigned char* input, unsigned char* output,
                 unsigned char* key, enum KEY_SIZES key_size) {
    int expanded_key_size, n_rounds;
    unsigned char* expanded_key;
    unsigned char block[16] = {0};

    switch (key_size) {
        case SIZE_16:
            n_rounds = 10;
            break;
        case SIZE_24:
            n_rounds = 12;
            break;
        case SIZE_32:
            n_rounds = 14;
            break;
        default:
            return ERROR_AES_UNKNOWN_KEYSIZE;
            break;
    }

    expanded_key_size = (n_rounds + 1) * 16;
    expanded_key =
        (unsigned char*)malloc(expanded_key_size * sizeof(unsigned char));
    if (expanded_key == NULL) return ERROR_MEMORY_ALLOCATION_FAILED;

    /* Set the block values, for the block:
     * a0,0 a0,1 a0,2 a0,3
     * a1,0 a1,1 a1,2 a1,3
     * a2,0 a2,1 a2,2 a2,3
     * a3,0 a3,1 a3,2 a3,3
     * the mapping order is a0,0 a1,0 a2,0 a3,0 a0,1 a1,1 ... a2,3 a3,3
     */

    // Equivalent to Transpose
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) block[i + j * 4] = input[j + i * 4];

    expand_key(expanded_key, key, key_size, expanded_key_size);
    aes_backbone(block, expanded_key, n_rounds);

    // Equivalent to Transpose
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) output[j + i * 4] = block[i + j * 4];

    free(expanded_key);
    expanded_key = NULL;
    return SUCCESS;
}

// AES Decryption
//

void inv_substitute_bytes(unsigned char* state) {
    for (int i = 0; i < 16; i++) state[i] = _get_inv_sbox_value(state[i]);
}

void _inv_shift_row_once(unsigned char* state, int n_rows) {
    unsigned char c;
    for (int i = 0; i < n_rows; i++) {
        c = state[3];
        for (int j = 3; j > 0; j--) state[j] = state[j - 1];
        state[0] = c;
    }
}

void inv_shift_rows(unsigned char* state) {
    for (int i = 0; i < 4; i++) _inv_shift_row_once(state + i * 4, i);
}

void _inv_mix_column_once(unsigned char* col) {
    unsigned char cpy[4];
    for (int i = 0; i < 4; i++) cpy[i] = col[i];
    col[0] = galois_multi(cpy[0], 14) ^ galois_multi(cpy[3], 9) ^
             galois_multi(cpy[2], 13) ^ galois_multi(cpy[1], 11);
    col[1] = galois_multi(cpy[1], 14) ^ galois_multi(cpy[0], 9) ^
             galois_multi(cpy[3], 13) ^ galois_multi(cpy[2], 11);
    col[2] = galois_multi(cpy[2], 14) ^ galois_multi(cpy[1], 9) ^
             galois_multi(cpy[0], 13) ^ galois_multi(cpy[3], 11);
    col[3] = galois_multi(cpy[3], 14) ^ galois_multi(cpy[2], 9) ^
             galois_multi(cpy[1], 13) ^ galois_multi(cpy[0], 11);
}

void inv_mix_columns(unsigned char* state) {
    unsigned char cols[4] = {0};
    int i, j;

    // Similar to Matrix Multiplication
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) cols[j] = state[j * 4 + i];
        _inv_mix_column_once(cols);
        for (j = 0; j < 4; j++) state[j * 4 + i] = cols[j];
    }
}

void aes_inv_backbone(unsigned char* state, unsigned char* expanded_key,
                      int n_rounds) {
    unsigned char round_key[16];

    // Main Body
    create_round_key(expanded_key + 16 * n_rounds, round_key);
    add_round_key(state, round_key);
    for (int i = n_rounds - 1; i > 0; i--) {
        inv_shift_rows(state);
        inv_substitute_bytes(state);
        create_round_key(expanded_key + 16 * i, round_key);
        add_round_key(state, round_key);
        inv_mix_columns(state);
    }
    inv_shift_rows(state);
    inv_substitute_bytes(state);
    create_round_key(expanded_key, round_key);
    add_round_key(state, round_key);
}

char aes_decrypt(unsigned char* input, unsigned char* output,
                 unsigned char* key, enum KEY_SIZES key_size) {
    int expanded_key_size, n_rounds;
    unsigned char* expanded_key;
    unsigned char block[16] = {0};

    switch (key_size) {
        case SIZE_16:
            n_rounds = 10;
            break;
        case SIZE_24:
            n_rounds = 12;
            break;
        case SIZE_32:
            n_rounds = 14;
            break;
        default:
            return ERROR_AES_UNKNOWN_KEYSIZE;
            break;
    }

    expanded_key_size = (n_rounds + 1) * 16;
    expanded_key =
        (unsigned char*)malloc(expanded_key_size * sizeof(unsigned char));
    if (expanded_key == NULL) return ERROR_MEMORY_ALLOCATION_FAILED;

    /* Set the block values, for the block:
     * a0,0 a0,1 a0,2 a0,3
     * a1,0 a1,1 a1,2 a1,3
     * a2,0 a2,1 a2,2 a2,3
     * a3,0 a3,1 a3,2 a3,3
     * the mapping order is a0,0 a1,0 a2,0 a3,0 a0,1 a1,1 ... a2,3 a3,3
     */

    // Equivalent to Transpose
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) block[i + j * 4] = input[j + i * 4];

    expand_key(expanded_key, key, key_size, expanded_key_size);
    aes_inv_backbone(block, expanded_key, n_rounds);

    // Equivalent to Transpose
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) output[j + i * 4] = block[i + j * 4];

    free(expanded_key);
    expanded_key = NULL;
    return SUCCESS;
}