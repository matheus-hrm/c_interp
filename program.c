#include <stdio.h>

int main() {
    // Solicita ao usuário que insira alguns dados
    char nome[50];
    int idade;
    float altura;

    printf("Por favor, insira seu nome: ");
    scanf("%s", nome);

    printf("Por favor, insira sua idade: ");
    scanf("%d", &idade);

    printf("Por favor, insira sua altura em metros: ");
    scanf("%f", &altura);

    // Preenche alguns dados
    int ano_nascimento = 2022 - idade;

    // Realiza alguns cálculos
    float altura_em_polegadas = altura * 39.37;

    // Imprime os resultados
    printf("Olá %s, você nasceu em %d e sua altura em polegadas é %.2f\n", nome, ano_nascimento, altura_em_polegadas);

    return 0;
}