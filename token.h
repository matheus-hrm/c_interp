#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    TOKEN_NUMBER,
    TOKEN_OPEN_QUOTE,
    TOKEN_CLOSE_QUOTE,
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    uint16_t len;
    uint16_t line;
} Token;

void check_balance(char **tokens, uint32_t token_count, const char *filename);
void check_keywords(const char *input, const char *filename);
char **tokenize(const char *input, uint64_t *token_count);

#endif // TOKEN_H