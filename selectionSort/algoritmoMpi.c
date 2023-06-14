#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM 50000

int *geraVetor(int n){
    int* vet = (int*)malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++){
        vet[i] = rand() % 100;
    }
    return vet;
}

void mostraVetor(int *v, int n){
    for(int i = 0; i < n; i++){
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

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int* received_arr = (int*)malloc(local_n * sizeof(int));
            MPI_Recv(received_arr, local_n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int k = i * local_n;
            for (int j = 0; j < local_n; j++) {
                arr[k++] = received_arr[j];
            }

            free(received_arr);
        }

        for (int i = 0; i < n; i++) {
            int min_idx = i;
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }
            swap(&arr[i], &arr[min_idx]);
        }
    }
    else {
        MPI_Send(local_arr, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int *vet = NULL;
    vet = geraVetor(TAM);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* int arr[] = {5, 2, 8, 1, 3, 9, 6, 4, 7};
    int n = sizeof(arr) / sizeof(arr[0]); */

    double inicio = MPI_Wtime();

    selection_sort_parallel(vet, TAM, rank, size);

    double fim = MPI_Wtime();
    
    if (rank == 0) {
        printf("Tempo paralelo mpi: %f\n", fim - inicio);
        /* printf("Array ordenado: ");
        for (int i = 0; i < TAM; i++) {
            printf("%d ", vet[i]);
        }
        printf("\n"); */
    }

    MPI_Finalize();

    return 0;
}
