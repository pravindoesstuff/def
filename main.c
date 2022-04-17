#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 100
#define DATA_SIZE 10000
#define URL_BASE "https://api.dictionaryapi.dev/api/v2/entries/en/"

write_fn(void *ptr, size_t size, size_t nmemb, void *stream);
bool all_alpha(char *input);

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
            perror("Input was too long!\n");
        } else {
            // Terminate at the newline
            *newline = '\0';
        }
    } else if (argc == 2) {
        if (strlen(argv[1]) > MAX_INPUT_LEN) {
            perror("Input was too long!\n");
            return 1;
        }
        // Copy the 1st argument into input
        strncpy(input, argv[1], MAX_INPUT_LEN);
    } else {
        perror("Too many arguments!\n");
        return 2;
    }

    if (!all_alpha(input)) {
        perror("The input contained invalid characters!\n");
        return 1;
    }

    // Build url
    char url[sizeof(URL_BASE) + MAX_INPUT_LEN] = URL_BASE;
    char data[DATA_SIZE];
    strcat(url, input);
    // Define handle for curl to use
    CURL *handle = curl_easy_init();
    // Add url to the handle
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_fn);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, data);

    // Perform the transfer on handle
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
}

/// Custom write function for curl (stolen from: https://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string)
write_fn(void *ptr, size_t size, size_t nmemb, void *stream){
    strncpy(stream, ptr, size*nmemb);
}


bool all_alpha(char *input) {
    while (*input != '\0') {
        if (!isalpha(*input)) {
            return false;
        }
        ++input;
    }
    return true;
}
