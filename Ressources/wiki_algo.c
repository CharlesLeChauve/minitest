/*The recursive descent, proposed by wikipedia and commented by chat GPT*/

#include <stdio.h>

// Define symbols representing different parts of the language.
typedef enum {
    IDENTIFIER, NUMBER, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, MULTIPLY,
    DIVIDE, PLUS, MINUS, EQUAL, NOT_EQUAL, LESS_THAN, LESS_EQUAL,
    GREATER_THAN, GREATER_EQUAL, CALL, BEGIN, SEMICOLON, END,
    IF, WHILE, ASSIGN, THEN, DO, CONST, COMMA, VAR, PROCEDURE, PERIOD, ODD
} Symbol;

// Global variable holding the current symbol from the input.
Symbol currentSymbol;

// Function prototypes.
void nextSymbol(void);
void error(const char message[]);

// Accept the current symbol if it matches the expected symbol and move to the next symbol.
int accept(Symbol expectedSymbol) {
    if (currentSymbol == expectedSymbol) {
        nextSymbol();
        return 1;
    }
    return 0;
}

// Expect the current symbol to be a specific symbol and move to the next symbol; otherwise, throw an error.
int expect(Symbol expectedSymbol) {
    if (accept(expectedSymbol))
        return 1;
    error("Unexpected symbol");
    return 0;
}

// Parse a factor in the grammar.
void factor(void) {
    if (accept(IDENTIFIER)) {
        // Do nothing if the factor is an identifier.
    } else if (accept(NUMBER)) {
        // Do nothing if the factor is a number.
    } else if (accept(LEFT_PARENTHESIS)) {
        // If the factor starts with a left parenthesis, parse the expression inside.
        expression();
        expect(RIGHT_PARENTHESIS);
    } else {
        // If none of the above, it's a syntax error.
        error("Syntax error in factor");
        nextSymbol();
    }
}

// Parse a term in the grammar.
void term(void) {
    factor();
    while (currentSymbol == MULTIPLY || currentSymbol == DIVIDE) {
        nextSymbol();
        factor();
    }
}

// Parse an expression in the grammar.
void expression(void) {
    if (currentSymbol == PLUS || currentSymbol == MINUS)
        nextSymbol();
    term();
    while (currentSymbol == PLUS || currentSymbol == MINUS) {
        nextSymbol();
        term();
    }
}

// Parse a condition in the grammar.
void condition(void) {
    if (accept(ODD)) {
        expression();
    } else {
        expression();
        if (currentSymbol == EQUAL || currentSymbol == NOT_EQUAL || currentSymbol == LESS_THAN || currentSymbol == LESS_EQUAL || currentSymbol == GREATER_THAN || currentSymbol == GREATER_EQUAL) {
            nextSymbol();
            expression();
        } else {
            error("Invalid operator in condition");
            nextSymbol();
        }
    }
}

// Parse a statement in the grammar.
void statement(void) {
    if (accept(IDENTIFIER)) {
        expect(ASSIGN);
        expression();
    } else if (accept(CALL)) {
        expect(IDENTIFIER);
    } else if (accept(BEGIN)) {
        do {
            statement();
        } while (accept(SEMICOLON));
        expect(END);
    } else if (accept(IF)) {
        condition();
        expect(THEN);
        statement();
    } else if (accept(WHILE)) {
        condition();
        expect(DO);
        statement();
    } else {
        error("Syntax error in statement");
        nextSymbol();
    }
}

// Parse a block in the grammar.
void block(void) {
    if (accept(CONST)) {
        do {
            expect(IDENTIFIER);
            expect(EQUAL);
            expect(NUMBER);
        } while (accept(COMMA));
        expect(SEMICOLON);
    }
    if (accept(VAR)) {
        do {
            expect(IDENTIFIER);
        } while (accept(COMMA));
        expect(SEMICOLON);
    }
    while (accept(PROCEDURE)) {
        expect(IDENTIFIER);
        expect(SEMICOLON);
        block();
        expect(SEMICOLON);
    }
    statement();
}

// Start parsing a program.
void program(void) {
    nextSymbol();
    block();
    expect(PERIOD);
}

// Note: Implementations of nextSymbol and error functions are required for completeness.
