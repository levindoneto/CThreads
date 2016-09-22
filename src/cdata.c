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
	main_thread = (TCB_t*)malloc(sizeof(TCB_t));
	main_thread->state = PROCST_EXEC;
	main_thread->tid = 0;
	main_thread->ticket = NEW_TICKET;

	/* Insert main thread in all_threads*/
	rb_insert(control.all_threads, main_thread->tid, main_thread);

	/* Create end functions to threads*/
	getcontext(&control.ended_thread);
	control.ended_thread.uc_link = NULL;
	control.ended_thread.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	control.ended_thread.uc_stack.ss_size = SIGSTKSZ;
	makecontext(&control.ended_thread, (void (*)(void))ended_thread, 0);

	/* Set Main thread as running*/
	control.running_thread = main_thread;

	/* Create context to main thread*/
	getcontext(&main_thread->context);
	main_thread->context.uc_link = &control.ended_thread;
	main_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	main_thread->context.uc_stack.ss_size = SIGSTKSZ;
}

void ended_thread(void){
	/* Save context to next callback*/
	getcontext(&control.ended_thread);

	/* Verify REALESER_THREADS*/
	release_verification();

	control.running_thread->state = PROCST_TERMINO;
	#if DEBUG == TRUE
	printf("\n########END#OF#THREAD######\n");
	printf("TID: %i \n",control.running_thread->tid);
	printf("###########################\n\n");
	#endif
	dispatcher();

	/* Verify if exists no more threads*/
	if (control.all_threads->root == control.all_threads->nil){
		rb_destroy_tree(control.all_threads);
		rb_destroy_tree(control.able_threads);
		rb_destroy_tree(control.releaser_threads);
		#if DEBUG == TRUE
		printf("\n---END OF LIBRARY.\n\n");
		#endif
	}

}

void release_verification(void){
	csem_t* sem;

	/* Search for running_thread in releaser threads*/
	sem = (csem_t*)rb_search(control.releaser_threads, control.running_thread->tid);

	/* No thread need this*/
	if (sem == NULL)
		return;
	/* Unblock thread blocked by cjoin*/
	csignal(sem);

	/* Debug prints*/
	#if DEBUG == TRUE
	printf("\n---------RELEASE----------\n");
	printf(" ABLE THREADS:\n");
	rb_able_print_tree(control.able_threads);
	printf("---------------------------\n\n");
	#endif
	/* Remover releaser thread*/
	rb_delete(control.releaser_threads, control.running_thread->tid);
	free(sem);
}

void dispatcher(){
	TCB_t* next_thread;
	TCB_t* current_thread = control.running_thread;
    int raffle;

	/* Raffle a ticket*/
    raffle = NEW_TICKET;
	/* Search for thread winner*/
    next_thread = rb_able_search(raffle);
	/* No thread in able threads*/
	if (next_thread == NULL)
		next_thread = current_thread;
	else{
		/* Remove next thread from able threads tree*/
		rb_able_delete(next_thread->tid);
		/* Set as running_thread*/
		next_thread->state = PROCST_EXEC;
		control.running_thread = next_thread;
	}
	/* Debug prints*/
	#if DEBUG == TRUE
	printf("\n*********DISPATCHER********\n");
	printf(" NEXT TID: %d RAFFLED: %d\n", next_thread->tid, raffle);
	#endif

	/* Old running thread must be inserted in able threads tree*/
	if (current_thread->state == PROCST_EXEC){
		/* If the next thread are the running thread do nothing*/
		if (next_thread != current_thread){
			/* Set state of old running thread as APTO and add to able threads*/
			current_thread->state = PROCST_APTO;
			rb_able_insert(current_thread->tid);
			/* Debug prints*/
			#if DEBUG == TRUE
			printf("ABLE THREADS:\n");
			rb_able_print_tree(control.able_threads);
			printf("***************************\n\n");
			#endif

			/* Swapping context to new thread*/
			swapcontext(&current_thread->context, &next_thread->context);
		}
	}
	/* Old running thread must be blocked*/
	else if (current_thread->state == PROCST_BLOQ){
		/* Debug prints*/
		#if DEBUG == TRUE
		printf("REALESER THREADS:\n");
		rb_print_tree(control.releaser_threads);
		printf("***************************\n\n");
		#endif
		/* Swapping context to new thread*/
		swapcontext(&current_thread->context, &next_thread->context);
	}
	/* Running Thread must be deleted*/
	else if (current_thread->state == PROCST_TERMINO){
		/* Remove running thread from list of all threads*/
		rb_delete(control.all_threads, current_thread->tid);
		/* Debug prints*/
		#if DEBUG == TRUE
		printf("ALL THREADS:\n");
		rb_print_tree(control.all_threads);
		printf("***************************\n\n");
		#endif
		/* Free TCB memory of old running thread*/
		free(current_thread->context.uc_stack.ss_sp);
		free(current_thread);
		/* If the next thread are the running thread do nothing*/
		if (next_thread != current_thread)
			setcontext(&next_thread->context);
	}
}
