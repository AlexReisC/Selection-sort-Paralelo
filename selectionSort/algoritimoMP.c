#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MAX 1000

int *geraVetor(int n){
    int* vet = (int*)malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++){
        vet[i] = rand();
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
    // Inicia a regiao paralela e torna privada para cada thread as variaveis i, j, menorIndice e aux
    #pragma omp parallel private(i, j, menorIndice, aux) num_threads(4)
    {
        // 'ordered' indica que em um ponto as threads trabalharam sequencialmente
        #pragma omp for ordered
        for (i = 0; i < n; i++) {
            menorIndice = i;
            for (j = i+1; j < n; j++){
                // Inicia a regiao sequencial
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
    printf("--------- Tempo paralelo: %f ----------", tempo);
}

int main() {
    srand(time(NULL));
    
    int *vetor = NULL;
    vetor = geraVetor(MAX);

    double inicio = omp_get_wtime();
    selectionSortParallel(vetor, MAX);
    double fim = omp_get_wtime();

    double t = fim - inicio;
    printf("\nTempo 2: %f", t);
    
    return 0;
}
