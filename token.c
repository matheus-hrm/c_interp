#include "interpreter.h"
#include "token.h"

// List of C keywords
const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
    "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local", "#define", "#elif", "#else", "#endif",
    "#error", "#if", "#ifdef", "#ifndef", "#" , "include", "#line", "#pragma",
    "%s", "%d", "%f", "%c", "%p", "%ld", "%lu", "%lld", "%llu", "%x", "%X",
    "NULL", "true", "false", "%lf"
};

// List of C operators
const char* operators[] = {"==", "!=", ">=", "<=", "++", "--", "&&", "||", "<<", ">>"};

const uint64_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

char **tokenize(const char *input, uint64_t *token_count)
{
    size_t capacity = 10;
    char **tokens = malloc(capacity * sizeof(char *));
    *token_count = 0;

    size_t i = 0;
    while (input[i] != '\0')
    {
        while (isspace(input[i]))
            i++;

        if (input[i] == '\0')
            break;

        size_t start = i;

        if (isalpha(input[i]) || input[i] == '_')
        {
            while (isalnum(input[i]) || input[i] == '_')
                i++;
        }
        else if (isdigit(input[i]))
        {
            while (isdigit(input[i]))
                i++;
        }
        else if (input[i] == '\"')
        {
            i++;
            while (input[i] != '\"' && input[i] != '\0')
                i++;
            if (input[i] == '\"')
                i++;
        }
        else if (input[i] == '\'')
        {
            i++;
            while (input[i] != '\'' && input[i] != '\0')
                i++;
            if (input[i] == '\'')
                i++;
        }
        else
        {
            char c = input[i];
            if (strchr("(){}[];,", c))
            {
                i++;
            }
            else
            {
                // check if we found an operator
               int found = 0; 
               for (size_t j = 0; j < sizeof(operators) / sizeof(operators[0]); ++j)
               {
                   if (strncmp(input + i, operators[j], strlen(operators[j])) == 0)
                   {
                       i += strlen(operators[j]);
                       found = 1;
                       break;
                   }
               }
                if (!found)
                {
                     i++;
                } 
            }
        }

        size_t length = i - start;
        if (*token_count >= capacity)
        {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(char *));
        }
        tokens[*token_count] = strndup(input + start, length);
        (*token_count)++;
    }

    return tokens;
}
void check_keywords(const char *input, const char *filename)
{
    uint64_t token_count;
    char **tokens = tokenize(input, &token_count);

    for (size_t i = 0; i < token_count; ++i)
    {
        int is_keyword = 0;
        for (size_t j = 0; j < keyword_count; ++j)
        {
            if (strcmp(tokens[i], keywords[j]) == 0)
            {
                is_keyword = 1;
                break;
            }
        }

        if (!is_keyword)
        {
            if (isalpha(tokens[i][0]) || tokens[i][0] == '_')
            {
                // It's a valid identifier (variable name or function name)
                continue;
            }
            if (isdigit(tokens[i][0]))
            {
                // It's a number literal
                continue;
            }
            if ((tokens[i][0] == '\"' && tokens[i][strlen(tokens[i]) - 1] == '\"') ||
                (tokens[i][0] == '\'' && tokens[i][strlen(tokens[i]) - 1] == '\''))
            {
                // It's a string or character literal
                continue;
            }
            if (strchr(tokens[i], '\%') && strchr("diouxXeEfFgGaAcsCspn%", tokens[i][strlen(tokens[i]) - 1]))
            {
                // It's a format specifier
                continue;
            }
            // It's an operator or other symbol
            char *op[] = {
                "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=",
                "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "=", "+=", "-=", "*=", "/=", "%=",
                "&=", "|=", "^=", "<<=", ">>=", "->", ".", ",", ";", ":", "?", "(", ")", "[", "]", "{", "}"};
            size_t op_count = sizeof(op) / sizeof(op[0]);
            for (size_t j = 0; j < op_count; ++j)
            {
                if (strcmp(tokens[i], op[j]) == 0)
                {
                    is_keyword = 1;
                    break;
                }
            }

            if (!is_keyword)
            {
                printf("Atencao: Palavra ou simbolo nao identificado %s\n", tokens[i]);
            }
        }
    }

    // print others tokens found
   // for (size_t i = 0; i < token_count; ++i)
   // {
   //     printf("%s\n", tokens[i]);
   // }
    check_balance(tokens, token_count, filename);
    free_tokens(tokens, token_count);
}

void check_balance(char **tokens, uint32_t token_count, const char *filename)
{
    char stack[token_count];
    char line_stack[token_count];
    char column_stack[token_count];
    char line[1024];
    
    int32_t line_number = 0;
    int top = -1;
    
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro: Nao pode abrir o arquivo\n");
        return;
    }

    while(fgets(line, sizeof(line) ,file))
    {
        line_number++;
        int32_t column_number = 0;

        for ( uint32_t i = 0; line[i] != '\0'; i++)
        {
            column_number++;
            char c = line[i];

            if( c == '(' || c == '{' || c == '[')
            {
                stack[++top] = c;
                line_stack[top] = line_number;
                column_stack[top] = column_number;
            }
            else if (c == ')' || c == '}' || c == ']')
            {
                if (top == -1)
                {
                    printf("Erro: Fechamento sem abertura do caractere '%c' na linha %d, coluna %d\n", c, line_number, column_number);
                    fclose(file);
                    return;
                }

                char open = stack[top];
                int32_t open_line = line_stack[top];
                int32_t open_column = column_stack[top];
                top--;

                if ((open == '(' && c != ')') ||
                    (open == '{' && c != '}') ||
                    (open == '[' && c != ']'))
                {
                    printf("Erro: Caracteres nao identificados no bloco \n %c aberto na linha %d, col %d e %c linha %d, col %d\n", open, open_line, open_column, c, line_number, column_number);
                    fclose(file);
                    return;
                }
            }

        }
    }

    if (top != -1)
    {
        char open = stack[top];
        int32_t open_line = line_stack[top];
        int32_t open_column = column_stack[top];
        printf("Erro: Colchete de abertura não balanceado %c na linha %d, coluna %d\n", open, open_line, open_column);
    }
    
    fclose(file);
}