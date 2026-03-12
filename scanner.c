#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
    const char* start;
    const char* curr;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.curr = source;
    scanner.line = 1;
}

Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.curr;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();
    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(
                match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(
                match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return string();
    }
    
    return errorToken("Unexpected character.");
}

// NOTE: the token created below will have the length of the string itself WITHOUT
// the quotes - differing from b implementation
static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        char consumed = advance();
        if (consumed == '\n') {
            scanner.line++;
        }
    }

    if (isAtEnd()) {
        return errorToken("unterminated string");
    }

    Token strTok = makeToken(TOKEN_STRING);
    advance();
    return strTok;
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.curr[1];
}

static char peek() {
    return *scanner.curr;
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.curr != expected) return false;
    scanner.curr++;
    return true;
}

static char advance() {
    scanner.curr++;
    return scanner.curr[-1];
}

static bool isAtEnd() {
    return *scanner.curr == "\0";
}

static Token makeToken(TokenType tokenType) {
    Token token;
    token.type = tokenType;
    token.start = scanner.start;
    token.length = (int)(scanner.curr - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}