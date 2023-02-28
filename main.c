// Set Up libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void encrypt(char *pt, int ptLen, char *ct, char *key, char *iv);
void exceptionHandler(void);

// Set up main function
int main() {

    // Upload input files
    FILE *pt_file = fopen("file", "r"); //Reading Plaintext
    FILE *ct_file = fopen("ciphertext.txt", "r"); //Reading Ciphertext
    FILE *iv_file = fopen("iv.txt", "r"); // Reading iv
    FILE *word_file = fopen("wordlist_edited.txt", "r"); // Reading possible keys

    // Check if files exist
    if (pt_file == NULL || ct_file == NULL || iv_file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }

    char word[128];
    char plaintext[128];
    char ciphertest[128];
    char ivStr[128];

    unsigned char ciphertext_result[128];
    int ciphertext_len;

    //Get plaintext string
    fgets(plaintext, 128, pt_file);
    printf("%d", (int)strlen(plaintext));
    // printf("%s\n", plaintext);
    //Get cipher test text
    fgets(ciphertest, 128, ct_file);

    // Get IV test
    fgets(ivStr, 128, iv_file);

    //Get key-word from word_file
    fgets(word, 16, word_file);
    // Pad each word with # if less than 16 characters
    while(word != EOF) {
        for(int i = 0; i < 16; i++){
            if(word[i] == '\n') {
                for(int j = i;j<16; j++) {
                    word[j] = '#';
                }
            }
            encrypt(plaintext, strlen(plaintext), ciphertext_result, word, ivStr);
        }
        // printf("%s\n", ciphertext_result);
        fgets(word, 128, word_file);
    }
    
    fclose(pt_file);
    fclose(ct_file);
    fclose(iv_file);
    fclose(word_file);
    return 0;
}

void exceptionHandler(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

void encrypt(char *pt, int ptLen, char *ct, char *key, char *iv) {
    // Create and initialize new cipher context
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    int len;

    int ctLen;
    
    // Test if the context exists
    if(ctx == NULL) {
        exceptionHandler();
    }

    // Initialize the encryption operation.
    if(EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
        exceptionHandler();
    }

    // Run the encryption Method
    if(EVP_EncryptUpdate(ctx, ct, &len, pt, ptLen) != 1) {
        exceptionHandler();
    }

    ctLen = len;

    // Finalise the encryption
    if(EVP_EncryptFinal_ex(ctx, ct + len, &len) != 1) {
        exceptionHandler();
    }
    ctLen += len;

    // Clean up context space
    EVP_CIPHER_CTX_free(ctx);
}