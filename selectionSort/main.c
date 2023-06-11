#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>

#define MAX 100

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

int main(){
    srand(time(NULL));
    int *vetor = NULL;
    vetor = geraVetor(MAX);

    int aux, menorIndice, i, j;

    // Implementacao serial
    double inicio_seq = omp_get_wtime();
    for(i = 0; i < MAX; i++){
        menorIndice = i;
        for(j = 1; j <= MAX; j++){
            if(vetor[j] < vetor[menorIndice]){
                menorIndice = j;
            }
        }
        aux = vetor[i];
        vetor[i] = vetor[menorIndice];
        vetor[menorIndice] = aux;
    }

    double fim_seq = omp_get_wtime();

    double tempo_seq = fim_seq - inicio_seq;
    printf("Tempo sequencial: %.5f\n", tempo_seq);

    // Implementacao com OpenMP
    double inicio_omp = omp_get_wtime();
    mostraVetor(vetor, MAX);
    #pragma omp parallel private(menorIndice) num_threads(4)
    {
        #pragma omp for
        for(i = 0; i < MAX; i++){
            menorIndice = i;
            for(j = i+1; j < MAX; j++){
                if(vetor[j] < vetor[menorIndice]){
                    menorIndice = j;
                }
            }
            aux = vetor[i];
            vetor[i] = vetor[menorIndice];
            vetor[menorIndice] = aux;
        }
    }
    mostraVetor(vetor, MAX);
    double fim_omp = omp_get_wtime();
    double tempo_omp = fim_omp - inicio_omp;
    printf("Tempo com OpenMp: %.5f \n", tempo_omp);

    // Implementacao com Mpi
    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    time_t t;
    srand(time(NULL)*rank);

    double inicio_mpi = MPI_Wtime();



    double fim_mpi = MPI_Wtime();

    double tempo_mpi = fim_mpi - inicio_mpi;
    printf("Tempo com MPI: %.5f", tempo_mpi);

    MPI_Finalize();

    // Analise de desempenho
    double speedup1 = tempo_seq / tempo_omp;
    double eficiencia1 = speedup1;
    double speedup2 = tempo_seq / tempo_mpi;
    double eficiencia2 = speedup2;

    printf("\n---------------");
    printf("Speedup entre serial e paralelo com OpenMp: %.5f", speedup1);
    printf("Eficiencia: %.5f", eficiencia1);

    printf("Speedup entre serial e paralelo com MPI: %.5f", speedup2);
    printf("Eficiencia: %.5f", eficiencia2);

    return 0;
}
