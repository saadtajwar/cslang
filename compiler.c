#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
    Token curr;
    Token prev;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    fprintf(stderr, "line %d error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        //nothing
    } else {
        fprintf(stderr, "at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char* message) {
    erorrAt(&parser.prev, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.curr, message);
}

static void advance() {
    parser.prev = parser.curr;

    for (;;) {
        parser.curr = scanToken();
        if (parser.curr.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.curr.start);
    }
}

static void consume(TokenType expected, char* message) {
    if (parser.curr.type == expected) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");
    return !parser.hadError;
}