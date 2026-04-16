#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5

// Estrutura que representa uma peça de Tetris
typedef struct {
    char nome; // Armazena a forma da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador numérico único
} Peca;

typedef struct {
    Peca itens[MAX]; // Array fixo para armazenar as peças
    int inicio;      // Índice de onde sai a próxima peça
    int fim;         // Índice de onde entra a nova peça
    int total;       // Contador para saber se a fila está cheia ou vazia
} Fila;

// --- Protótipos das Funções ---
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void inserir(Fila *f, Peca p);
void remover(Fila *f, Peca *p);
void mostrarFila(Fila *f);
Peca gerarPeca(int *contadorId);

int main() {
    Fila filaPeças;
    Peca pecaAux;
    int opcao;
    int proximoId = 0;

    // Inicializa a lista para geração aleatória e a fila
    srand(time(NULL));
    inicializarFila(&filaPeças);

    // Preenche a fila inicialmente conforme o exemplo (5 peças)
    for (int i = 0; i < MAX; i++) {
        inserir(&filaPeças, gerarPeca(&proximoId));
    }

    do {
        printf("\n=== TETRIS STACK: Fila de Pecas ===\n");
        mostrarFila(&filaPeças);

        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&filaPeças)) {
                    remover(&filaPeças, &pecaAux);
                    printf("\n>>> Voce jogou a peca [%c %d]!\n", pecaAux.nome, pecaAux.id);
                } else {
                    printf("\nErro: Nao ha pecas para jogar.\n");
                }
                break;

            case 2:
                if (!filaCheia(&filaPeças)) {
                    pecaAux = gerarPeca(&proximoId);
                    inserir(&filaPeças, pecaAux);
                    printf("\n>>> Peca [%c %d] adicionada a fila de espera.\n", pecaAux.nome, pecaAux.id);
                } else {
                    printf("\nErro: A fila de proximas pecas esta cheia.\n");
                }
                break;

            case 0:
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

// Insere uma peça no final da fila (Fila Circular)
void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->itens[f->fim] = p;
    // O operador % (módulo) faz o índice voltar para 0 ao chegar no MAX
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

// Remove a peça da frente da fila
void remover(Fila *f, Peca *p) {
    if (filaVazia(f)) return;

    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

// Exibe o estado visual da fila
void mostrarFila(Fila *f) {
    if (filaVazia(f)) {
        printf("[ Fila Vazia ]\n");
        return;
    }

    int idx = f->inicio;
    for (int i = 0; i < f->total; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX;
    }
    printf("\n");
}

// Gera uma peça aleatória conforme os tipos permitidos
Peca gerarPeca(int *contadorId) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = (*contadorId)++;
    return nova;
}
