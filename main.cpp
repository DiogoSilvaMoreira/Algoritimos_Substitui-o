#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas
typedef struct Process {
    int pid;            // ID do processo
    int size;           // Tamanho do processo
    int R;              // Bit R
    int M;              // Bit M
    int lastMUpdate;    // Tempo da última atualização do bit M
    int lastReference;  // Tempo da última referência
} Process;

typedef struct MemoryBlock {
    int start;                  // Endereço inicial
    int size;                   // Tamanho do bloco
    int free;                   // 1 se livre, 0 se ocupado
    Process *process;           // Ponteiro para o processo (se ocupado)
    struct MemoryBlock *next;   // Próximo bloco
    struct MemoryBlock *prev;   // Bloco anterior
} MemoryBlock;

// Variáveis globais
MemoryBlock *memory = NULL;     // Início da memória
int memorySize;                 // Tamanho da memória
int time = 0;                   // Tempo global
int processCount = 1;           // Contador de processos

void initializeMemory(int size) {
    memory = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    memory->start = 0;
    memory->size = size;
    memory->free = 1;
    memory->process = NULL;
    memory->next = NULL;
    memory->prev = NULL;
}

void printMemory() {
    MemoryBlock *current = memory;
    printf("\nEstado da Memória no tempo %d:\n", time);
    printf("Memória: \n");
    while (current) {
        if (current->free) {
            printf("Bloco livre: Início %d, Tamanho %d\n", current->start, current->size);
        } else {
            printf("Processo %d: Início %d, Tamanho %d, R=%d, M=%d\n",
                   current->process->pid, current->start, current->size,
                   current->process->R, current->process->M);
        }
        current = current->next;
    }
    current = memory;
    while (current) {
        if (current->free) {
            for (int i = 0; i < current->size; i++) {
                printf("[]");
            }
        } else {
            for (int i = 0; i < current->size; i++) {
                printf("[%d]", current->process->pid);
            }
        }
        current = current->next;
    }
    printf("\n\n");
}

void liberarProcesso(int pid) {
    MemoryBlock *current = memory;
    while (current) {
        if (!current->free && current->process->pid == pid) {
            // Libera o bloco
            current->free = 1;
            free(current->process);
            current->process = NULL;
            printf("Processo %d removido da memória no tempo %d.\n", pid, time);
            // Tenta fundir com o próximo bloco se estiver livre
            if (current->next && current->next->free) {
                MemoryBlock *temp = current->next;
                current->size += temp->size;
                current->next = temp->next;
                if (temp->next) {
                    temp->next->prev = current;
                }
                free(temp);
            }
            // Tenta fundir com o bloco anterior se estiver livre
            if (current->prev && current->prev->free) {
                MemoryBlock *temp = current;
                current->prev->size += current->size;
                current->prev->next = current->next;
                if (current->next) {
                    current->next->prev = current->prev;
                }
                free(current);
            }
            return;
        }
        current = current->next;
    }
    printf("Processo %d não encontrado na memória.\n", pid);
}

void resetRBits() {
    MemoryBlock *current = memory;
    while (current) {
        if (!current->free && current->process->R == 1) {
            current->process->R = 0;
            printf("Bit R do processo %d resetado para 0 no tempo %d.\n", current->process->pid, time);
        }
        current = current->next;
    }
}

void updateMBits() {
    MemoryBlock *current = memory;
    while (current) {
        if (!current->free && current->process->M == 1) {
            if (time - current->process->lastMUpdate >= 10) {
                current->process->M = 0;
                printf("Bit M do processo %d resetado para 0 no tempo %d.\n", current->process->pid, time);
            }
        }
        current = current->next;
    }
}

void limparBitsR() {
    printf("Limpando bits R de todos os processos no tempo %d...\n", time);
    resetRBits();
}

int aplicarNUR() {
    int candidatos[4] = { -1, -1, -1, -1 }; // Array para guardar IDs dos processos por classe
    MemoryBlock *current = memory;
    while (current) {
        if (!current->free) {
            int classe = (current->process->R << 1) | current->process->M; // Calcula a classe (0 a 3)
            if (candidatos[classe] == -1) {
                candidatos[classe] = current->process->pid; // Armazena o ID do processo
            }
        }
        current = current->next;
    }
    // Remover o processo da menor classe disponível
    for (int c = 0; c < 4; c++) {
        if (candidatos[c] != -1) { // Se encontrou um processo na classe
            liberarProcesso(candidatos[c]);
            printf("Processo %d removido (Classe %d) no tempo %d.\n", candidatos[c], c, time);
            return 1; // Sucesso na remoção
        }
    }
    // Se nenhuma classe disponível, limpar bits R e tentar novamente
    limparBitsR();
    // Reavalia após limpar bits R
    current = memory;
    memset(candidatos, -1, sizeof(candidatos)); // Reseta os candidatos
    while (current) {
        if (!current->free) {
            int classe = (current->process->R << 1) | current->process->M;
            if (candidatos[classe] == -1) {
                candidatos[classe] = current->process->pid;
            }
        }
        current = current->next;
    }
    for (int c = 0; c < 4; c++) {
        if (candidatos[c] != -1) {
            liberarProcesso(candidatos[c]);
            printf("Processo %d removido (Classe %d) no tempo %d após resetar bits R.\n", candidatos[c], c, time);
            return 1;
        }
    }
    printf("Erro: Não foi possível liberar memória usando o algoritmo NUR.\n");
    return 0;
}

