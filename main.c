#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_RELATOS 100
#define PI 3.141592653589793
#define RAIO_MAX_KM 10.0

typedef struct {
    char nome[50];
    char email[50];
    char tipo[30];
    char descricao[100];
    double latitude;
    double longitude;
} Relato;

Relato relatos[MAX_RELATOS];
int totalRelatos = 0;

// ========================
// Funções relacionadas a arquivo
// ========================

/**
 * Salva todos os relatos no arquivo "relatos.txt".
 */
void salvarArquivo() {
    FILE *f = fopen("relatos.txt", "w");
    if (!f) {
        printf("Erro ao salvar arquivo.\n");
        return;
    }
    for (int i = 0; i < totalRelatos; i++) {
        fprintf(f, "%s;%s;%s;%s;%.6f;%.6f\n",
                relatos[i].nome, relatos[i].email, relatos[i].tipo, relatos[i].descricao,
                relatos[i].latitude, relatos[i].longitude);
    }
    fclose(f);
}

/**
 * Carrega relatos do arquivo "relatos.txt" para a memória.
 */
void carregarArquivo() {
    FILE *f = fopen("relatos.txt", "r");
    if (!f) return;
    while (totalRelatos < MAX_RELATOS &&
           fscanf(f, " %49[^;];%49[^;];%29[^;];%99[^;];%lf;%lf\n",
                  relatos[totalRelatos].nome, relatos[totalRelatos].email,
                  relatos[totalRelatos].tipo, relatos[totalRelatos].descricao,
                  &relatos[totalRelatos].latitude, &relatos[totalRelatos].longitude) == 6) {
        totalRelatos++;
    }
    fclose(f);
}

// ========================
// Funções de validação e leitura
// ========================

/**
 * Remove o '\n' do final da string, se houver.
 */
void removerNovaLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

/**
 * Valida se a string não está vazia, não ultrapassa tamanho max e não é só espaços.
 */
int validarString(const char *str, int maxLen) {
    if (!str) return 0;
    int len = strlen(str);
    if (len == 0 || len > maxLen) return 0;
    for (int i = 0; i < len; i++) {
        if (!isspace((unsigned char)str[i])) return 1;
    }
    return 0;
}

/**
 * Valida se o email contém '@' e termina com ".com".
 */
int validarEmail(const char *email) {
    if (!email) return 0;
    const char *at = strchr(email, '@');
    if (!at) return 0;
    int len = strlen(email);
    return (len > 4 && strcmp(email + len - 4, ".com") == 0);
}

/**
 * Lê um double do stdin, com validação de intervalo.
 * Retorna 1 se sucesso, 0 se erro.
 */
int lerDouble(const char *prompt, double *valor, double min, double max) {
    char buffer[100], *endptr;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        *valor = strtod(buffer, &endptr);
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("Entrada inválida. Digite um número válido.\n");
            continue;
        }
        if (*valor < min || *valor > max) {
            printf("Valor deve estar entre %.2f e %.2f.\n", min, max);
            continue;
        }
        return 1;
    }
}

/**
 * Lê uma string do stdin, remove o '\n' e valida.
 * Retorna 1 se válido, 0 se inválido.
 */
int lerString(char *buffer, int tamanho, const char *prompt, int maxLen) {
    printf("%s", prompt);
    if (!fgets(buffer, tamanho, stdin)) return 0;
    removerNovaLinha(buffer);
    return validarString(buffer, maxLen);
}

// ========================
// Funções principais do programa
// ========================

/**
 * Calcula distância entre dois pontos (lat/lon) usando fórmula de Haversine.
 */
double calcularDistancia(double lat1, double lon1, double lat2, double lon2) {
    double rad = PI / 180.0;
    double dLat = (lat2 - lat1) * rad;
    double dLon = (lon2 - lon1) * rad;
    lat1 *= rad;
    lat2 *= rad;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
    return 6371 * 2 * atan2(sqrt(a), sqrt(1 - a));
}

/**
 * Cadastra um novo relato, lendo dados do usuário e validando.
 */
