#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
    #define debug(...) fprintf(stderr, __VA_ARGS__)
#else
    #define debug(...) {}
#endif

int main(int argc, char** argv) {

    // User input
    if(argc != 4) {
        printf("usage: scrypt password plaintext ciphertext\n");
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

    unsigned long modulus = 256;
    unsigned long multiplier = 1103515245;
    unsigned long increment = 12345;

    // Initial stream key using seed
    unsigned char key = (unsigned char) ((multiplier * seed + increment) % modulus);
    
    unsigned char plaintext_char, ciphertext_char;
    while(1) {
        // Plaintext done, end
        if(fread(&plaintext_char, 1, 1, plaintext) == 0) {
            break;
        }

        // Encrypt is simply xor
        ciphertext_char = (unsigned char) (plaintext_char ^ key);
        fwrite(&ciphertext_char, 1, 1, ciphertext); // Write out to file

        // Get the next stream from the current key
        key = (unsigned char) ((multiplier * key + increment) % modulus);
    }

    // No leaks
    fclose(plaintext);
    fclose(ciphertext);

    return 0;
}