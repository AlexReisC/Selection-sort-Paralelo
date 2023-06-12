#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX 10

int *geraVetor(int n) {
    int* vet = (int*)malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++) {
        vet[i] = rand() % 100;
    }
    return vet;
}

void mostraVetor(int *v, int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void troca(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSortParalelo(int *vetor, int n, int rank, int size) {
    int i, j, menorIndice;

    for(i = 0; i < n - 1; i++) {
        menorIndice = i;
        for(j = i + 1; j < n; j++) {
            if(vetor[j] < vetor[menorIndice]) {
                menorIndice = j;
            }
        }

        if(menorIndice != i) {
            troca(&vetor[i], &vetor[menorIndice]);
        }

        // Troca de informações entre os processos
        if(rank == i % size) {
            int localMenorIndice = menorIndice;
            // Envia o índice do menor elemento
            MPI_Bcast(&localMenorIndice, 1, MPI_INT, rank, MPI_COMM_WORLD);

            if(localMenorIndice != i) {
                // Envia o valor do menor elemento
                MPI_Send(&vetor[i], 1, MPI_INT, localMenorIndice % size, 0, MPI_COMM_WORLD);

                // Recebe o valor do menor elemento
                MPI_Recv(&vetor[i], 1, MPI_INT, localMenorIndice % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else if(rank == menorIndice % size) {
            int localMenorIndice;
            // Recebe o índice do menor elemento
            MPI_Bcast(&localMenorIndice, 1, MPI_INT, i % size, MPI_COMM_WORLD);

            if(localMenorIndice != i) {
                int menorElemento;
                // Recebe o valor do menor elemento
                MPI_Recv(&menorElemento, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Envia o valor do menor elemento
                MPI_Send(&vetor[menorIndice], 1, MPI_INT, i % size, 0, MPI_COMM_WORLD);

                vetor[menorIndice] = menorElemento;
            }
        }

        // Sincronização entre os processos
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *vetor = NULL;

    if(rank == 0) {
        vetor = geraVetor(MAX);
        mostraVetor(vetor, MAX);
    }

    int elementosPorProcesso = MAX / size;
    int *subVetor = (int*)malloc(sizeof(int)*elementosPorProcesso);

    MPI_Scatter(vetor, elementosPorProcesso, MPI_INT, subVetor, elementosPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    selectionSortParalelo(subVetor, elementosPorProcesso, rank, size);

    MPI_Gather(subVetor, elementosPorProcesso, MPI_INT, vetor, elementosPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        mostraVetor(vetor, MAX);
    }

    free(subVetor);
    if(rank == 0) {
        free(vetor);
    }

    MPI_Finalize();

    return 0;
}
