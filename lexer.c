#include "lexer.h"

int is_digit(char c)
{
    return c >= '0' && c <= '9' || c == '.';
}

char** lexer (char* input) 
{
    size_t capacity = 128;
    char** tokens = malloc(sizeof(char*) * capacity);
    size_t token_count = 0;

    TOKENIZER_STATE curr = NEW_TOKEN;
    TOKENIZER_STATE next = NEW_TOKEN;

    char curr_char = input[0]; 
    Token curr_token = {TOKEN_UNKNOWN, NULL, 0, "UNKNOWN"};
    
    while (curr_char != '/0')
    {
        switch(curr)
        {
            case NEW_TOKEN:
                if (is_digit(curr_char))
                {
                    curr_token.type = TOKEN_NUM;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                    
                }
                if (curr_char == '+' || curr_char == '-' || curr_char == '*')
                {
                    curr_token.type = TOKEN_OP;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                if (curr_char == '(')
                {
                    curr_token.type = TOKEN_PAREN_OPEN;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                if (curr_char == ')')
                {
                    curr_token.type = TOKEN_PAREN_CLOSE;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                if (curr_char == '{')
                {
                    curr_token.type = TOKEN_BRACE_OPEN;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                if (curr_char == '}')
                {
                    curr_token.type = TOKEN_BRACE_CLOSE;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                if (curr_char == ',')
                {
                    curr_token.type = TOKEN_COMMA;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                if (curr_char == ';')
                {
                    curr_token.type = TOKEN_SEMICOLON;
                    curr_token.value = malloc(sizeof(char) * capacity);
                    curr_token.value[curr_token.len++] = curr_char;
                }
                
        }
    }



}