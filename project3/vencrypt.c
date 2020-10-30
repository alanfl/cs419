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
        printf("usage: vencrypt keyfile plaintext ciphertext\n");
        return -1;
    }

    char* keyfile_name = argv[1];
    char* plaintext_name = argv[2];
    char* ciphertext_name = argv[3];

    // Open files
    FILE *keyfile, *plaintext, *ciphertext;
    keyfile = fopen(keyfile_name, "r");
    plaintext = fopen(plaintext_name, "r");
    ciphertext = fopen(ciphertext_name, "w");

    // Check files
    if(keyfile == NULL) {
        printf("error: Failed to open keyfile file: %s\n", keyfile_name);
        return -1;
    }
    if(plaintext == NULL) {
        printf("error: Failed to open plaintext file: %s\n", plaintext_name);
    }
    if(ciphertext == NULL) {
        printf("error: Failed to open ciphertext file: %s\n", ciphertext_name);
    }

    // Read keyfile
    // Need buffer size, seek to end, then get pos
    fseek(keyfile, 0L, SEEK_END);
    long keyfile_size = ftell(keyfile);
    fseek(keyfile, 0L, SEEK_SET);   // Reset file!

    // Pass to buffer
    unsigned char* key = (unsigned char*) calloc(sizeof(unsigned char), keyfile_size);
    fread(key, 1, keyfile_size, keyfile);
    printf("keyfile=%s, length=%ld\n", keyfile_name, keyfile_size);
    
    unsigned char plaintext_char, ciphertext_char;
    long key_pos = 0L;
    while(1) {
        // Plaintext done, end
        if(fread(&plaintext_char, 1, 1, plaintext) != 1) {
            break;
        }

        // We can derive cipher text by the following:
        // (hex_value_of_plain) + (hex_value_of_key) % 256
        ciphertext_char = (unsigned char) (((int) plaintext_char + (int) key[key_pos]));
        fwrite(&ciphertext_char, 1, 1, ciphertext); // Write out to file

        // Move to next key, will repeat
        key_pos = (key_pos + 1) % keyfile_size;
    }

    // No leaks
    fclose(keyfile);
    fclose(plaintext);
    fclose(ciphertext);

    return 0;
}