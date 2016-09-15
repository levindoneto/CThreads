#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "red_black_tree.h"


/*
	Initialing control variables
*/
void init_lib(void){
	TCB_t main_thread;
	control.all_threads = rb_init_tree(sizeof(TCB_t), true);
	control.able_threads = rb_init_tree(sizeof(THREAD_LIST), false);
	control.releaser_threads = rb_init_tree(sizeof(csem_t), true);
	control.init = TRUE;

	main_thread.state = PROCST_EXEC;
	main_thread.tid = 0
	main_thread.ticket = new_ticket();

	rb_insert(control.all_threads, main_thread.tid, &main_thread);

	getcontext(&control.running_thread);
	finish_context->uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	finish_context->uc_link = NULL;
	makecontext(&control.running_thread, (void (*)(void))ended_thread, 0);

}

void ended_thread(void){

	control->running_thread.state = PROCST_TERMINO;

	/* Verify REALESER_THREADS*/
	release_verification();
	
	dispatcher();
}

int new_ticket(void){
	return random2() % 256;
}


ucontext_t* allocator_init(ucontext_t *uc_link, void *(*start)(void *), void *arg){
	// Alloc memory for the context
	allocation_context = (ucontext_t*)malloc(sizeof(ucontext_t*));

	// Verify if the allocation was work
	if (allocation_context == NULL)
		return NULL;

	// Create the context for the threads output
	getcontext(allocation_context);
	// Alloc a stack for the thread for the new context
	allocation_context->uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	allocation_context->uc_stack.ss_size = SIGSTKSZ;
	allocation_context->uc_link = uc_link;
	// Modify the context for allocatation_context
	makecontext(allocation_context, (void (*)(void))start, 1, arg);

	return allocation_context;
}


/* For create a main thread control block for a main*/
TCB_t* TCB_create_main()
{
	TCB_t *thread = TCB_alloc();

	if(thread != NULL)
	{
		// Set the TID_counter for tid of thread created
		thread->tid = TID_counter;
		// Put thread in the able state
		thread->state = PROCST_APTO;
		// For to have a unique TID for each thread
		TID_counter += 1;
	}

	return thread;
}

/* For create a thread control block for a thread*/
TCB_t* TCB_create(ucontext_t* context)
{
	TCB_t *thread = TCB_alloc();

	if(thread != NULL)
	{
		// Set the TID_counter for tid of thread created
		thread->tid = TID_counter;
		// Put thread in the able state
		thread->state = PROCST_APTO;
		// For to have a unique TID for each thread
		TID_counter += 1;
		// Set the context to the created thread
		thread->context = *context;
	}

	return thread;
}

/*	REVISAR!!
		swapcontext talvez não seja a melhor abordagem, já
		se uma thread termina o contexto é desalocado.
*/
void dispatcher(){
    int ticket;
    ticket = random2() % 256; // Discard numbers with more than 255 bits
    TCB_t *current_able;
    //TODO search will be modified to return the closest number to the node that you are looking for
    current_able = rb_search(control.able_threads,ticket);
    if (swapcontext(&control.running_thread->context, &current_able->context) == -1){
        printf("Error swapping");
    }
    else{
        control.running_thread->state = PROCST_APTO;
        running_thread = current_able;
        running_thread->state = PROCST_EXEC;

        //setcontext(&runing_thread -> context);   /* Stay here the question */
    }
}
