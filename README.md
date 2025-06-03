# Sistema de Cadastro e Consulta de Relatos sobre Catástrofes Naturais

## 📚 Disciplina
**Data Structures and Algorithms**

## 🎯 Objetivo do Projeto

Desenvolver uma interface de programação em linguagem C que permita o **cadastro, listagem, ordenação e consulta geográfica** de relatos sobre catástrofes naturais ocorridas em um raio de até 10 km. A aplicação deve registrar os dados pessoais do autor do relato e os detalhes da ocorrência, permitindo um controle eficaz e organizado das informações.

---

## 🧠 Conceitos Aplicados

- `struct` para representar o relato como entidade lógica
- `malloc`/`realloc` para permitir armazenamento dinâmico
- Ponteiros e manipulação de memória
- Validações robustas de string, email e coordenadas
- Cálculo da distância com a fórmula de Haversine
- Algoritmo de ordenação `qsort` por nome
- Operações de leitura/escrita com `fopen`, `fprintf`, `fscanf`
- Uso de `scanf`, `fgets`, `strtol`, `strtod`, `strcmp`, `strchr`
- Estruturas condicionais (`if/else`)
- Laços de repetição (`for`, `while`, `do-while`)

---

## 🧾 Estrutura de Dados

```c
typedef struct {
    char nome[50];
    char email[50];
    char tipo[30];
    char descricao[100];
    double latitude;
    double longitude;
} Relato;
```

---

## 🧱 Organização do Projeto

```
📁 Projeto
├── main.c          # Código-fonte principal
├── relatos.txt     # Arquivo gerado para salvar os relatos
└── README.md       # Documentação completa
```

---

## 🧪 Funcionalidades

### 1. Cadastrar Relato
- Coleta nome, email, tipo de catástrofe, descrição e coordenadas.
- Valida todos os campos, incluindo verificação de email e limites geográficos.
- Armazena em memória e salva no arquivo `relatos.txt`.

### 2. Listar Relatos
- Exibe todos os relatos cadastrados com formatação amigável.

### 3. Consultar por Raio de 10km
- Usuário insere sua posição atual (lat/long).
- Relatos próximos até 10 km são listados com cálculo de distância.

### 4. Ordenar por Nome
- Relatos são ordenados em ordem alfabética pelo nome do autor.

### 0. Sair
- Encerra o programa.

---

## 💾 Persistência

Os relatos são gravados no arquivo `relatos.txt` em formato:

```txt
nome;email;tipo;descricao;latitude;longitude
```

Ao iniciar o programa, o arquivo é carregado automaticamente.

---

## 🧮 Fórmula de Haversine

Utilizada para calcular a distância entre dois pontos da Terra com base nas coordenadas de latitude e longitude:

```c
double calcularDistancia(double lat1, double lon1, double lat2, double lon2);
```

---

## 🔒 Validações Implementadas

- **Nome/Tipo/Descrição:** não podem ser vazios ou compostos apenas por espaços.
- **Email:** deve conter `@` e terminar com `.com`.
- **Latitude:** valor entre -90 e 90.
- **Longitude:** valor entre -180 e 180.
- **Entradas numéricas:** verificação com `strtod`, rejeitando lixo ou letras.

---

## 📎 Exemplo de Uso

### Cadastro:

```
Nome (max 49 chars): Rafael Lima
Email (deve conter '@' e terminar com '.com'): rafael@email.com
Tipo de catástrofe (max 29 chars): Enchente
Descrição (max 99 chars): Ruas alagadas no centro
Latitude (-90 a 90): -23.55
Longitude (-180 a 180): -46.63
Relato cadastrado com sucesso!
```

### Consulta por Raio:

```
Digite sua latitude (-90 a 90): -23.55
Digite sua longitude (-180 a 180): -46.63

Relatos em até 10 km:
Rafael Silva - rafael@email.com
Tipo: Enchente
Descrição: Ruas alagadas no centro
Distância: 0.00 km
```

---

## 📌 Observações

- O código está estruturado em funções bem definidas, com responsabilidade única.
- Uso de memória dinâmica garante flexibilidade ao sistema.
- Comentários explicam a lógica de cada função e trecho relevante.
- O programa é totalmente funcional.

---

## 👨‍💻 Autor

**Rafael Do Nascimento Silva**  
Curso: Ciência da Computação - 1º Semestre  
Turma: 1CCPF
Instituição: FIAP
