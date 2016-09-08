#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include <stdio.h>

int init = 0;

int ccreate (void* (*start)(void*), void *arg){
	//__init__ queue and main's TCB
	if(init != 1){ // Thread has not initialized yet
		ables = (PFILA2) malloc(sizeof(PFILA2));
		block_join = (PFILA2) malloc(sizeof(PFILA2));
		block = (PFILA2) malloc(sizeof(PFILA2));
		
		CreateFila2(ables);
		CreateFila2(block_join);
		CreateFila2(block);

		thread_main = TCB_create_main();
		init = 1;    // __init__ thread
	}

	/* Create a new TCB with enter function */
	ucontext_t* finish_context = (ucontext_t*) malloc(sizeof(ucontext_t));

	/* __Init__ the data structure for the run flow */
	getcontext(finish_context);
	/* Define Stack */
	finish_context->uc_stack.ss_sp = (char*) malloc(16384); //Stack Size = 16384
	finish_context->uc_link = NULL;
	/* Modify the context */
	makecontext(finish_context, (void (*)(void))EndPoint, 0);

	getcontext(context);
	context->uc_stack.ss_sp = (char*) malloc(16384);
	context->uc_stack.ss_size = 16384;
	context->uc_link = finish_context;
	makecontext(context, (void (*)(void))start, 1, arg);

	TCB_t* thread = TCB_create(context);

	/* Put ables in the able queue */
	AppendFila2(ables, (void (*)(void)) thread);

	/* Return the Thread Identifier*/
	return thread->tid;
};

int csem_init(csem_t *sem, int count){
	/* Set count of the semaphore variable */
	sem->count = count;
	/* Allocate FIFO */
	sem->fila = (PFILA2) malloc(sizeof(PFILA2));
	/* If it allocated ... */
	if(sem->fila != NULL)
	/* Create FIFO, if it returned 0, it was successfully created */
		if(CreateFila2(sem->fila) == 0)
			return 0;

	return -1;
};




int cwait(csem_t *sem){
    /* Test count to see if it still has available resources */
	if(sem->count <= 0){
		if(running_thread != NULL){
		    /* Put running thread in the semaphore FIFO */
			AppendFila2(sem->fila, (void *)running_thread);
            /* Change state to blocked */
	    	running_thread->state = PROCST_BLOQ;
	    	/* Saves context in the thread  */
	    	getcontext(&running_thread->context);
            return 0;
	    
		}
		/* No thread was running, so it needs to call dispatcher to swap context */
		else{
		    sem->count--;
		    dispatcher();
			
		}
	}
    /* If it has available resources it needs to decrement count and keep running the thread  */
	else{
		sem->count--;
	}

	return 0;
};

int csignal(csem_t *sem){
    /* Increments count to free resources  */
	sem->count++;
    /* Goes to the first item in the FIFO  */
	if(FirstFila2(sem->fila) == 0){
		TCB_t *freeTCB;
        /* Get the first item in FIFO that will be freed  */
		freeTCB = GetAtIteratorFila2(sem->fila);
		if(freeTCB != NULL){
		    /* Delete item from FIFO, thread is not blocked by the semaphore anymore  */
			DeleteAtIteratorFila2(sem->fila);
			/* Put the thread in the ables queue so it can run again */
			AppendFila2(ables, freeTCB);
		}
		else{
		   /* There was no resource to be freed **/
		   sem->count--; 
		   return -1;
		}
	}
	else 
	{
	    /* There was an error going to the first item in the FIFO **/
	     return -1;
	}

	return 0;
};


