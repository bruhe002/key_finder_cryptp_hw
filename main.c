// Set Up libraries
#include <stdio.h>
#include <stdlib.h>


// Set up main function
int main() {

    // Upload input files
    FILE *pt_file = fopen("file", "r"); //Reading Plaintext
    FILE *ct_file = fopen("ciphertext.txt", "r"); //Reading Ciphertext
    FILE *iv_file = fopen("iv.txt", "r"); // Reading iv

    // Check if files exist
    if (pt_file == NULL || ct_file == NULL || iv_file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }

    char c = fgetc(ct_file);

    while(c != EOF) {
        printf("%c", c);
        c = fgetc(ct_file);
    }

    fclose(pt_file);
    fclose(ct_file);
    fclose(iv_file);
    return 0;
}