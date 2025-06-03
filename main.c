// Sistema de Cadastro e Consulta de Relatos sobre Catástrofes Naturais
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// ---------------------- Constantes Globais ----------------------
#define PI 3.141592653589793 // Valor de PI para cálculo da distância
#define RAIO_MAX_KM 10.0      // Raio máximo para filtro de relatos por proximidade (em km)

// ---------------------- Structs ----------------------
typedef struct {
    char nome[50];        // Nome do usuário
    char email[50];       // Email do usuário
    char tipo[30];        // Tipo de catástrofe
    char descricao[100];  // Descrição do ocorrido
    double latitude;      // Latitude do local
    double longitude;     // Longitude do local
} Relato;

// ---------------------- Protótipos ----------------------
// Declaração das funções utilizadas
void salvarArquivo();
void carregarArquivo();
void removerNovaLinha(char *str);
int validarString(const char *str, int maxLen);
int validarEmail(const char *email);
int lerDouble(const char *prompt, double *valor, double min, double max);
int lerString(char *buffer, int tamanho, const char *prompt, int maxLen);
double calcularDistancia(double lat1, double lon1, double lat2, double lon2);
void cadastrarRelato();
void listarRelatos();
void consultarPorRaio();
void ordenarPorNome();
void limparBuffer();
int compararNome(const void *a, const void *b);

// ---------------------- Variáveis Dinâmicas ----------------------
Relato *relatos = NULL;        // Vetor dinâmico de relatos
int totalRelatos = 0;          // Total atual de relatos
int capacidadeRelatos = 0;     // Capacidade alocada do vetor

// ---------------------- Funções de Arquivo ----------------------
// Salva os relatos no arquivo texto
void salvarArquivo() {
    FILE *f = fopen("relatos.txt", "w");
    if (!f) {
        printf("Erro ao salvar arquivo.\n");
        return;
    }
    // Escreve cada relato em formato separado por ponto e vírgula
    for (int i = 0; i < totalRelatos; i++) {
        fprintf(f, "%s;%s;%s;%s;%.6f;%.6f\n",
                relatos[i].nome, relatos[i].email, relatos[i].tipo, relatos[i].descricao,
                relatos[i].latitude, relatos[i].longitude);
    }
    fclose(f);
}

// Carrega os relatos do arquivo texto (se existir)
void carregarArquivo() {
    FILE *f = fopen("relatos.txt", "r");
    if (!f) return; // Se o arquivo não existir, simplesmente retorna

    char linha[300];
    while (fgets(linha, sizeof(linha), f)) {
        // Aumenta capacidade do vetor, se necessário
        if (totalRelatos == capacidadeRelatos) {
            capacidadeRelatos = capacidadeRelatos == 0 ? 10 : capacidadeRelatos * 2;
            relatos = realloc(relatos, capacidadeRelatos * sizeof(Relato));
            if (!relatos) {
                printf("Erro de alocação.\n");
                fclose(f);
                exit(1);
            }
        }

        // Lê dados da linha do arquivo
        sscanf(linha, " %49[^;];%49[^;];%29[^;];%99[^;];%lf;%lf",
               relatos[totalRelatos].nome, relatos[totalRelatos].email,
               relatos[totalRelatos].tipo, relatos[totalRelatos].descricao,
               &relatos[totalRelatos].latitude, &relatos[totalRelatos].longitude);
        totalRelatos++;
    }

    fclose(f);
}

// ---------------------- Funções de Validação ----------------------
// Remove o caractere de nova linha (\n) da string
void removerNovaLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

// Valida se a string não é nula, não está vazia e tem tamanho adequado
int validarString(const char *str, int maxLen) {
    if (!str) return 0;
    int len = strlen(str);
    if (len == 0 || len > maxLen) return 0;
    for (int i = 0; i < len; i++) {
        if (!isspace((unsigned char)str[i])) return 1;
    }
    return 0;
}

// Valida email contendo "@" e terminando em ".com"
int validarEmail(const char *email) {
    if (!email) return 0;
    const char *at = strchr(email, '@');
    if (!at) return 0;
    int len = strlen(email);
    return (len > 4 && strcmp(email + len - 4, ".com") == 0);
}

