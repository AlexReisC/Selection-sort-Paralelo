#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

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

void selectionSortParallel(int vetor[], int n) {
    int i, j, menorIndice, aux;
    
    double inicio = omp_get_wtime();
    #pragma omp parallel private(i, j, menorIndice, aux) num_threads(4)
    {
        #pragma omp for ordered
        for (i = 0; i < n; i++) {
            menorIndice = i;
            for (j = i+1; j < n; j++){
                #pragma omp ordered
                {
                    if (vetor[j] < vetor[menorIndice])
                    {
                        menorIndice = j;
                    }
                }
                
            }
            // Troca os elementos
            if(menorIndice != i){
                aux = vetor[menorIndice];
                vetor[menorIndice] = vetor[i];
                vetor[i] = aux;
            }
        }
    }
    double fim = omp_get_wtime();
    double tempo = fim - inicio;
    printf("--------- Tempo: %f ----------", tempo);
}

int main() {
    srand(time(NULL));
    
    int *vetor = NULL;
    vetor = geraVetor(MAX);

    selectionSortParallel(vetor, MAX);

    return 0;
}
