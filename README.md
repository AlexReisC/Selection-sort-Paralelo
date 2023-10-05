# selection-Sort-Paralelo
Implementação paralela do algoritmo de ordenação Selection Sort, utilizando OpenMP e OpenMPI.
## Proposta
Este projeto foi feito para apresentação na disciplina de Programação Concorrente, a proposta era escolher um algoritmo qualquer (de ordenação ou não), paraleliza-lo com OpenMP e OpenMPI, calcular o desempenho em cada implementação (speedup e eficiência) para entradas a partir de 1 milhão de elementos, e então comparar os resultados com a implementação normal.
## O algoritmo e os resultados
Selection sort é um algoritmo bastante rápido e tem por complexidade big O(n²). Na tentativa de paraleliza-lo, apesr de dividir o vetor em multiplas threads, tive que implementar uma diretiva que impede as threads de verificar a condição de 'maior que' no vetor simultaneamente, para que não houvesse condição de corrida. Por este motivo a implementação com OpenMp se tornou mais lenta que o algoritmo normal. A implementação com OpenMPI também acabou se tornando mais lenta. 
