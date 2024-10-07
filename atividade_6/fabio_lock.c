#include <stdio.h>
#include <pthread.h>
#include "fabio_lock.h"

pthread_mutex_t mut_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t escritor = PTHREAD_COND_INITIALIZER;
pthread_cond_t leitor = PTHREAD_COND_INITIALIZER;
int escrevendo = 0, lendo = 0, quer_escrever = 0, quer_ler = 0;

/* Agora note, existe este Initializer para as variáveis mutex e cond


    É porque meu mutex estava dando um erro horrível, e tenebroso.
    Se você inicializa-se a função com mais de uma thread, tinha uma chance em não sei quantas
    de entrar em um loop infinito de "Quero ler!!"

    Era um verdadeiro pesadelo.
 */


void w_lock(pthread_mutex_t mutex_funcao){
    pthread_mutex_lock(mutex_funcao);   // Mais do que natural dar um lock no lock
    quer_escrever++;
    while(escrevendo != 0){
        printf("Quero Escrever!!\n");
        pthread_cond_wait(&escritor, mutex_funcao);
    }
    quer_escrever--;
    escrevendo++;
    pthread_mutex_unlock(mutex_funcao); // Mas é muito anti-natural um unlock no lock
}

void r_lock(pthread_mutex_t mutex_funcao){
    pthread_mutex_lock(mutex_funcao);   // E aqui o padrão se repete
    quer_ler++;
    while(escrevendo != 0 || quer_escrever != 0){
        printf("Quero ler!!!\n");   //Curioso saber que isto é muitíssimo raro ocorrer
        pthread_cond_wait(&leitor, mutex_funcao);
    }
    quer_ler--;
    lendo++;
    pthread_mutex_unlock(mutex_funcao); //Mas um unlock no lock
}

/*
    Uma arquitetura leitor/escritor é nada mais nada menos do que abandonar o bloqueio via lock
    e se concentrar em bloquear por meio de condicionais.
 */


void unlock(pthread_mutex_t mutex_funcao){  //Aqui a lógica é mais extensa

    /*Lembra-se do que escrevi em fabio_lock.h?
    
        Que inevitavelmente, pela maneira que o programa estava escrito, ou só teríamos escritores ou
        apenas leitores? pois bem, é assim que o unlock funciona.
    */
    pthread_mutex_lock(mutex_funcao);
    if(escrevendo > 0){
        escrevendo--;
        printf("Já escrevi!!\n");
        if(escrevendo == 0){
            printf("PODEM VOLTAR A LER!!\n");
            pthread_cond_broadcast(&leitor);
        }
    } else if(lendo > 0){
        lendo--;
        if(lendo == 0){
            //printf("PODEM VOLTAR A ESCREVER!!!!\n");
            pthread_cond_signal(&escritor);
        }
    }
    pthread_mutex_unlock(mutex_funcao);
}