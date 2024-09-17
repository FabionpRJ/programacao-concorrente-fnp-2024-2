/*
    ALUNO: Fábio do Nascimento Patão
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <pthread.h>

float **A, **B; //Nossas Matrizes declaradas globalmente.

float **mult_mat_S(float A[][], float B[][], int linha_A, int coluna_B){    // SOLUÇÃO SEQUENCIAL

    /*
        As linhas de A irão operar com as colunas de B!
        A dimensão do Vetor C se dá por linha_A x Coluna_B
        Ex: A = 4x2
        B = 2x4
        C = 4x4;

        Essencialmente, os dois são o mesmo número, logo, substituirei as variáveis por N...

     */
    if(linha_A == coluna_B){
        int N = linha_A;    //Atribui um valor N qualquer
    }
    else{
        printf("ERRO -- Matrizes de dimensões incompatíveis");
        return 1;       
    }
    float C[N][N];
    float soma;
    for(int i = 0; i<N; i++){
        for(int j = 0; j < N; j++){
            soma = 0.0;
            for(int k = 0; k < N; k++){
                soma = soma + A[i][k]*B[k][j];
            }
            C[i][j] = soma;
        }
    }
    return C;
 }

 void *mult_mat_P(void *args){
    
 }