// Lê valor double com verificação de faixa válida
int lerDouble(const char *prompt, double *valor, double min, double max) {
    char buffer[100], *endptr;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        *valor = strtod(buffer, &endptr);
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("Entrada inválida.\n");
            continue;
        }
        if (*valor < min || *valor > max) {
            printf("Valor deve estar entre %.2f e %.2f.\n", min, max);
            continue;
        }
        return 1;
    }
}

// Lê string com prompt e validação de tamanho e conteúdo
int lerString(char *buffer, int tamanho, const char *prompt, int maxLen) {
    printf("%s", prompt);
    if (!fgets(buffer, tamanho, stdin)) return 0;
    removerNovaLinha(buffer);
    return validarString(buffer, maxLen);
}

// ---------------------- Função de Distância ----------------------
// Calcula a distância entre dois pontos geográficos usando fórmula de Haversine
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

// ---------------------- Cadastro e Consulta ----------------------
// Cadastra um novo relato
void cadastrarRelato() {
    // Garante espaço no vetor
    if (totalRelatos == capacidadeRelatos) {
        capacidadeRelatos = capacidadeRelatos == 0 ? 10 : capacidadeRelatos * 2;
        relatos = realloc(relatos, capacidadeRelatos * sizeof(Relato));
        if (!relatos) {
            printf("Erro de alocação.\n");
            exit(1);
        }
    }

    Relato r;
    // Leitura e validação dos campos
    if (!lerString(r.nome, sizeof(r.nome), "Nome: ", 49)) return;
    if (!lerString(r.email, sizeof(r.email), "Email: ", 49) || !validarEmail(r.email)) return;
    if (!lerString(r.tipo, sizeof(r.tipo), "Tipo: ", 29)) return;
    if (!lerString(r.descricao, sizeof(r.descricao), "Descrição: ", 99)) return;
    if (!lerDouble("Latitude (-90 a 90): ", &r.latitude, -90, 90)) return;
    if (!lerDouble("Longitude (-180 a 180): ", &r.longitude, -180, 180)) return;

    // Adiciona ao vetor
    relatos[totalRelatos++] = r;
    salvarArquivo();
    printf("Relato salvo com sucesso!\n");
}

// Lista todos os relatos cadastrados
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

// Consulta relatos em um raio de até 10km
void consultarPorRaio() {
    if (totalRelatos == 0) {
        printf("Nenhum relato cadastrado.\n");
        return;
    }

    double lat, lon;
    if (!lerDouble("Digite sua latitude (-90 a 90): ", &lat, -90, 90)) return;
    if (!lerDouble("Digite sua longitude (-180 a 180): ", &lon, -180, 180)) return;

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

// Função de comparação usada na ordenação por nome
int compararNome(const void *a, const void *b) {
    return strcmp(((Relato *)a)->nome, ((Relato *)b)->nome);
}

// Ordena os relatos alfabeticamente pelo nome do usuário
void ordenarPorNome() {
    if (totalRelatos < 2) {
        printf("Poucos relatos para ordenar.\n");
        return;
    }
    qsort(relatos, totalRelatos, sizeof(Relato), compararNome);
    salvarArquivo();
    printf("Relatos ordenados por nome.\n");
}

// ---------------------- Auxiliar ----------------------
// Limpa caracteres residuais do buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------------------- Main ----------------------
int main() {
    carregarArquivo(); // Carrega os relatos ao iniciar o programa
    int opcao;

    do {
        // Menu principal
        printf("\n--- MENU ---\n");
        printf("1. Cadastrar relato\n");
        printf("2. Listar relatos\n");
        printf("3. Consultar relatos por raio de 10km\n");
        printf("4. Ordenar relatos por nome\n");
        printf("0. Sair\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        // Tratamento da opção escolhida
        switch (opcao) {
            case 1: cadastrarRelato(); break;
            case 2: listarRelatos(); break;
            case 3: consultarPorRaio(); break;
            case 4: ordenarPorNome(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    free(relatos); // Libera a memória alocada
    return 0;
}
