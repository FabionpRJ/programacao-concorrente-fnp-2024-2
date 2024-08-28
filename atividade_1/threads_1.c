#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


/*
    IDEIA INICIAL: Incrementar em uma unidade cada elemento de um vetor dado.

    Implementação:
        Sobre os Vetores:
            1. Os vetores são preenchidos com valores aleatórios via função inicializarVetor()
            2. Antes de ir para as threads, os vetores são divididos de maneira justa para o número de threads.
        Sobre a função da thread:
            1. recebe uma estrutura t_args como argumento, que contém {id, *vetor, posição de ínicio, posição fim}
            2. Incrementa os elementos no intervalo
            3. pthread_exit()
        Sobre a verificação:
            1. Ela recebe o vetor não processado pelas threads e o vetor processado por elas.
            2. Ela 

*/

typedef struct {
    int idThread;
    int *vetor;
    int inicio;
    int fim;
} t_Args;



int *inicializarVetor(int tamanho) {    //Função de gerar vetores aleatórios
    int i;
    int *vetor = (int*)malloc(tamanho * sizeof(int));

    srand(time(NULL));

    for (i = 0; i < tamanho; i++) {
        vetor[i] = rand() % 10;
    }
    return vetor;
}

void printVetor(int* vetor, int tamanho) {  //Função auxiliar para imprimir o vetor
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

void *funcao(void* arg) {                   //Função da thread :)
    t_Args *args = (t_Args *) arg;

    for (int i = args->inicio; i < args->fim; i++) {
        args->vetor[i] += 1;
    }
    pthread_exit(NULL);
}

int funcao_verificar(int* vetor_inicial, int* vetor_threads, int tamanho){
    int i = 0;
    while(i<tamanho){
        vetor_inicial[i]++; //Incrementa o vetor inicial em 1
        if(vetor_inicial[i] != vetor_threads[i]){//Verifica se algo está diferente.
            return 1;       //Código de erro, 1 != 0
        }
        i++;
    }
    return 0;               //Se nada estiver diferente, retorna 0.

}

int main(int argc, char* argv[]) {          //Nossa main!
    int nthreads;
    int tamanho_vetor;

    if (argc < 3) {                         //Teste inicial do número de argumentos
        printf("ERRO -- Insira pelo menos 2 argumentos!!\n");
        return 1;
    }
    //Coleta de argumentos
    nthreads = atoi(argv[1]);               //Número de threads
    tamanho_vetor = atoi(argv[2]);          //Tamanho do vetor

    pthread_t tid_sistema[nthreads];        //Thread id, para alocar espaço mais tarde
    t_Args args[nthreads];                  //Um vetor de estruturas, para os argumentos de cada thread

    int *vetor = inicializarVetor(tamanho_vetor);           //Inicializando o Vetor
    int *vetor_inicial = vetor;                            //Salvando o *vetor em outra variável para verificar depois
    int elementos_por_thread = tamanho_vetor / nthreads;    // Divisão de elementos para a thread. Trataremos o resto depois

    printf("Vetor inicial:\n");                             //Print do Vetir inicial
    printVetor(vetor, tamanho_vetor);

    for (int i = 0; i < nthreads; i++) {

        //instânciando os argumentos

        args[i].idThread = i;                               //id
        args[i].vetor = vetor;                              //vetor
        args[i].inicio = i * elementos_por_thread;          //início. i vezes o número de elementos.
        if (i == nthreads - 1) {                            //Fim caso seja a ultima thread. É o restante do vetor.
            args[i].fim = tamanho_vetor;            
        } 
        else {                                              //Fim caso seja uma thread qualquer
            args[i].fim = (i + 1) * elementos_por_thread;

        }

        /*instânciando a thread*/

        if (pthread_create(&tid_sistema[i], NULL, funcao, (void*)&args[i])) {   //Criando a thread da maneira determinada
            printf("--ERRO: pthread_create()\n");
            return 2;
        }
    }

    for (int i = 0; i < nthreads; i++) {                                        //Aguardando as threads terminarem.
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() da thread %d\n", i);
        }
    }

    printf("Vetor apos soma:\n");
    printVetor(vetor, tamanho_vetor);

    if(funcao_verificar(vetor_inicial, vetor, tamanho_vetor)){          //Função verificar
        printf("ERRO -- HOUVE ERRO NA INCREMENTAÇÃO\n");
        free(vetor);
        free(vetor_inicial);
        return 1;
    }

    printf("SUCESSO -- INCREMENTACAO CORRETA!!\n");

    free(vetor);
    free(vetor_inicial);
    return 0;
}