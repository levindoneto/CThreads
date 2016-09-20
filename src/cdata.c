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

/*
	Init control variables and create main thread
*/
void init_lib(void){
	TCB_t* main_thread;

	/* Init Tries*/
	control.all_threads = rb_init_tree(sizeof(TCB_t), false);
	control.able_threads = rb_init_tree(sizeof(THREAD_LIST), false);
	control.releaser_threads = rb_init_tree(sizeof(csem_t), false);
	control.init = TRUE;

	/* Create main thread with TID = 0*/
	main_thread = (TCB_t*) malloc(sizeof(TCB_t));
	main_thread->state = PROCST_EXEC;
	main_thread->tid = 0;
	main_thread->ticket = NEW_TICKET;

	/* Insert main thread in all_threads*/
	rb_insert(control.all_threads, main_thread->tid, main_thread);

	/* Create end functions to threads*/
	getcontext(&control.ended_thread);
	control.ended_thread.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	control.ended_thread.uc_link = NULL;
	makecontext(&control.ended_thread, (void (*)(void))ended_thread, 0);

	/* Set Main thread as running*/
	control.running_thread = main_thread;

	/* Create context to main thread*/
	getcontext(&main_thread->context);
	main_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	main_thread->context.uc_link = &control.ended_thread;
}

void ended_thread(void){
	/* Save context to next callback*/
	getcontext(&control.ended_thread);
	control.running_thread->state = PROCST_TERMINO;

	/* Verify REALESER_THREADS*/
	release_verification();

	/* */
	dispatcher();
}

void release_verification(void){
	return;
}

/*	REVISAR!!
		swapcontext talvez não seja a melhor abordagem, já
		se uma thread termina o contexto é desalocado.
*/
void dispatcher(){
	/*
    int ticket;
    ticket = random2() % 256; // Discard numbers with more than 255 bits
    TCB_t *current_able;
    //TODO search will be modified to return the closest number to the node that you are looking for
    current_able = rb_able_search(control.able_threads,ticket);
    if (swapcontext(&control.running_thread->context, &current_able->context) == -1){
        printf("Error swapping");
    }
    else{
        control.running_thread->state = PROCST_APTO;
        running_thread = current_able;
        running_thread->state = PROCST_EXEC;

        //setcontext(&runing_thread -> context);
	}
	*/
}
