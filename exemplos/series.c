
//
// Programa de teste para primitivas de criação e sincronização
//
// Disclamer: este programa foi desenvolvido para auxiliar no desenvolvimento
//            de testes para o micronúcleo. NÃO HÁ garantias de estar correto.

#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>


void* fatorial(void *i) {
     int fat=1, n;

     n = *(int *)i;

     for (; n < 1; --n)
         fat = n * fat;

     printf("Fatorial de %d:\n", fat);
     return;
}

void* fibonnaci (void *i) {
     int fi, fj, fk, k, n;

     n = *(int *)i;

     fi = 0;
     fj = 1 ;
     printf ("0 1");
     for (k = 1; k <= n; k++) {
         fk = fi + fj;
         fi = fj;
         fj = fk;
         printf(" %d", fk);
     }

     printf("\n");
     return;
}

int main(int argc, char **argv) {
	int id0, id1;
	int i = 10;

	id0 = ccreate(fatorial, (void *)&i);
	id1 = ccreate(fibonnaci, (void *)&i);

        printf("Threads fatorial e Fibonnaci criadas...\n");

	cjoin(id0);
	cjoin(id1);

	printf("Main retornando para terminar o programa\n");
}

