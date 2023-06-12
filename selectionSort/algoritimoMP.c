#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>

#define MAX 10

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
    mostraVetor(vetor, MAX);
    int i = 0, j, aux, menorIndice;

    // Implementacao com OpenMP
    double inicio_omp = omp_get_wtime();
    #pragma omp parallel private(menorIndice) num_threads(2)
    {
        #pragma omp for
        for(i = 0; i < MAX; i++){
            menorIndice = i;
            for(j = i+1; j < MAX; j++){
                #pragma omp atomic
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

    return 0;
}
