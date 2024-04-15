
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    COMMAND, ARGUMENT, PIPE, REDIRECT_IN, REDIRECT_OUT, REDIRECT_APPEND, BACKGROUND, ENV_VAR, EOL, EOF_TOKEN, UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

#define BUFFER_SIZE 1024

// Prototypes de fonctions
Token nextToken(void);
void tokenizeInput(char *input);
void freeToken(Token *token);
void printToken(Token token);

char *currentChar;
Token currentToken;

// Fonction principale pour démonstration
int main() {
    char input[] = "cat file.txt | grep 'search' > output.txt";
    tokenizeInput(input);
    return 0;
}

// Tokenize l'entrée entière
void tokenizeInput(char *input) {
    currentChar = input;
    do {
        currentToken = nextToken();
        printToken(currentToken);
        freeToken(&currentToken);
    } while (currentToken.type != EOF_TOKEN && currentToken.type != EOL);
}

// Fonction pour obtenir le prochain token
Token nextToken(void) {
    while (isspace(*currentChar)) {
        currentChar++; // Ignorer les espaces
    }

    if (*currentChar == '\0') {
        return (Token){EOF_TOKEN, NULL};
    }

    Token token;
    int len = 0;
    char *start = currentChar;

    if (isalpha(*currentChar)) { // Début d'une commande ou d'un argument
        while (isalnum(*currentChar) || *currentChar == '_' || *currentChar == '-') {
            len++;
            currentChar++;
        }
        token.type = (start == input) ? COMMAND : ARGUMENT;
    } else if (*currentChar == '|') {
        token.type = PIPE;
        len = 1;
        currentChar++;
    } else if (*currentChar == '>') {
        currentChar++;
        if (*currentChar == '>') {
            token.type = REDIRECT_APPEND;
            len = 2;
            currentChar++;
        } else {
            token.type = REDIRECT_OUT;
            len = 1;
        }
    } else {
        token.type = UNKNOWN;
        len = 1;
        currentChar++;
    }

    token.value = malloc(len + 1);
    strncpy(token.value, start, len);
    token.value[len] = '\0';

    return token;
}

// Fonction pour libérer un token
void freeToken(Token *token) {
    if (token->value) {
        free(token->value);
        token->value = NULL;
    }
}

// Fonction pour imprimer un token (pour débogage)
void printToken(Token token) {
    printf("Type: %d, Value: '%s'\n", token.type, token.value ? token.value : "NULL");
}
