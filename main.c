#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_INPUT_LEN 100
#define URL_BASE "https://api.dictionaryapi.dev/api/v2/entries/en/"

int main(int argc, char **argv) {
    char input[MAX_INPUT_LEN];
    if (argc == 1) {
        printf("Lookup word: ");
        // Copy stdin into input (restricted by MAX_INPUT_LEN)
        fgets(input, MAX_INPUT_LEN, stdin);
        // Find '\n' char in input
        char *newline = strchr(input, '\n');
        // If '\n' doesn't appear, then there is remaining
        // data in stdin, that is relevant to the lookup input
        if (newline == NULL) {
            perror("Input was too long! Exiting...\n");
        } else {
            // Terminate at the newline
            *newline = '\0';
        }
    } else if (argc == 2) {
        if (strlen(argv[1]) > MAX_INPUT_LEN) {
            perror("Input was too long! Exiting...\n");
            return 1;
        }
        // Copy the 1st argument into input
        strncpy(input, argv[1], MAX_INPUT_LEN);
    } else {
        perror("Too many arguments! Exiting...\n");
        return 2;
    }
}
