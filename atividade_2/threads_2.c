/*
*   ALUNO: Fábio do Nascimento Patão
*   DRE:122048211
*   
*   
*   De maneira semelhante ao lab, temos o produto interno sequencial
*   e o produto interno paralelo sendo executados no mesmo arquivo :)
*   
*   O arquivo de gerar vetor randomicamente realmente faz apenas isto.
*
**/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h> // Para usar o fabs()


// Variáveis globais
long int n; // Tamanho dos vetores
double *vet1, *vet2; // Vetores de elementos
int nthreads; // Número de threads

// Função executada pelas threads
void *ProdutoInterno(void *tid) {
    long int id = (long int) tid; // Identificador da thread
    int ini, fim, bloco; // Auxiliares para divisão do vetor em blocos
    double prod_interno, *ret; // Produto interno local
    
    bloco = n / nthreads; // Tamanho do bloco de dados de cada thread
    ini = id * bloco; // Posição inicial do vetor
    fim = ini + bloco; // Posição final do vetor
    if (id == (nthreads - 1)) fim = n; // A última thread trata os elementos restantes no caso de divisão não exata

    // Produto Interno
    prod_interno = 0;
    for (int i = ini; i < fim; i++) {
        prod_interno += vet1[i] * vet2[i];
    }

    // Retorna o resultado
    ret = malloc(sizeof(double));
    if (ret != NULL) *ret = prod_interno;
    else printf("--ERRO: malloc() thread\n");
    pthread_exit((void*) ret);
}

// Função principal do programa
int main(int argc, char *argv[]) {
    FILE *arq; // Arquivo de leitura dos vetores
    size_t ret; // Retorno da função fread
    
    double prod_interno_sequencial, prod_interno_paralelo;
    double *retorno_thread;
    double erro;

    pthread_t *tid_sistema; // Vetor de identificadores das threads no sistema

    // Valida e recebe os valores de entrada
    if (argc < 4) {
        printf("Use: %s <vetor 1> <vetor 2> <nº threads>\n", argv[0]);
        return 1;
    }

    // Abre o primeiro arquivo de entrada com os valores do vetor 1
    arq = fopen(argv[1], "rb");
    if (arq == NULL) { 
        printf("--ERRO: fopen() vetor 1\n"); 
        return 2; 
    }

    // Lê o tamanho dos vetores (primeira linha do arquivo)
    ret = fread(&n, sizeof(long int), 1, arq);
    if (ret != 1) {
        fprintf(stderr, "Erro de leitura do tamanho do vetor 1\n");
        return 3;
    }

    // Aloca espaço de memória e carrega o vetor 1 de entrada
    vet1 = malloc(sizeof(double) * n);
    if (vet1 == NULL) { 
        printf("--ERRO: malloc() vet1\n"); 
        return 4; 
    }
    ret = fread(vet1, sizeof(double), n, arq);
    if (ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 1\n");
        return 5;
    }
    fclose(arq);

    // Abre o segundo arquivo de entrada com os valores do vetor 2
    arq = fopen(argv[2], "rb");
    if (arq == NULL) { 
        printf("--ERRO: fopen() vetor 2\n"); 
        return 6; 
    }
    
    vet2 = malloc(sizeof(double) * n);
    if (vet2 == NULL) {
        printf("--ERRO: malloc() vet2\n");
        return 7;
    }
    ret = fread(vet2, sizeof(double), n, arq);
    if (ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 2\n");
        return 8;
    }
    fclose(arq);

    // Lê o número de threads da entrada do usuário 
    nthreads = atoi(argv[3]);

    // Aloca espaço para o vetor de identificadores das threads no sistema
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) { 
        printf("--ERRO: malloc() tid_sistema\n"); 
        return 9; 
    }

    // Produto Interno Sequencial
    prod_interno_sequencial = 0;
    for (int i = 0; i < n; i++) {
        prod_interno_sequencial += vet1[i] * vet2[i];
    }

    // Produto Interno Paralelo
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, ProdutoInterno, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); 
            return 10;
        }
    }

    // Espera todas as threads terminarem e calcula o produto interno total
    prod_interno_paralelo = 0;
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void **) &retorno_thread)) {
            printf("--ERRO: pthread_join()\n"); 
            return 11;
        }
        prod_interno_paralelo += *retorno_thread;
        free(retorno_thread);
    }

    // Cálculo do erro absoluto
    erro = fabs((prod_interno_sequencial - prod_interno_paralelo) / prod_interno_sequencial);

    // Imprime os resultados
    printf("\nProduto Interno Sequencial  = %.26f\n", prod_interno_sequencial);
    printf("Produto Interno Concorrente = %.26f\n", prod_interno_paralelo);
    printf("Erro = %.26f\n", erro);

    // Desaloca os espaços de memória
    free(vet1);
    free(vet2);
    free(tid_sistema);

    return 0;
}