void insertProcess() {
    Process *process = (Process *)malloc(sizeof(Process));
    process->pid = processCount++;
    process->R = 1;
    process->M = 1;
    process->lastMUpdate = time;
    process->lastReference = time;

    printf("Digite o tamanho do processo %d: ", process->pid);
    scanf("%d", &process->size);

    // Verifica se o tamanho do processo é maior que a memória total
    if (process->size > memorySize) {
        printf("Erro: O processo é muito grande para a memória disponível.\n");
        free(process);
        return;
    }

    while (1) {
        MemoryBlock *current = memory;
        // Tenta encontrar um bloco livre adequado
        while (current) {
            if (current->free && current->size >= process->size) {
                // Encontrou um bloco livre suficiente
                if (current->size > process->size) {
                    // Dividir o bloco
                    MemoryBlock *newBlock = (MemoryBlock *)malloc(sizeof(MemoryBlock));
                    newBlock->start = current->start + process->size;
                    newBlock->size = current->size - process->size;
                    newBlock->free = 1;
                    newBlock->process = NULL;
                    newBlock->next = current->next;
                    newBlock->prev = current;
                    if (newBlock->next) {
                        newBlock->next->prev = newBlock;
                    }
                    current->next = newBlock;
                }
                // Alocar o bloco atual
                current->size = process->size;
                current->free = 0;
                current->process = process;
                printf("Processo %d inserido na memória no tempo %d.\n", process->pid, time);
                return;
            }
            current = current->next;
        }
        // Se não encontrou bloco adequado, aplicar NUR
        if (!aplicarNUR()) {
            printf("Erro: Memória cheia, não foi possível alocar o processo %d.\n", process->pid);
            free(process);
            return;
        }
        // Após liberar espaço, tenta novamente
    }
}

void advanceTime(int units) {
    int targetTime = time + units;
    while (time < targetTime) {
        time++;
        // Ciclo de clock a cada 10 unidades de tempo
        if (time % 10 == 0) {
            printf("\nCiclo de clock ocorrido no tempo %d.\n", time);
            resetRBits();
        }
        updateMBits();
    }
    printf("Tempo avançado para %d.\n", time);
}

void referenceProcess() {
    int pid;
    char modify;
    printf("Digite o ID do processo a ser referenciado: ");
    scanf("%d", &pid);
    printf("Deseja modificar o bit M? (s/n): ");
    scanf(" %c", &modify);

    MemoryBlock *current = memory;
    int found = 0;
    while (current) {
        if (!current->free && current->process->pid == pid) {
            current->process->R = 1;
            current->process->lastReference = time;
            if (modify == 's' || modify == 'S') {
                current->process->M = 1;
                current->process->lastMUpdate = time;
            }
            printf("Processo %d referenciado%s no tempo %d.\n", pid, (modify == 's' || modify == 'S') ? " e modificado" : "", time);
            found = 1;
            break;
        }
        current = current->next;
    }
    if (!found) {
        printf("Processo %d não encontrado.\n", pid);
    }
}

int main() {
    printf("Digite o tamanho da memória: ");
    scanf("%d", &memorySize);
    initializeMemory(memorySize);

    int option;
    do {
        printf("\nMenu:\n");
        printf("1. Inserir Processo\n");
        printf("2. Mostrar Memória\n");
        printf("3. Avançar Ciclos de Clock\n");
        printf("4. Referenciar Processo\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                insertProcess();
                break;
            case 2:
                printMemory();
                break;
            case 3: {
                int units;
                printf("Quantas unidades de tempo deseja avançar? ");
                scanf("%d", &units);
                advanceTime(units);
                break;
            }
            case 4:
                referenceProcess();
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (option != 0);

    // Liberação da memória
    MemoryBlock *current = memory;
    while (current) {
        MemoryBlock *temp = current;
        current = current->next;
        if (!temp->free) {
            free(temp->process);
        }
        free(temp);
    }

    return 0;
}