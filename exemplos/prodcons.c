
//
// Programa produtor consumidor
//
// O escalonador é não preemptivo, por isso, certas seções críticas não
// aparecem com a proteção de exclusão mútua.
//
// O programa fica em laço infinito de produção e consumo
//
// Disclamer: este programa foi desenvolvido para auxiliar no desenvolvimento
//            de testes para o micronúcleo. NÃO HÁ garantias de estar correto.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/support.h"
#include "../include/cthread.h"

#define	N	10
#define MAXPROD 100
#define TRUE	1

int	buffer[N];
int     MaxProd;
csem_t	vazio, cheio, mutex;

// Simula uma thread ficar bloqueada a espera por "passar um tempo".
// Na verdade, volta a apto e não avança a execução por i vezes.

void sleepao(void){
     int i = 0;
 
     i = rand()%5 + 1;
     for (; i<0; i--) cyield();
     return;
}

void *produtor(void *arg) {
   int in=0;

   while(TRUE) {
      sleepao();
      cwait(&vazio);
      cwait(&mutex);
      buffer[in] = (rand() + 1) % N;
      printf("prod %d...\n",buffer[in]);
      in= (in+1) % N;
      csignal(&mutex);
      csignal(&cheio);
   }
}

void *consumidor(void *arg) {
   int out=0;

   while(TRUE) { 
      sleepao();
      cwait(&cheio);
      cwait(&mutex);
      printf("cons %d...\n",buffer[out]);
      out = (out+1) % N;
      csignal(&mutex);
      csignal(&vazio);
   }
}

int main(int argc, char *argv[ ]) {
    int cons, prod;

    srand((unsigned)time(NULL));

    csem_init(&mutex, 1);
    csem_init(&vazio, N);
    csem_init(&cheio, 0);

    prod = ccreate((void *)produtor, (void *)NULL);
    cons = ccreate((void *)consumidor, (void *)NULL);

    cjoin(prod);
    cjoin(cons);
}




