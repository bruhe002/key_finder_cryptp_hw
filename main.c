// Set Up libraries
#include <stdio.h>
#include <stdlib.h>




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

    char word[16];
    char *c = 0;

    //Get word from word_file
    fgets(word, 16, word_file);

    while(word != EOF) {
        for(int i = 0; i < 16; i++){
            if(word[i] == '\n') {
                for(int j = i;j<16; j++) {
                    word[j] = '#';
                }
            }
            printf("%c", word[i]);
        }
        printf("\n");
        fgets(word, 16, word_file);
    }

    fclose(pt_file);
    fclose(ct_file);
    fclose(iv_file);
    fclose(word_file);
    return 0;
}