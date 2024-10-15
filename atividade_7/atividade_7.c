/*

/* Disciplina: Programacao Concorrente */
/* Aluno: Fábio do Nascimento Patão */

/*
 Atividade 3

 Objetivo: Projetar e implementar um programa concorrente em C para ler, manipular e
 imprimir na tela os caracteres de um arquivo texto. Use semáforos para tratar as demandas
 de sincronização. Acompanhe a explanacão da professora.
 Descricão: O programa deverá ser implementado com 3 threads:

 1. a thread 1 carrega os caracteres do arquivo de entrada em blocos de tamanho N e
 os deposita no buffer_1 de tamanho N.

 2. a thread 2 processa os caracteres do buffer_1 acrescentando um caracter de final de
 linha a cada 2n + 1 caracteres lidos (n = 0,1,2..10) e deposita os caracteres no
 buffer2. Depois de n atingir o valor 10, o caracter de final de linha deverá ser
 inserido a cada bloco de 10 caracteres lidos.

 3. a thread 3 imprime os caracteres do buffer2 na tela.

Importante:
 1. Os caracteres devem ser impressos na mesma ordem que foram escritos no arquivo
 de entrada.
 2. Gere arquivos de entrada para teste.
 3. Execute o programa várias vezes, alterando os parâmetros de entrada e ateste sua
 corretude.
 4. A organização e modularidade do código serão levadas em consideracão na
 avaliação do exercício.

*/



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3 
sem_t estado1,estado2; 
char buffer_1[257];   //Número qualquer pro buffer
char buffer_2[257];



void *t1(void *arg){//Leitor
    //printf("Thread: 1 está executando!...\n");
    fgets(buffer_1, 257, stdin);    //Lê o stdin
    //printf("Thread: 1 executou! Liberando sinal\n");
    sem_post(&estado1);
    
}

void *t2() { // Processador
    //printf("Thread: 2 está executando!...\n");

    int i = 0;  // contador que percorre o buffer_1
    int j = 0;  // contador que percorre o buffer_2
    int n = 0;  // variável para calcular o valor de 2n + 1
    int extensao = 0; // contador da extensao ja percorrida da nova linha

    sem_wait(&estado1);
    //printf("SINAL LIBERADO NA THREAD 2!!\n");

    while(buffer_1[i] != '\0') {
        buffer_2[j] = buffer_1[i];
        j++;
        i++;
        extensao++;

        if ((extensao == (2 * n + 1)) && (n<10)) {//Primeira condição de pulo
            buffer_2[j] = '\n';
            j++;
            n++;  
            extensao = 0; // Reinicia, nova linha
        }
        if((extensao == 10) && n>=10){//Segunda condição de pulo
            buffer_2[j] = '\n';
            j++;
            n++;  
            extensao = 0;
        }
    }

    buffer_2[j] = '\0';
    //printf("Thread: 2 executou! Liberando Sinal\n");
    sem_post(&estado2);
}


void *t3(){//Impressor
    //printf("Thread: 3 está executando!...\n");
    int i = 0;
    sem_wait(&estado2);
    while(buffer_2[i] != '\0'){
        printf("%c",buffer_2[i]);
        i++;
    }
    //printf("\nThread: 3 Executou!!\n");


}


int main(int argc, char *argv[]){
    pthread_t tid[NTHREADS];    //Cria 3 threads vazias

    //inicia semáforos
    sem_init(&estado1,0,0);
    sem_init(&estado2,0,0);

    


    //Instancia as threads
    if (pthread_create(&tid[0],NULL,t1,NULL)){//Curiosidade, a professora faz essa instânciação de trás pra frente
        printf("ERRO!! pthread_create()\n"); exit(-1);}
    if (pthread_create(&tid[1],NULL,t2,NULL)){//Ela instância na ordem t3->t1
        printf("ERRO!! pthread_create()\n"); exit(-1);}
    if (pthread_create(&tid[2],NULL,t3,NULL)){//Eu faço na ordem t1->t3
        printf("ERRO!! pthread_create()\n"); exit(-1);}

    for (int t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    return 0;
}