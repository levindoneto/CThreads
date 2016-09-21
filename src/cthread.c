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

struct _cth control = {.init = FALSE};

int ccreate(void* (*start)(void*), void *arg){
	TCB_t* new_thread;

	/* Check if internal variables was initialized */
	if(control.init == FALSE)
		init_lib();

	/* Making thread context */
	new_thread = (TCB_t*) malloc(sizeof(TCB_t));
	getcontext(&new_thread->context);
	new_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	new_thread->context.uc_stack.ss_size = SIGSTKSZ;
	new_thread->context.uc_link = &control.ended_thread;
	makecontext(&new_thread->context, (void (*)(void))start, 1, arg);

	/* Changing TCB fields */
	new_thread->tid = rb_get_key_max(control.all_threads) + 1;
	new_thread->state = PROCST_APTO;
	new_thread->ticket = NEW_TICKET;

	/* Put it into all_treads and able_threads */
	if (!rb_insert(control.all_threads, new_thread->tid, new_thread))
		return FALSE;
	if (!rb_able_insert(new_thread->tid))
		return FALSE;

	/* Return the Thread Identifier*/
	return new_thread->tid;
};

int cyield(void){
	/* Verify REALESER_THREADS*/
	release_verification();
	dispatcher();
	return TRUE;
}

int cjoin(int tid){
	csem_t* block;

	/* Test if tid thread is the running_thread*/
	if (tid == control.running_thread->tid)
		return FALSE;
	/* Test if tid reference to a existing thread*/
	if (rb_search(control.all_threads, tid) == NULL)
		return FALSE;
	/* Test if other thread was waiting for this tid thread*/
	if (rb_search(control.releaser_threads, tid) == NULL)
		return FALSE;

	/* Create Semaphore to block running_thread*/
	block = (csem_t*) malloc(sizeof(csem_t));
	csem_init(block, 0);

	if (!rb_insert(control.releaser_threads, tid, block))
		return FALSE;

	cwait(block);
	return TRUE;
}

int csem_init(csem_t *sem, int count){
	/* Check if internal variables was initialized */
	if(control.init == FALSE)
		init_lib();

	/* Set count of the semaphore variable */
	sem->count = count;
	/* Allocate FIFO */
	sem->fila = (PFILA2) malloc(sizeof(PFILA2));
	/* If it allocated ... */
	if (sem->fila == NULL)
		return FALSE;
	/* Create FIFO, if it returned 0, it was successfully created */
	if(CreateFila2(sem->fila) == 0)
		return TRUE;
	return FALSE;
};

int cwait(csem_t *sem){
    /* Test count to see if it still has available resources */
	if(sem->count <= 0){
	    /* Put running thread in the semaphore FIFO */
		AppendFila2(sem->fila, (void *)control.running_thread);
		/* Change state to blocked */
    	control.running_thread->state = PROCST_BLOQ;
    	sem->count--;
    	dispatcher();
        return TRUE;
	}
	/* If it has available resources it needs to decrement count and keep running the thread*/
	else
		sem->count--;
	return TRUE;
};

int csignal(csem_t *sem){
    /* Increments count to free resources*/
	sem->count++;
	/* Goes to the first item in the FIFO*/
	if(FirstFila2(sem->fila) == 0){
		TCB_t *freeTCB;
		/* Get the first item in FIFO that will be freed*/
		freeTCB = GetAtIteratorFila2(sem->fila);
		if(freeTCB != NULL){
		    /* Delete item from FIFO, thread is not blocked by the semaphore anymore*/
			DeleteAtIteratorFila2(sem->fila);
			/* Change state to 'able' */
			freeTCB->state = PROCST_APTO;
			/* Put the thread in the ables queue so it can run again*/
			rb_able_insert(freeTCB->tid);
		}
		else{
		   /* There was no resource to be freed*/
		   sem->count--;
		   return FALSE;
		}
	}
	else{
	    /* There was an error going to the first item in the FIFO*/
	    return FALSE;
	}
	return TRUE;
};

int cidentify(char *name, int size){
	char* str ="Béuren Felipe Bechlin (00230321)\n\
				Eduardo Stein Brito (00243657)\n\
				Levindo Gabriel Taschetto Neto (00243685)\n";
	memcpy(name, str, size);
	return TRUE;
}
