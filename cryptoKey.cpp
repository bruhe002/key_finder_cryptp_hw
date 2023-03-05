// Set Up libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

int encrypt(unsigned char *pt, unsigned char *ct, int do_encrypt, unsigned char *keys, unsigned char *iv);
// int valueOf(char hexDigit);
char* hex2Ascii(char hex[]);
void ascii2Hex(char * in, char* out);


// Set up main function
int main() {

    // Upload input files
    // FILE *pt_file = fopen("file", "r"); //Reading Plaintext
    // FILE *ct_file = fopen("ciphertext.txt", "r"); //Reading Ciphertext
    // FILE *iv_file = fopen("iv.txt", "r"); // Reading iv
    FILE *word_file = fopen("words.txt", "r"); // Reading possible keys

    // FILE *result_ct;
    //FILE *result_ct_hex = fopen("result_ct.txt", "r");

    unsigned char expectedResult[1024];
    // fgets(expectedResult, 1024, ct_file);
    expectedResult[strlen(expectedResult) - 1] = '\0';
    // // Check if files exist
    // if (pt_file == NULL || ct_file == NULL || iv_file == NULL) {
    //     printf("Error! Could not open file\n");
    //     exit(-1);
    // }

    unsigned char plaintext[] = "This is top secret.";
    unsigned char ivStr[] =  {170, 187, 204, 221, 238, 255, 0, 153, 136, 119, 102, 85, 68, 51, 34, 17};
    unsigned char ciphertext[] = {0x76, 0x4a, 0xa2, 0x6b, 0x55, 0xa4, 0xda, 0x65, 0x4d, 0xf6, 0xb1, 
                                    0x9e, 0x4b, 0xce, 0x00, 0xf4, 0xed, 0x05, 0xe0, 0x93, 0x46, 0xfb, 
                                    0x0e, 0x76, 0x25, 0x83, 0xcb, 0x7d, 0xa2, 0xac, 0x93, 0xa2}; 

    const char* padding[15] = {"#", "##", "###", "####", "#####", "######", "#######", "########", "#########", "##########",
                                   "###########", "############", "#############", "##############", "###############"};

    // char* key = NULL;
    // size_t word_length = 0;
    //     ssize_t number_read;

    //     // Read each word from the file
    //     while((number_read = getline(&key, &word_length, file)) != -1) {
    //             // Remove the trailing newline
    //             key[number_read-1] = '\0';

    //             // Add the padding to the key if necessary
    //             if((number_read-1) < 16) {
    //                     int padding_index = 16-number_read;
    //                     key = strcat(key, padding[padding_index]);
    //             }

    // fgets(ivStr, 1024, iv_file);
    // fgets(plaintext, 1024, pt_file);
    printf("%s\n", ciphertext);
    printf("%s\n",ivStr);
    unsigned char key[16];
    size_t word_length = 0;
    while(getline(&key, &word_length, word_file) != 0) {
        // for(int i = 0; i <  16; i++) {
        //     if(word[i] == '\n') {
        //         for(int j = i; j < 16; j++) {
        //             word[j] = '#';
        //         }
        //         word[16] = '\0';
        //     };  
        // }
                    // Remove the trailing newline
                key[number_read-1] = '\0';

                // Add the padding to the key if necessary
                if((number_read-1) < 16) {
                        int padding_index = 16-number_read;
                        key = strcat(key, padding[padding_index]);
                }

        word[strlen(word) - 1] = '#';    
        printf("%s    %d\n", word,  strlen(word));
        
        int answer = encrypt(plaintext, ciphertext, 1, word, ivStr);
        
        if(answer == 1) {
            printf("%10dThe key is: %s\n", answer, word);
            break;
        }
    }
    
    // fclose(pt_file);
    // fclose(ct_file);
    // fclose(iv_file);
    fclose(word_file);
    return 0;
}

int encrypt(unsigned char *in, unsigned char *out, int do_encrypt, unsigned char *key, unsigned char *iv) {
    // Allow enough space in the output for additional block
    unsigned char outputBuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inputLen, outputLen, tempLen;
    
    // Create and initialize new cipher context
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    // Check Key and IV lengths
    // EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL);
    // OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
    // OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);

    // Set the key and IV
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    // printf("AM I IN AN INFINITE LOOP?");
    if(!EVP_EncryptUpdate(ctx, outputBuf, &outputLen, in, strlen(in))) {
        // Error
        EVP_CIPHER_CTX_free(ctx);
        printf("There was an error in update\n");
        return 0;
    }
    if(!EVP_EncryptFinal_ex(ctx, outputBuf + outputLen, &tempLen)) {
        //Error
        EVP_CIPHER_CTX_free(ctx);
        printf("There was an error in final\n");
        return 0;
    }
    // printf("%s   %s\n",outputBuf, out);
    if(memcmp(outputBuf, out, 32) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        printf("THE KEY WORKS!");
        return 1;
    }

    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

// char* hex2Ascii(char hex[]) {
//     int hexLength = strlen(hex);
//     printf("%d\n", hexLength);
//     char* result = NULL;
    
//     if(hexLength > 0) {
//         int symbolCount;
//         int oddHexCount = hexLength % 2 == 1;
//         if(oddHexCount) {
//             symbolCount = (hexLength / 2) + 1;
//         }
//         else {
//             symbolCount = hexLength / 2;
//         }
//         // printf("%d\n", symbolCount + 1);
//         result = malloc(symbolCount + 1);
//         int lastIdx = hexLength - 1;
//         for(int i = lastIdx; i >= 0; --i) {
//             // printf("%d\n", i);
//             // printf("%d\n", strlen(result));
//             if((lastIdx - i) % 2 != 0) {
//                 int baseTenVal = 16 * valueOf(hex[i]) + valueOf(hex[i + 1]);
//                 if(oddHexCount) {
//                     if(baseTenVal == 0) {
//                         result[i/2+1] = ' ';
//                     }
//                     else {
//                         result[i/2+1] = baseTenVal;
//                     }
                    
//                     // printf("%d, %c\n", baseTenVal, result[i/2+1]);
//                 }
//                 else {
//                     if(baseTenVal == 0) {
//                         result[i/2] = baseTenVal;
//                     }
//                     else {
//                         result[i/2] = baseTenVal;
//                     }
//                     // printf("%d, %c\n", baseTenVal, result[i/2]);
//                 }
//             }
//             else if(i == 0) {
//                 int dec = valueOf(hex[0]);
//                 result[0] = dec;
//                 // printf("%c\n", result[0]);
//             }
//         }
//         result[symbolCount] = '\0';
//         // printf(" Result size = %d\n", strlen(result));
//     }

//     return result;
// }

// void ascii2Hex(char *in, char* out) {
//     int loop = 0;
//     int idx = 0;
//     while(in[loop] != '\0') {
//         sprintf((char*)(out + idx), "%02x", in[loop]);
//         loop+=1;
//         idx+=2;
//     }

//     out[idx++] = '\0';
// }