#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
    #define debug(...) fprintf(stderr, __VA_ARGS__)
#else
    #define debug(...) {}
#endif

unsigned long modulus = 256;
unsigned long multiplier = 1103515245;
unsigned long increment = 12345;

int byte_shuffle(unsigned char* block, unsigned char* keys, int size);

int read_key_block(unsigned char* keys, int size, unsigned char key);

int main(int argc, char** argv) {

    // User input
    if(argc != 4) {
        printf("usage: sbencrypt password ciphertext plaintext\n");
        return -1;
    }

    char* password = argv[1];
    char* ciphertext_name = argv[2];
    char* plaintext_name = argv[3];

    // Open files
    FILE *plaintext, *ciphertext;
    plaintext = fopen(plaintext_name, "r");
    ciphertext = fopen(ciphertext_name, "w");

    // Check files
    if(plaintext == NULL) {
        printf("error: Failed to open plaintext file: %s\n", plaintext_name);
        return -1;    
    }
    if(ciphertext == NULL) {
        printf("error: Failed to open ciphertext file: %s\n", ciphertext_name);
        return -1;
    }

    // Obtain seed as defined in docs
    unsigned long seed = 0;
    int c;
    while(c = *password++) {
        seed = c + (seed << 6) + (seed << 16) - seed;
    }
    printf("using seed=%ld from password=\"%s\"\n", seed, argv[1]);

    // Initial stream key using seed
    unsigned char key = (unsigned char) ((multiplier * seed + increment) % modulus);
    
    unsigned char plaintext_block[16], cipher_block[16], key_block[16], temp_block[16], previous_cipher_block[16];

    // Initialize block cipher as IV
    previous_cipher_block[0] = key;

    // Pass the last key into the function to generate the next key
    // Function returns the last key it generated
    // which we save for the next block
    key = read_key_block(&previous_cipher_block[1], 15, key);
    int count = 0;

    while(1) {
        // No more blocks to decrypt
        if(fread(cipher_block, 1, 16, ciphertext) == 0) {
            break;
        }

        // 1. temp_block = cipher_block ^ key_block
        key = read_key_block(key_block, 16, key);
        for(int i = 0; i < 16; i++) {
            temp_block[i] = cipher_block[i] ^ key_block[i];
        }

        // 2. Reverse the byte_shuffle
        byte_shuffle(temp_block, key_block, 16);

        // 3. Set plaintext_block = temp_block ^ previous_ciphertext_block
        for(int i = 0; i < 16; i++) {
            plaintext_block[i] = temp_block[i] ^ previous_cipher_block[i];
        }

        fwrite(plaintext_block, 1, 16, plaintext);

        // Pass current cipher_block for the next round
        for(int i = 0; i < 16; i++) {
            previous_cipher_block[i] = cipher_block[i];
        }

        count++; // Increment count for when we need to remove padding
    }

    // No leaks
    fclose(plaintext);
    fclose(ciphertext);

    // Still need to remove padding
    unsigned char padding_size = plaintext_block[15];

    // First, see which bytes have the same value as the potential padding byte
    while(padding_size <= 16 && padding_size > 0 && plaintext_block[15] == plaintext_block[16-padding_size]) {
        padding_size--;
    }

    if(padding_size == 0 && plaintext_block[15] != 0) {
        truncate(plaintext, 16 * count - plaintext_block[15]);
    }
    return 0;
}

int byte_shuffle(unsigned char* block, unsigned char* keys, int size) {
    unsigned char first, second, temp;

    // Must reverse the shuffling process for each block
    // Start from the last block and perform unshuffling
    for(int i = size - 1; i >= 0; i--) {
        first = keys[i] & 0xf;
        second = (keys[i] >> 4) & 0xf;
        temp = block[first];
        block[first] = block[second];
        block[second] = temp;
    }
    return size;
}

int read_key_block(unsigned char* keys, int size, unsigned char key) {
    for(int i = 0; i < size; i++) {
        key = (unsigned char)((multiplier * key + increment) % modulus);
        *(keys + i) = key;
    }
    return key;
}