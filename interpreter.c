#include "interpreter.h"
#include "token.h"

void read_args(int argc, char **argv)
{
    if (argc > 2)
    {
        printf("Usage: %s [file]\n", argv[0]);
        exit(1);
    }
    else if (argc == 2)
    {
        file_mode(argv[1]);
    }
    else
    {
        repl_mode();
    }
}

void repl_mode()
{
    printf("Entering REPL mode. Type 'exit' to quit.\n");
    char *input = NULL;
    size_t len = 0;
    size_t read;

    while (1)
    {
        printf("> ");
        read = custom_getline(&input, &len, stdin);
        if (read == -1)
        {
            free(input);
            break;
        }

        // Remove newline character
        if (input[read - 1] == '\n')
        {
            input[read - 1] = '\0';
        }

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        check_keywords(input, "REPL");
    }
}

void file_mode(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Error: Nao pode abrir o arquvo %s\n", filepath);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = malloc(length + 1);
    if (input == NULL)
    {
        printf("Error: Erro ao alocar memoria para o arquivo %s\n", filepath);
        fclose(file);
        exit(1);
    }

    fread(input, 1, length, file);
    input[length] = '\0';
    fclose(file);

    check_keywords(input, filepath);
    free(input);
}

void free_tokens(char **tokens, size_t token_count)
{
    for (size_t i = 0; i < token_count; ++i)
    {
        free(tokens[i]);
    }
    free(tokens);
}


int32_t custom_getline(char **lineptr, size_t *n, FILE *stream)
{
    if (*lineptr == NULL)
    {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL)
        {
            return -1;
        }
    }

    size_t i = 0;
    int c;

    while ((c = fgetc(stream)) != EOF)
    {
        if (i >= *n - 1)
        {
            *n *= 2;
            *lineptr = realloc(*lineptr, *n);
            if (*lineptr == NULL)
            {
                return -1;
            }
        }
        (*lineptr)[i++] = (char)c;
        if (c == '\n')
        {
            break;
        }
    }

    if (c == EOF && i == 0)
    {
        return -1;
    }

    (*lineptr)[i] = '\0';
    return i;
}