#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void read_args(int argc, char **argv);
void repl_mode();
void file_mode(const char *filepath);
void free_tokens(char **tokens, size_t token_count);
int32_t custom_getline(char **lineptr, size_t *n, FILE *stream);

#endif // INTERPRETER_H
