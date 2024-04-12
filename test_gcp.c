#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define DELIMITERS " \t\n"

int main() {
    char *input = NULL;
    size_t input_size = 0;
    ssize_t bytes_read;
    char *token;

    // Lire la ligne de commande
    printf("shell$ ");
    bytes_read = getline(&input, &input_size, stdin);

    if (bytes_read <= 0) {
        free(input);
        return 1;
    }

    // Tokenization de la ligne
    token = strtok(input, DELIMITERS);
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, DELIMITERS);
    }

    free(input);
    return 0;
}