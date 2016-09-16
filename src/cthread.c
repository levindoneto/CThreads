#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "red_black_tree.h"
#include "able_threads.h"
#include "cthread.h"
#include "support.h"
#include "cdata.h"

int ccreate (void* (*start)(void*), void *arg){
	TCB_t new_thread;

	/* Check if internal variables was initialized */
	if(control.init == FALSE)
		init_lib();

	/* Making thread context */
	getcontext(&new_thread.context);
	new_thread.context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	new_thread.context.uc_stack.ss_size = SIGSTKSZ;
	new_thread.context.uc_link = &control.ended_thread;
	makecontext(&new_thread.context, (void (*)(void))start, 1, arg);

	/* Changing TCB fields */
	new_thread.tid = rb_get_key_max(control.all_threads);
	new_thread.state = PROCST_APTO;
	new_thread.ticket = NEW_TICKET;

	/* Put into all_treads and able_threads */
	rb_insert(control.all_threads, new_thread.tid, &new_thread);
	rb_able_insert(new_thread.tid);

	/* Return the Thread Identifier*/
	return new_thread.tid;
};

int cyield (void){
	return FALSE;
}

int cjoin(int tid){
	return FALSE;
}

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
		if(control.running_thread != NULL){
		    /* Put running thread in the semaphore FIFO */
			AppendFila2(sem->fila, (void *)control.running_thread);
			/* Change state to blocked */
	    	control.running_thread->state = PROCST_BLOQ;
	    	sem->count--;
	    	/* Saves context in the thread  */
	    	getcontext(&control.running_thread->context);
	    	dispatcher();
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
			/* Change state to 'able' */
			freeTCB->state = PROCST_APTO;
			/* Put the thread in the ables queue so it can run again */
			rb_able_insert(freeTCB->tid);
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
