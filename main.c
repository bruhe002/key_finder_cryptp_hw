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
    FILE *word_file = fopen("words.txt", "r"); // Reading possible keys

    // Initialize Text to be read
    unsigned char plaintext[] = "This is a top secret.";
    unsigned char ivStr[] =  {170, 187, 204, 221, 238, 255, 0, 153, 136, 119, 102, 85, 68, 51, 34, 17};
    unsigned char ciphertext[] = {0x76, 0x4a, 0xa2, 0x6b, 0x55, 0xa4, 0xda, 0x65, 0x4d, 0xf6, 0xb1, 
                                    0x9e, 0x4b, 0xce, 0x00, 0xf4, 0xed, 0x05, 0xe0, 0x93, 0x46, 0xfb, 
                                    0x0e, 0x76, 0x25, 0x83, 0xcb, 0x7d, 0xa2, 0xac, 0x93, 0xa2}; 


    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!!!!FOR SOME REASON THIS LINE OF CODE IS NECESSARY FOR TO GET RESULTS!!!!!
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    // const char* padding[15] = {"#", "##", "###", "####", "#####", "######", "#######", "########", "#########", "##########",
    //                                "###########", "############", "#############", "##############", "###############"};
    unsigned char key[16];

    // Parse through the words.txt file
    // Grab the words line by line
    while(fgets(key, 16, word_file) != NULL) {
        // Pad the keys with # character
        for(int i = 0; i <  16; i++) {
            if(key[i] == '\n') {
                for(int j = i; j < 16; j++) {
                    key[j] = '#';
                }
                key[16] = '\0'; // Add terminator character at the end
            };  
        } 

        // Make it look "hack-ey"
        printf("%s    %d      %s\n", key,  strlen(key), ivStr);
        
        // Pass the parameters to string, return one if a match is found
        int answer = encrypt(plaintext, ciphertext, 1, key, ivStr);
        
        // Display the key
        if(answer == 1) {
            printf("The key is: %s\n", key);
            break;
        }
    }
    
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

    // Set the key and IV
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    // Run encryption
    if(!EVP_EncryptUpdate(ctx, outputBuf, &outputLen, in, strlen(in))) {
        // Error
        EVP_CIPHER_CTX_free(ctx);
        printf("There was an error in update\n");
        return 0;
    }

    // Finalize Blocks
    if(!EVP_EncryptFinal_ex(ctx, outputBuf + outputLen, &tempLen)) {
        //Error
        EVP_CIPHER_CTX_free(ctx);
        printf("There was an error in final\n");
        return 0;
    }

    // Test expected result with actual result
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