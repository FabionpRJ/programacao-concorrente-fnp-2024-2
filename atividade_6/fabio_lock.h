#include <stdio.h>
#include <pthread.h>



extern pthread_mutex_t mutex_funcao;
extern pthread_cond_t escritor, leitor;
extern int escrevendo, lendo, quer_escrever, quer_ler;

/* 
    Note o uso do extern.
    Eu estive com muitos erros para compilar o meu programa por questões de declaração de variável.
    O problema eram as múltiplas declarações de variáveis
*/

/*
 * 	A lógica do nosso programa será controlada por estas simples variáveis de controle
 *   	1. Escrevendo e Lendo:
 *    		Indica quantas threads estão atualmente em processo de leitura e de escrita.
 *     		Note que pela natureza do programa, sempre teremos Leitura = 0 e Escrita != 0
 *      		Ou Leitura != 0 e Escrita = 0 (Ou alguns leêm, ou alguém escreve)
 *       
 *      2. quer_escrever e quer_ler:
 *   		quer_ler é uma variável inútil para a proposta deste trabalho.
 *          	quer_escrever é uma variável que utilizaremos para FORÇAR PRIORIDADE DA ESCRITA
 *           	e evitar um starvation.
 *            
 */

void w_lock(pthread_mutex_t mutex_funcao);

void r_lock(pthread_mutex_t mutex_funcao);

void unlock(pthread_mutex_t mutex_funcao);
