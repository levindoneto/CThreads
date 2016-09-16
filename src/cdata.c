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
	Initialing control variables
*/
void init_lib(void){
	TCB_t main_thread;
	control.all_threads = rb_init_tree(sizeof(TCB_t), true);
	control.able_threads = rb_init_tree(sizeof(THREAD_LIST), false);
	control.releaser_threads = rb_init_tree(sizeof(csem_t), true);
	control.init = TRUE;

	main_thread.state = PROCST_EXEC;
	main_thread.tid = 0;
	main_thread.ticket = NEW_TICKET;

	rb_insert(control.all_threads, main_thread.tid, &main_thread);

	getcontext(&control.running_thread->context);
	control.running_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	control.running_thread->context.uc_link = NULL;
	makecontext(&control.running_thread->context, (void (*)(void))ended_thread, 0);

}

void ended_thread(void){

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
