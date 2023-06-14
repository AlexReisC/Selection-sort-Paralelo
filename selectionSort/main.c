#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>

#define MAX 1000

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
        for(j = i+1; j < MAX; j++){
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

    return 0;
}
