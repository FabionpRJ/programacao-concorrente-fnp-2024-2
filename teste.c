#include <stdio.h>
#include <stdlib.h>

int main(){
    int **vetor;
    int n = 3;
    int m = 3;

    // Alocar memória para o vetor
    vetor = (int **)malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++){
        vetor[i] = (int *)malloc(m * sizeof(int));
    }

    // Atribuir valores e imprimir
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            vetor[i][j] = i + j;
            printf("Sou o Vetor[%d][%d]: %d\n", i, j, vetor[i][j]);
        }
    }

    // Liberar memória
    for(int i = 0; i < n; i++){
        free(vetor[i]);
    }
    free(vetor);

    return 0;
}
