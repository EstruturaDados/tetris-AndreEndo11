#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// --- Estruturas ---

typedef struct {
    char nome; 
    int id;    
} Peca;

typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, total;
} Fila;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// --- Protótipos ---
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void enqueue(Fila *f, Peca p);
int dequeue(Fila *f, Peca *p);

void inicializarPilha(Pilha *p);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void push(Pilha *p, Peca pce);
int pop(Pilha *p, Peca *pce);

Peca gerarPeca(int *contadorId);
void mostrarEstado(Fila *f, Pilha *p);

// --- Main ---

int main() {
    Fila filaProximas;
    Pilha pilhaReserva;
    Peca pecaAux;
    int opcao;
    int proximoId = 0;

    srand(time(NULL));
    inicializarFila(&filaProximas);
    inicializarPilha(&pilhaReserva);

    // Preenchimento inicial da fila
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&filaProximas, gerarPeca(&proximoId));
    }

    do {
        mostrarEstado(&filaProximas, &pilhaReserva);
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (Fila)\n");
        printf("2 - Reservar peca (Fila -> Pilha)\n");
        printf("3 - Usar peca reservada (Pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            break;
        }

        switch (opcao) {
            case 1:
                if (dequeue(&filaProximas, &pecaAux)) {
                    printf("\n>>> VOCE JOGOU: [%c %d]\n", pecaAux.nome, pecaAux.id);
                    enqueue(&filaProximas, gerarPeca(&proximoId));
                }
                break;

            case 2:
                if (!pilhaCheia(&pilhaReserva)) {
                    if (dequeue(&filaProximas, &pecaAux)) {
                        push(&pilhaReserva, pecaAux);
                        printf("\n>>> [%c %d] MOVIDA PARA A RESERVA.\n", pecaAux.nome, pecaAux.id);
                        enqueue(&filaProximas, gerarPeca(&proximoId));
                    }
                } else {
                    printf("\nErro: Pilha de reserva cheia!\n");
                }
                break;

            case 3:
                if (pop(&pilhaReserva, &pecaAux)) {
                    printf("\n>>> VOCE USOU A RESERVA: [%c %d]\n", pecaAux.nome, pecaAux.id);
                } else {
                    printf("\nErro: Nao ha pecas na reserva.\n");
                }
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// --- Fila Circular ---

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) { return f->total == MAX_FILA; }
int filaVazia(Fila *f) { return f->total == 0; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Erro: fila cheia!\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

int dequeue(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("Erro: fila vazia!\n");
        return 0;
    }
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return 1;
}

// --- Pilha ---

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca pce) {
    if (pilhaCheia(p)) {
        printf("Erro: pilha cheia!\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = pce;
}

int pop(Pilha *p, Peca *pce) {
    if (pilhaVazia(p)) {
        return 0;
    }
    *pce = p->itens[p->topo]; 
    p->topo--;
    return 1;
}

// --- Auxiliares ---

Peca gerarPeca(int *contadorId) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = (*contadorId)++;
    return nova;
}

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n-------------------------------------------");
    printf("\nESTADO ATUAL:");
    
    printf("\nFila de Pecas:   ");
    int idx = f->inicio;
    for (int i = 0; i < f->total; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }

    printf("\nPilha Reserva:   ");
    if (pilhaVazia(p)) {
        printf("(Vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
        printf(" <- Topo");
    }

    printf("\n-------------------------------------------\n");
}
