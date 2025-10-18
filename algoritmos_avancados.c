#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================
// Estrutura da Sala
// =====================
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
} Sala;

// =====================
// Função: criarSala()
// Cria uma sala dinamicamente e define seu nome
// =====================
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// =====================
// Função: explorarSalas()
// Permite a navegação interativa pelo mapa
// =====================
void explorarSalas(Sala *salaAtual) {
    char escolha;

    while (salaAtual != NULL) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        // Verifica se a sala é um nó-folha (sem saídas)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Você chegou ao fim do caminho nesta sala!\n");
            break;
        }

        printf("Deseja ir para (e) esquerda, (d) direita ou (s) sair? ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("Não há caminho à esquerda!\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita;
            } else {
                printf("Não há caminho à direita!\n");
            }
        } 
        else if (escolha == 's' || escolha == 'S') {
            printf("Você decidiu sair da exploração.\n");
            break;
        } 
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// =====================
// Função principal: main()
// Monta o mapa fixo e inicia a exploração
// =====================
int main() {
    // Criação automática das salas (árvore binária)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");
    Sala *escritorio = criarSala("Escritório");

    // Montagem da estrutura da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = porao;
    cozinha->direita = escritorio;

    // Início da exploração
    printf("=== Detective Quest ===\n");
    printf("Explore a mansão e encontre o culpado!\n");

    explorarSalas(hall);

    // Liberação da memória
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(porao);
    free(escritorio);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
