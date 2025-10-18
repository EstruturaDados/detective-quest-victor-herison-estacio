#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// =======================
// Estruturas principais
// =======================

// Estrutura de uma sala (nó da árvore da mansão)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de um nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura da tabela hash (chave: pista, valor: suspeito)
typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;

// =======================
// Protótipos das funções
// =======================
Sala* criarSala(const char *nome);
void explorarSalas(Sala *raiz, PistaNode **raizPista, HashItem *tabelaHash[]);
void inserirPista(PistaNode **raiz, const char *pista);
void imprimirPistas(PistaNode *raiz);
void inicializarHash(HashItem *tabela[]);
int hashFunction(const char *chave);
void inserirNaHash(HashItem *tabela[], const char *pista, const char *suspeito);
const char* encontrarSuspeito(HashItem *tabela[], const char *pista);
void verificarSuspeitoFinal(HashItem *tabela[], PistaNode *raiz);

// =======================
// Função: criarSala()
// Cria uma sala dinamicamente e define seu nome
// =======================
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// =======================
// Função: inserirPista()
// Insere uma pista coletada na árvore BST de forma ordenada
// =======================
void inserirPista(PistaNode **raiz, const char *pista) {
    if (*raiz == NULL) {
        *raiz = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy((*raiz)->pista, pista);
        (*raiz)->esquerda = (*raiz)->direita = NULL;
    } else if (strcmp(pista, (*raiz)->pista) < 0) {
        inserirPista(&((*raiz)->esquerda), pista);
    } else if (strcmp(pista, (*raiz)->pista) > 0) {
        inserirPista(&((*raiz)->direita), pista);
    }
}

// =======================
// Função: imprimirPistas()
// Exibe todas as pistas coletadas em ordem
// =======================
void imprimirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        imprimirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        imprimirPistas(raiz->direita);
    }
}

// =======================
// Função: inicializarHash()
// Cria uma tabela hash vazia
// =======================
void inicializarHash(HashItem *tabela[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        tabela[i] = NULL;
    }
}

// =======================
// Função: hashFunction()
// Retorna o índice baseado na soma dos caracteres
// =======================
int hashFunction(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// =======================
// Função: inserirNaHash()
// Associa uma pista a um suspeito
// =======================
void inserirNaHash(HashItem *tabela[], const char *pista, const char *suspeito) {
    int indice = hashFunction(pista);
    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

// =======================
// Função: encontrarSuspeito()
// Consulta o suspeito de uma pista
// =======================
const char* encontrarSuspeito(HashItem *tabela[], const char *pista) {
    int indice = hashFunction(pista);
    HashItem *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return "Desconhecido";
}

// =======================
// Função auxiliar: pistaDaSala()
// Retorna a pista associada a cada cômodo
// =======================
const char* pistaDaSala(const char *nomeSala) {
    if (strcmp(nomeSala, "Hall de Entrada") == 0) return "Pegada de lama";
    if (strcmp(nomeSala, "Sala de Estar") == 0) return "Copo quebrado";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca suja";
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Livro rasgado";
    if (strcmp(nomeSala, "Jardim") == 0) return "Luva caída";
    if (strcmp(nomeSala, "Porão") == 0) return "Mancha de sangue";
    if (strcmp(nomeSala, "Escritório") == 0) return "Bilhete ameaçador";
    return NULL;
}

// =======================
// Função: explorarSalas()
// Permite a navegação e coleta de pistas
// =======================
void explorarSalas(Sala *raiz, PistaNode **raizPista, HashItem *tabela[]) {
    Sala *atual = raiz;
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Exibe e coleta pista
        const char *pista = pistaDaSala(atual->nome);
        if (pista != NULL) {
            printf("Você encontrou uma pista: '%s'\n", pista);
            inserirPista(raizPista, pista);
        }

        printf("Ir para (e) esquerda, (d) direita ou (s) sair: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há caminho à esquerda!\n");
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há caminho à direita!\n");
        } else if (escolha == 's' || escolha == 'S') {
            printf("\nEncerrando exploração...\n");
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }
}

// =======================
// Função: contarPistasDoSuspeito()
// Conta quantas pistas pertencem a um suspeito
// =======================
int contarPistasDoSuspeito(PistaNode *raiz, HashItem *tabela[], const char *suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    const char *sus = encontrarSuspeito(tabela, raiz->pista);
    if (strcmp(sus, suspeito) == 0)
        cont++;
    cont += contarPistasDoSuspeito(raiz->esquerda, tabela, suspeito);
    cont += contarPistasDoSuspeito(raiz->direita, tabela, suspeito);
    return cont;
}

// =======================
// Função: verificarSuspeitoFinal()
// Fase final: julgamento
// =======================
void verificarSuspeitoFinal(HashItem *tabela[], PistaNode *raiz) {
    char acusado[50];
    printf("\n=== FASE FINAL ===\n");
    printf("Pistas coletadas:\n");
    imprimirPistas(raiz);

    printf("\nQuem você acha que é o culpado? ");
    scanf(" %[^\n]", acusado);

    int qtd = contarPistasDoSuspeito(raiz, tabela, acusado);

    if (qtd >= 2) {
        printf("\nAcusação aceita! %s foi considerado CULPADO com %d pistas contra ele!\n", acusado, qtd);
    } else {
        printf("\nAcusação fraca... %s foi considerado INOCENTE. (%d pistas apenas)\n", acusado, qtd);
    }
}

// =======================
// Função principal
// =======================
int main() {
    // Criação do mapa da mansão
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");
    Sala *escritorio = criarSala("Escritório");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = porao;
    cozinha->direita = escritorio;

    // Inicializa tabela hash de suspeitos
    HashItem *tabelaHash[TAM_HASH];
    inicializarHash(tabelaHash);

    inserirNaHash(tabelaHash, "Pegada de lama", "Jardineiro");
    inserirNaHash(tabelaHash, "Copo quebrado", "Cozinheiro");
    inserirNaHash(tabelaHash, "Faca suja", "Cozinheiro");
    inserirNaHash(tabelaHash, "Livro rasgado", "Bibliotecária");
    inserirNaHash(tabelaHash, "Luva caída", "Jardineiro");
    inserirNaHash(tabelaHash, "Mancha de sangue", "Motorista");
    inserirNaHash(tabelaHash, "Bilhete ameaçador", "Secretária");

    // Inicializa árvore de pistas
    PistaNode *raizPista = NULL;

    printf("=== DETECTIVE QUEST - CAPÍTULO FINAL ===\n");
    explorarSalas(hall, &raizPista, tabelaHash);
    verificarSuspeitoFinal(tabelaHash, raizPista);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
