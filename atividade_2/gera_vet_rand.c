/*
Programa auxiliar para gerar dois vetores de doubles 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 //valor máximo de um elemento do vetor
#define VECTOR_NUMBER 2 // número de vetores a serem criados
//descomentar o define abaixo caso deseje imprimir uma versão do vetor gerado no formato texto
//#define TEXTO 

int main(int argc, char *argv[]) {
   double *vetor_A, *vetor_B; // vetores que serão gerados
   long int n; // quantidade de elementos por vetor que serão gerados
   double elem; // valor gerado para incluir no vetor
   int fator = 1; // fator multiplicador para gerar números negativos

   FILE *descritorArquivo_A, *descritorArquivo_B; // descritores dos arquivos de saída
   size_t ret; // retorno da função de escrita no arquivo de saída

   // recebe os argumentos de entrada
   if (argc < 4) {
      fprintf(stderr, "Digite: %s <dimensao> <arquivo saida_1> <arquivo saida_2>\n", argv[0]);
      return 1;
   }
   n = atoi(argv[1]);

   // aloca memória para os vetores 
   vetor_A = (double*) malloc(sizeof(double) * n);
   vetor_B = (double*) malloc(sizeof(double) * n);
   if (!vetor_A || !vetor_B) {
      fprintf(stderr, "Erro de alocação da memória dos vetores\n");
      return 2;
   }

   // preenche os vetores com valores double aleatórios
   srand(time(NULL));
   for (long int i = 0; i < n; i++) {
      elem = (rand() % MAX) / 3.0 * fator; 
      vetor_A[i] = elem;
      fator *= -1;
   }
   for (long int i = 0; i < n; i++) {
      elem = (rand() % MAX) / 3.0 * fator; 
      vetor_B[i] = elem;
      fator *= -1;
   }

   // imprimir na saída padrão os vetores gerados
   #ifdef TEXTO
   fprintf(stdout, "%ld\n", n);
   for (long int i = 0; i < n; i++) {
      fprintf(stdout, "%f ", vetor_A[i]);
   }
   fprintf(stdout, "\n");

   for (long int i = 0; i < n; i++) {
      fprintf(stdout, "%f ", vetor_B[i]);
   }
   fprintf(stdout, "\n");
   #endif

   // abre os arquivos para escrita binária
   descritorArquivo_A = fopen(argv[2], "wb");
   if (!descritorArquivo_A) {
      fprintf(stderr, "Erro de abertura do arquivo de saída 1\n");
      return 3;
   }
   descritorArquivo_B = fopen(argv[3], "wb");
   if (!descritorArquivo_B) {
      fprintf(stderr, "Erro de abertura do arquivo de saída 2\n");
      return 3;
   }

   // escreve a dimensão e os elementos dos vetores nos arquivos
   ret = fwrite(&n, sizeof(long int), 1, descritorArquivo_A);
   ret = fwrite(vetor_A, sizeof(double), n, descritorArquivo_A);
   if (ret < n) {
      fprintf(stderr, "Erro de escrita no arquivo de saída 1\n");
      return 4;
   }

   ret = fwrite(&n, sizeof(long int), 1, descritorArquivo_B);
   ret = fwrite(vetor_B, sizeof(double), n, descritorArquivo_B);
   if (ret < n) {
      fprintf(stderr, "Erro de escrita no arquivo de saida 2\n");
      return 4;
   }

   // finaliza o uso das variáveis
   fclose(descritorArquivo_A);
   fclose(descritorArquivo_B);
   free(vetor_A);
   free(vetor_B);
   printf("")
   return 0;
}
