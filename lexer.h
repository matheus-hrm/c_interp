#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TOKEN_END = 0,
    TOKEN_SYMBOL,
    TOKEN_NUM,
    TOKEN_OP,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_BRACE_OPEN,
    TOKEN_BRACE_CLOSE,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_EQUAL,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_BANG,
    TOKEN_UNKNOWN,
} Token_Type;

typedef enum {
    NEW_TOKEN,
    COMPLETE_TOKEN,
    ERROR_TOKEN,
    END_TOKEN,
} TOKENIZER_STATE;

typedef struct {
    Token_Type type;
    char *value;
    size_t len;
    char *type_str;
} Token;

#endif // LEXER_H