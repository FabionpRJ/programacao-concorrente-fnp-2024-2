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
 os deposita no buffer1 de tamanho N.

 2. a thread 2 processa os caracteres do buffer1 acrescentando um caracter de final de
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






void *t1(){//Leitor


}

void *t2(){//Processador

}
void *t3(){//Impressor



}

int main(){


    return 0;
}