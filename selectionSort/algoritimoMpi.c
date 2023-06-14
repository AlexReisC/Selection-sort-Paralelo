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

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort_parallel(int arr[], int n, int rank, int size) {
    int local_n = n / size;
    int local_arr[local_n];
    
    MPI_Scatter(arr, local_n, MPI_INT, local_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < local_n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < local_n; j++) {
            if (local_arr[j] < local_arr[min_idx]) {
                min_idx = j;
            }
        }
        swap(&local_arr[i], &local_arr[min_idx]);
    }
    
    MPI_Gather(local_arr, local_n, MPI_INT, arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int *vetor = NULL;
    vetor = geraVetor(MAX);
    
    selection_sort_parallel(vetor, MAX, rank, size);
    
    if (rank == 0) {
        mostraVetor(vetor, MAX);
    }
    
    MPI_Finalize();
    
    return 0;
}
