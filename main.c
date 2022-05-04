#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <ctype.h>
#include <stdlib.h>
#include "cJSON.h"

#define MAX_INPUT_LEN 100
#define DATA_SIZE 10000
#define URL_BASE "https://api.dictionaryapi.dev/api/v2/entries/en/"

void print_json(cJSON *json);

int write_fn(char *ptr, size_t size, size_t nmemb, void *userdata);

bool all_alpha(char *input);

void read_input(char *input);

void process_arg(char *input, char *arg);

void load_url(char *url, char *buf);

int main(int argc, char **argv) {
    char input[MAX_INPUT_LEN];
    if (argc == 1) {
        read_input(input);
    } else if (argc == 2) {
        process_arg(input, argv[1]);
    } else {
        perror("Too many arguments!\n");
        exit(EXIT_FAILURE);
    }

    if (!all_alpha(input)) {
        perror("The input contained invalid characters!\n");
        exit(EXIT_FAILURE);
    }

    char url[sizeof(URL_BASE) + MAX_INPUT_LEN] = URL_BASE;
    strcat(url, input);

    char json_raw[DATA_SIZE];
    load_url(url, json_raw);

    cJSON *json = cJSON_Parse(json_raw);

    cJSON *word = cJSON_GetObjectItem(json->child, "word");
    if (word == NULL) {
        perror("Could not find that word!");
        exit(-2);
    } else {
	print_json(json->child);
    }

    cJSON_Delete(json);
}

void print_json(cJSON *json) {
	cJSON *phonetic = cJSON_GetObjectItem(json->child, "phonetic");
	printf("%s", phonetic->valuestring);
}

void read_input(char *input) {
    printf("Lookup word: ");
    fgets(input, MAX_INPUT_LEN, stdin);

    char *newline_ptr = strchr(input, '\n');
    if (newline_ptr == NULL) {
        perror("Input was too long!\n");
        exit(EXIT_FAILURE);
    } else {
        *newline_ptr = '\0';
    }
}

void process_arg(char *input, char *arg) {
    if (strlen(arg) > MAX_INPUT_LEN) {
        perror("Input was too long!\n");
        exit(EXIT_FAILURE);
    }
    // Copy the 1st argument into input
    strncpy(input, arg, MAX_INPUT_LEN);
}

void load_url(char *url, char *buf) {
    CURL *handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_fn);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, buf);

    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
}

int write_fn(char *ptr, size_t size, size_t nmemb, void *userdata) {
    memcpy(userdata, ptr, size * nmemb);
    return 0;
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
