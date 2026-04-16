#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ===================== ESTRUTURAS =====================

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

// ===================== FILA =====================

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) { return f->total == MAX_FILA; }
int filaVazia(Fila *f) { return f->total == 0; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

int dequeue(Fila *f, Peca *p) {
    if (filaVazia(f)) return 0;

    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return 1;
}

// ===================== PILHA =====================

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca pce) {
    if (pilhaCheia(p)) return;

    p->topo++;
    p->itens[p->topo] = pce;
}

int pop(Pilha *p, Peca *pce) {
    if (pilhaVazia(p)) return 0;

    *pce = p->itens[p->topo];
    p->topo--;
    return 1;
}

// ===================== FUNÇÕES AUXILIARES =====================

Peca gerarPeca(int *contadorId) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = (*contadorId)++;
    return nova;
}

// Troca topo da pilha com frente da fila
void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nErro: Nao ha elementos suficientes!\n");
        return;
    }

    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\n>>> Troca realizada com sucesso!\n");
}

// Troca múltipla (3 elementos)
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->total < 3 || p->topo < 2) {
        printf("\nErro: Elementos insuficientes para troca multipla!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % MAX_FILA;
        int idxPilha = p->topo - i;

        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = temp;
    }

    printf("\n>>> Troca multipla realizada!\n");
}

// Exibir estado
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n========================================");
    printf("\nESTADO ATUAL:");

    printf("\nFila: ");
    int idx = f->inicio;
    for (int i = 0; i < f->total; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }

    printf("\nPilha (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(Vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }

    printf("\n========================================\n");
}

// ===================== MAIN =====================

int main() {
    Fila fila;
    Pilha pilha;
    Peca aux;
    int opcao;
    int id = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preencher fila
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(&id));
    }

    do {
        mostrarEstado(&fila, &pilha);

        printf("\n1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar reserva\n");
        printf("4 - Trocar topo (fila <-> pilha)\n");
        printf("5 - Troca multipla\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                if (dequeue(&fila, &aux)) {
                    printf("\n>>> Jogou [%c %d]\n", aux.nome, aux.id);
                    enqueue(&fila, gerarPeca(&id));
                }
                break;

            case 2:
                if (!pilhaCheia(&pilha)) {
                    if (dequeue(&fila, &aux)) {
                        push(&pilha, aux);
                        printf("\n>>> Reservou [%c %d]\n", aux.nome, aux.id);
                        enqueue(&fila, gerarPeca(&id));
                    }
                } else {
                    printf("\nErro: Pilha cheia!\n");
                }
                break;

            case 3:
                if (pop(&pilha, &aux)) {
                    printf("\n>>> Usou reserva [%c %d]\n", aux.nome, aux.id);
                } else {
                    printf("\nErro: Pilha vazia!\n");
                }
                break;

            case 4:
                trocarTopo(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