void cadastrarRelato() {
    if (totalRelatos >= MAX_RELATOS) {
        printf("Limite de relatos atingido.\n");
        return;
    }

    Relato r;

    if (!lerString(r.nome, sizeof(r.nome), "Nome (max 49 chars): ", 49)) {
        printf("Nome inválido.\n");
        return;
    }

    if (!lerString(r.email, sizeof(r.email), "Email (deve conter '@' e terminar com '.com'): ", 49) ||
        !validarEmail(r.email)) {
        printf("Email inválido.\n");
        return;
    }

    if (!lerString(r.tipo, sizeof(r.tipo), "Tipo de catástrofe (max 29 chars): ", 29)) {
        printf("Tipo inválido.\n");
        return;
    }

    if (!lerString(r.descricao, sizeof(r.descricao), "Descrição (max 99 chars): ", 99)) {
        printf("Descrição inválida.\n");
        return;
    }

    if (!lerDouble("Latitude (-90 a 90): ", &r.latitude, -90, 90) ||
        !lerDouble("Longitude (-180 a 180): ", &r.longitude, -180, 180)) {
        printf("Coordenadas inválidas.\n");
        return;
    }

    relatos[totalRelatos++] = r;
    salvarArquivo();
    printf("Relato cadastrado com sucesso!\n");
}

/**
 * Lista todos os relatos cadastrados.
 */
void listarRelatos() {
    if (totalRelatos == 0) {
        printf("Nenhum relato cadastrado.\n");
        return;
    }

    for (int i = 0; i < totalRelatos; i++) {
        printf("\n[%d] %s - %s\nTipo: %s\nDescrição: %s\nLatitude: %.6f | Longitude: %.6f\n",
               i + 1, relatos[i].nome, relatos[i].email, relatos[i].tipo,
               relatos[i].descricao, relatos[i].latitude, relatos[i].longitude);
    }
}

/**
 * Consulta relatos num raio de 10km a partir de uma coordenada dada pelo usuário.
 */
void consultarPorRaio() {
    if (totalRelatos == 0) {
        printf("Nenhum relato cadastrado.\n");
        return;
    }

    double lat, lon;
    if (!lerDouble("Digite sua latitude (-90 a 90): ", &lat, -90, 90) ||
        !lerDouble("Digite sua longitude (-180 a 180): ", &lon, -180, 180)) return;

    int encontrados = 0;
    printf("\nRelatos em até %.0f km:\n", RAIO_MAX_KM);
    for (int i = 0; i < totalRelatos; i++) {
        double dist = calcularDistancia(lat, lon, relatos[i].latitude, relatos[i].longitude);
        if (dist <= RAIO_MAX_KM) {
            printf("\n%s - %s\nTipo: %s\nDescrição: %s\nDistância: %.2f km\n",
                   relatos[i].nome, relatos[i].email, relatos[i].tipo,
                   relatos[i].descricao, dist);
            encontrados++;
        }
    }
    if (!encontrados) printf("Nenhum relato encontrado no raio especificado.\n");
}

/**
 * Função de comparação para qsort: ordena relatos por nome (alfabético).
 */
int compararNome(const void *a, const void *b) {
    return strcmp(((Relato *)a)->nome, ((Relato *)b)->nome);
}

/**
 * Ordena os relatos por nome usando qsort.
 */
void ordenarPorNome() {
    if (totalRelatos < 2) {
        printf("Não há relatos suficientes para ordenar.\n");
        return;
    }
    qsort(relatos, totalRelatos, sizeof(Relato), compararNome);
    salvarArquivo();
    printf("Relatos ordenados por nome.\n");
}

/**
 * Limpa o buffer do teclado para evitar problemas de entrada.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ========================
// Função principal
// ========================

int main() {
    carregarArquivo();
    int opcao;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Cadastrar relato\n");
        printf("2. Listar relatos\n");
        printf("3. Consultar relatos por raio de 10km\n");
        printf("4. Ordenar relatos por nome\n");
        printf("0. Sair\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Digite um número.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: cadastrarRelato(); break;
            case 2: listarRelatos(); break;
            case 3: consultarPorRaio(); break;
            case 4: ordenarPorNome(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}
