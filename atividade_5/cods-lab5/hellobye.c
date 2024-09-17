/* Disciplina: Programação Concorrente */
/* Profa.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */
/* Aluno: Fábio do Nascimento Patão */

/*
 *
 	Roteiro da atividade:
	 1. Abra o arquivo hellobye.c e identifique qual e o requisito lógico/condicional 
	 da aplicação ̃(qual e a ordem de impressão requerida para as expressões HELLO e BYEBYE). 
 	 2. Execute a aplicação ̧várias vezes e verifique se o requisito lógico  ́é sempre cumprido.
 *
 * */

// Reparei que a professora utiliza sempre /* como comentário. Utilizarei // somente para diferenciar
// Quando sou eu, o ALUNO, escrevendo. 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  3 // Número de threads fixa desta vez, sem alocação no prompt

/* Variaveis globais */
int estado = 0;        // estado?
pthread_mutex_t mutex; // criamos um lock genérico chamado "mutex"
pthread_cond_t cond;   // Nossa cond_t cond.
		       //
		       //	Agora, eu amo a ideia de associar essa variável cond_t cond como uma nova
		       //	fila, pois isto facilita a abstração

/* Thread A */
void *A (void *t) {

  printf("A: Comecei\n");	
  
  printf("HELLO\n");

  pthread_mutex_lock(&mutex);
// CRITICAL INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Apenas explicando como funciona isto, esta thread (chamaremos de manuel) ATIVOU a chave mutex.
// Isso implica que todas as outras threads com A MESMA CHAVE devem aguardar o manuel terminar de executar
// o trecho de código entre o Critical Init e o Critical Exit.
//
//
// Outras threads que nada tem a ver com esta chave, continuam executando normalmente.
//
// Em tese, também existe uma "Fila" de mutex...
// 
//
  estado++; // Nossa variável global estado
  if (estado==2) {  // verifica se estado é 2
      printf("A:  estado = %d, vai sinalizar a condicao \n", estado); 
      pthread_cond_signal(&cond); // desbloqueia 1 thread da fila de condição
  }
  pthread_mutex_unlock(&mutex); // Teoricamente um broadcast da fila relacionada ao mutex.
				// Pois agora toda a thread relacionada a esta chave pode executar 
				// a sessão crítica. Porém, ao mesmo tempo, apenas uma irá executar
				// após o lock... Logo, seria um signal sem ordem determinada?
				// Isto é, o signal possui ordem?
// CRITICAL EXIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) { //Thread principal
  printf("B: Comecei\n");

  pthread_mutex_lock(&mutex);
  //	CRITICAL INIT!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (estado<2) { //Se ao ser iniciado, o estado for menor que 2, ele será bloqueado.
     printf("B: estado = %d, vai se bloquear...\n", estado);
     pthread_cond_wait(&cond, &mutex); // BLOQUEIA
				       // Agora, note, ele foi bloqueado e possui um mutex associado
				       // a ele. O mutex é o mesmo do declarado anteriormente. Isto
				       // Ocorre pois o wait inicialmente dá um UNLOCK no mutex.
				       //
				       // ISTO É, as outras threads com essa chave na fila de espera
				       // podem executar. Porém, note que, após o signal ser enviado
				       // o wait terminará e o mutex estará novamente em estado de LOCK
				       //
     printf("B: sinal recebido e mutex realocado, estado = %d\n", estado);
  }

  printf("BYEBYE\n");
  
  pthread_mutex_unlock(&mutex); //Unlock final
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];

  /* Inicializa o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init (&cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, B, NULL);
  pthread_create(&threads[2], NULL, A, NULL);

  /* Espera todas as threads completarem */
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
