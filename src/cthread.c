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