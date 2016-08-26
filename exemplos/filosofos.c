//
// Philosophers.c: Este programa implementa um dos classicos de programação
//                 concorrente: a janta dos filósofos.
//                 O objetivo deste programa é testar a implementação do
//                 micro kernel desenvolvido na disciplina INF01142
//
// Primitivas testadas: ccreate, cjoin, cyield, cwait e csignal.
// 
// Este programa é basedo na solução de Tanenbaum apresentada no livro
// "Modern Operating System" (Prentice Hall International).
//
// Disclamer: este programa foi desenvolvido para auxiliar no desenvolvimento
//            de testes para o micronúcleo. NÃO HÁ garantias de estar correto.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/support.h"
#include "../include/cthread.h"


#define		SEED	             234
#define		MAXRAND            10000
#define		N		       5
#define		LEFT	i==0?N-1:(i-1)%N
#define		RIGHT	         (i+1)%N
#define		THINKING	       0
#define		HUNGRY		       1
#define		EATING		       2
#define		DONE		       3

int 		state[N], End[N]; 
csem_t	        mutex;
csem_t		s[N];
char		status[N*2]={'H',' ','H',' ',
                             'H',' ','H',' ',
                             'H','\0'};
                               
 
void sleepao(void){
     int i = 0;
 
     i = rand()%5 + 1;
     for (; i<0; i--) cyield();
     return;
}

void	test(int i) 
{
	if (state[i] == HUNGRY && state[LEFT] != EATING &&
	    state[RIGHT] != EATING) {
	    state[i] = EATING;
	    *(status+2*i) = 'E';
	    printf("%s \n", status);
	    csignal(&s[i]);
	    cyield();
            End[i]++;
	}
	return;
}


void	put_forks(int i) 
{
	cwait(&mutex);
	state[i] = THINKING;
	*(status+2*i) = 'T';
	printf("%s\n", status);
	test(LEFT);
	test(RIGHT);
	csignal(&mutex);
	cyield();       /* If scheduling is FIFO without preemption */
	                /* We allow another philosopher to run      */	
	return;
}

 
void	take_forks(int i) 
{
	cwait(&mutex);
	state[i] = HUNGRY;
	*(status+2*i) = 'H';
	printf("%s \n", status);	
	test(i);
	csignal(&mutex);
	cwait(&s[i]); 	
	return;	
}

 
void	think_eat(void) 
{
	sleepao();
	return;		
}


 
void *Philosophers(void *arg) {
	int i;
	
	i= (int)arg;
		
	while (End[i]<10) {
		think_eat();        /* Philosophe goes to think          */
		take_forks(i);      /* acquire two forks or blocks       */
		think_eat();        /* Philosophe goes to eat            */
		put_forks(i);       /* put back the forks                */	
		End[i]=End[i]+1;	
	}

	*(status+2*i)='D';
	state[i] = DONE;
	return;
}


//*************************** MAIN ***********************
 
int	main(int argc, char *argv[]) {
	int 	ThreadId[N];
	int	i,ret;
        	
        srand((unsigned)time(NULL));

        csem_init(&mutex, 1);
	
	for(i = 0; i < N; i++) 
	   if (csem_init(&s[i], 0)) {
	      printf("# Semaphore initialization error...\n");
	      exit(0);
	   }

	for(i = 0; i < N; i++) {
	   if (ThreadId[i] = ccreate(Philosophers, (void *)i)) {
	      exit(0);
	   }
	}

        printf("#\n# The dinner will begin...\n");

	for(i = 0; i < N; i++)
	   ret = cjoin(ThreadId[i]);

        printf("\n# Diner ends... All philosophers goes to sleep...\n\n\n");	   
}
