#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h> // só para usar o abs();

// Variáveis globais
// Tamanho dos vetores
long int n;
// Vetores de elementos
double *vet1;
double *vet2;
// Número de threads
int nthreads;

// Função executada pelas threads
// Estratégia de divisão de tarefas: blocos de n/nthreads elementos
void *ProdutoInterno(void *tid) {                                 /*OBS: Até aqui é uma alteração do algoritmo da prof*/
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
int main(int argc, char *argv[]) {     //Nossa main!! :)

   /*       EXPLICANDO VARIÁVEIS
      
      -FILE arq: 
            é o nosso arquivo de leitura dos vetores...
            Inicialmente, a ideia era implementar um arquivo 1 e um arquivo 2, 
            porém a tática de usar a mesma estrutura de dados é valida.
      -ret: 
         contém o número de elementos dos vetores, e via
         fread, podemos inserir os elementos nos ponteiros globais *vet1 e *vet2
      -doubles (nome):
         a) Valores dos cáclulos
         b) ponteiro do retorno
         c) erro
      
   */


   //FILE é o nosso arquivo de leitura dos vetores...
    FILE *arq;            //Inicialmente, havia escrito dois arquivos, mas posso apenas abrir e fechar os dois na mesma variável 
    size_t ret;            //ret contém o número de elementos dos vetores, bem como escreve os arquivos em nossas variáveis globais
	      
    double prod_interno_sequencial, prod_interno_paralelo;
    double *retorno_thread;
    double erro;

    pthread_t *tid_sistema; // Vetor de identificadores das threads no sistema

    // Valida e recebe os valores de entrada
    if (argc < 4) {
        printf("Use: %s <vetor 1> <vetor 2> <nº threads> \n", argv[0]);
        return 1;
    }

    // Abre o arquivo de entrada com os valores dos vetores
    arq1 = fopen(argv[1], "rb");
    if (arq == NULL) { 
        printf("--ERRO: fopen() vetor 1\n"); 
        return 2; 
    }

    // Lê o tamanho dos vetores (primeira linha do arquivo)
    ret = fread(&n, sizeof(long int), 1, arq);
    if (!ret) {
        fprintf(stderr, "Erro de leitura do tamanho do vetor 1\n");
        return 3;
    }

    // Aloca espaço de memória e carrega os vetores de entrada
    // Instanciando os dois vetores de uma vez para ter um if único.
    vet1 = malloc(sizeof(double) * n);
    
    if (vet1 == NULL) { 
        printf("--ERRO: malloc()\n"); 
        return 4; 
    }
    ret = fread(vet1, sizeof(double), n, arq);  // vet1 = arq1...
    fclose(arq);

    // Abre o segundo arquivo com o segundo vetor
    arq = fopen(argv[2], "rb");
    if (arq == NULL) { 
        printf("--ERRO: fopen() vetor 2\n"); 
        return 5; 
    }
    vet2 = malloc(sizeof(double) * n);
    if (vet2 == NULL){
      printf("--ERRO:malloc() vetor 2\n");
      return 6;
    }
    ret = fread(vet2, sizeof(double), n, arq);  // vet2 = arq2...
    if (ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 2\n");
        return 7;
    }
    
    fclose(arq);  // Fechamento do arquivo!

    // Lê o número de threads da entrada do usuário 
    nthreads = atoi(argv[3]);

    // Aloca espaço para o vetor de identificadores das threads no sistema
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) { 
        printf("--ERRO: malloc() tid\n"); 
        return 8; 
    }

   // Produto Interno Sequencial
    prod_interno_sequencial = 0;
    for (int i = 0; i < n; i++) {
        prod_interno_sequencial += vet1[i] * vet2[i];
    }

   //Produto Interno Paralelo

    // Cria as threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, ProdutoInterno, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); 
            return 9;
        }
    }
    // Espera todas as threads terminarem e calcula o produto interno total
    prod_interno_paralelo = 0;
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void **) &retorno_thread)) {
            printf("--ERRO: pthread_join()\n"); 
            return 10;
        }
        prod_interno_paralelo += *retorno_thread;
        free(retorno_thread);
    }

    erro = (prod_interno_sequencial - prod_interno_paralelo)/prod_interno_sequencial;
    erro = abs(erro);

    printf("\nProduto Interno Sequencial  = %.26f\n", prod_interno_sequencial);
    printf("Produto Interno Concorrente = %.26f\n", prod_interno_paralelo);
    printf("Erro = %.26f\n", erro);


    free(vet1);
    free(vet2);
    free(tid_sistema);

    return 0;
} 


/*
   Resultados encontrados:
      O Erro para vetores de dimensão n:
         100:     0.00000000000000000000000000
         1000:    0.00000000000000095503667569
         10000:   0.00000000000000142423512561
         100000:  0.00000000000000548350605117

 */
