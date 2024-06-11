#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void read_args(int argc, char **argv);
void repl_mode();
void file_mode(const char *filepath);
void check_keywords(const char *input);
char** tokenize(const char *input, size_t *token_count);
void free_tokens(char **tokens, size_t token_count);
ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream);

#endif // INTERPRETER_H
