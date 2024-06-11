#include "interpreter.h"


const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
    "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local"
};
const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

void read_args(int argc, char **argv) {
    if (argc > 2) {
        printf("Usage: %s [file]\n", argv[0]);
        exit(1);
    } else if (argc == 2) {
        file_mode(argv[1]);
    } else {
        repl_mode();
    }
}

void repl_mode() {
    printf("Entering REPL mode. Type 'exit' to quit.\n");
    char *input = NULL;
    size_t len = 0;
    ssize_t read;
    
    while (1) {
        printf("> ");
        read = custom_getline(&input, &len, stdin);
        if (read == -1) {
            free(input);
            break;
        }

        // Remove newline character
        if (input[read - 1] == '\n') {
            input[read - 1] = '\0';
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        check_keywords(input);
    }
}

void file_mode(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filepath);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = malloc(length + 1);
    if (input == NULL) {
        printf("Error: Could not allocate memory for file %s\n", filepath);
        fclose(file);
        exit(1);
    }

    fread(input, 1, length, file);
    input[length] = '\0';
    fclose(file);

    check_keywords(input);
    free(input);
}

void check_keywords(const char *input) {
    size_t token_count;
    char **tokens = tokenize(input, &token_count);

    for (size_t i = 0; i < token_count; ++i) {
        int is_keyword = 0;
        for (size_t j = 0; j < keyword_count; ++j) {
            if (strcmp(tokens[i], keywords[j]) == 0) {
                is_keyword = 1;
                break;
            }
        }

        if (!is_keyword) {
            if (isalpha(tokens[i][0]) || tokens[i][0] == '_') {
                // It's a valid identifier (variable name or function name)
                continue;
            }
            if (isdigit(tokens[i][0])) {
                // It's a number literal
                continue;
            }
            // It's an operator or other symbol
            const char *operators[] = {
                "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=",
                "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "=", "+=", "-=", "*=", "/=", "%=",
                "&=", "|=", "^=", "<<=", ">>=", "->", ".", ",", ";", ":", "?", "(", ")", "[", "]", "{", "}"
            };
            size_t operator_count = sizeof(operators) / sizeof(operators[0]);
            for (size_t j = 0; j < operator_count; ++j) {
                if (strcmp(tokens[i], operators[j]) == 0) {
                    is_keyword = 1;
                    break;
                }
            }

            if (!is_keyword) {
                printf("Warning: Potentially misspelled or unrecognized token: %s\n", tokens[i]);
            }
        }
    }

    free_tokens(tokens, token_count);
}

char** tokenize(const char *input, size_t *token_count) {
    size_t capacity = 10;
    char **tokens = malloc(capacity * sizeof(char*));
    *token_count = 0;

    size_t i = 0;
    while (input[i] != '\0') {
        while (isspace(input[i])) i++;
        
        if (input[i] == '\0') break;

        size_t start = i;

        if (isalpha(input[i]) || input[i] == '_') {
            while (isalnum(input[i]) || input[i] == '_') i++;
        } else if (isdigit(input[i])) {
            while (isdigit(input[i])) i++;
        } else {
            char c = input[i];
            if (strchr("(){}[];,", c)) {
                i++;
            } else {
                // Handle multi-character operators
                if ((c == '=' && input[i + 1] == '=') ||
                    (c == '!' && input[i + 1] == '=') ||
                    (c == '>' && input[i + 1] == '=') ||
                    (c == '<' && input[i + 1] == '=') ||
                    (c == '+' && input[i + 1] == '+') ||
                    (c == '-' && input[i + 1] == '-') ||
                    (c == '&' && input[i + 1] == '&') ||
                    (c == '|' && input[i + 1] == '|') ||
                    (c == '<' && input[i + 1] == '<') ||
                    (c == '>' && input[i + 1] == '>')) {
                    i += 2;
                } else if ((c == '+' && input[i + 1] == '=') ||
                           (c == '-' && input[i + 1] == '=') ||
                           (c == '*' && input[i + 1] == '=') ||
                           (c == '/' && input[i + 1] == '=') ||
                           (c == '%' && input[i + 1] == '=') ||
                           (c == '&' && input[i + 1] == '=') ||
                           (c == '|' && input[i + 1] == '=') ||
                           (c == '^' && input[i + 1] == '=') ||
                           (c == '<' && input[i + 1] == '<') ||
                           (c == '>' && input[i + 1] == '>') ||
                           (c == '-' && input[i + 1] == '>')) {
                    i += 2;
                } else {
                    i++;
                }
            }
        }

        size_t length = i - start;
        if (*token_count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(char*));
        }
        tokens[*token_count] = strndup(input + start, length);
        (*token_count)++;
    }

    return tokens;
}

void free_tokens(char **tokens, size_t token_count) {
    for (size_t i = 0; i < token_count; ++i) {
        free(tokens[i]);
    }
    free(tokens);
}

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }

    size_t i = 0;
    int c;

    while ((c = fgetc(stream)) != EOF) {
        if (i >= *n - 1) {
            *n *= 2;
            *lineptr = realloc(*lineptr, *n);
            if (*lineptr == NULL) {
                return -1;
            }
        }
        (*lineptr)[i++] = (char)c;
        if (c == '\n') {
            break;
        }
    }

    if (c == EOF && i == 0) {
        return -1;
    }

    (*lineptr)[i] = '\0';
    return i;
}