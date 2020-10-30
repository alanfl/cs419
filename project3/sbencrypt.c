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

int read_plaintext_block(unsigned char* block, int size, FILE *file);

int read_key_block(unsigned char* keys, int size, unsigned char key);

int main(int argc, char** argv) {

    // User input
    if(argc != 4) {
        printf("usage: sbencrypt password plaintext ciphertext\n");
        return -1;
    }

    char* password = argv[1];
    char* plaintext_name = argv[2];
    char* ciphertext_name = argv[3];

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
    
    unsigned char plaintext_block[16], cipher_block[16], key_block[16], temp_block[16];

    // Initialize block cipher as IV
    cipher_block[0] = key;

    // Pass the last key into the function to generate the next key
    // Function returns the last key it generated
    // which we save for the next block
    key = read_key_block(&cipher_block[1], 15, key);
    int more = 1;

    while(more) {
        // Padding in block, must be the last black
        if(read_plaintext_block(plaintext_block, 16, plaintext) > 0) {
            more = 0;
        }

        // 1. temp_block = plain_block ^ previous_ciphertext_block
        for(int i = 0; i < 16; i++) {
            temp_block[i] = plaintext_block[i] ^ cipher_block[i];
        }

        // 2. Byte shuffle the temp_block using the key_block
        key = read_key_block(key_block, 16, key);
        byte_shuffle(temp_block, key_block, 16);

        // 3. Set ciphertext_block = temp_block ^ key_block
        for(int i = 0; i < 16; i++) {
            cipher_block[i] = temp_block[i] ^ key_block[i];
        }

        fwrite(cipher_block, 1, 16, ciphertext);
    }

    // No leaks
    fclose(plaintext);
    fclose(ciphertext);

    return 0;
}

int byte_shuffle(unsigned char* block, unsigned char* keys, int size) {
    unsigned char first, second, temp;
    for(int i = 0; i < size; i++) {
        first = keys[i] & 0xf;
        second = (keys[i] >> 4) & 0xf;
        temp = block[first];
        block[first] = block[second];
        block[second] = temp;
    }
    return size;
}

int read_plaintext_block(unsigned char* plaintext, int block_size, FILE *file) {
    int count = fread(plaintext, 1, block_size, file);
    
    // Determine if padding is needed
    if(count != block_size) {
        // Only pad if this is the last block
        int padding_size = block_size - count;
        // Pad with bytes with value = # of padding bytes
        for(int i = 0; i < padding_size; i++) {
            *(plaintext + count + i) = (unsigned char) padding_size;
        }
        return padding_size;
    }

    // If no padding was applied
    return 0;
}

int read_key_block(unsigned char* keys, int size, unsigned char key) {
    for(int i = 0; i < size; i++) {
        key = (unsigned char)((multiplier * key + increment) % modulus);
        *(keys + i) = key;
    }
    return key;
}