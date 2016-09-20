#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

#include "red_black_tree.h"
#include "able_threads.h"
#include "cthread.h"
#include "support.h"
#include "cdata.h"



int ccreate_test (void* (*start)(void*), void *arg, int ticket_test){
	TCB_t* new_thread;

	/* Check if internal variables was initialized */
	if(control.init == FALSE)
		init_lib();

	/* Making thread context */
	new_thread = (TCB_t*) malloc(sizeof(TCB_t));
	getcontext(&new_thread->context);
	new_thread->context.uc_link = &control.ended_thread;
	new_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	new_thread->context.uc_stack.ss_size = SIGSTKSZ;
	makecontext(&new_thread->context, (void (*)(void))start, 1, arg);

	/* Changing TCB fields */
	new_thread->tid = rb_get_key_max(control.all_threads) + 1;
	new_thread->state = PROCST_APTO;
	new_thread->ticket = ticket_test;

	/* Put it into all_treads and able_threads */
	rb_insert(control.all_threads, new_thread->tid, new_thread);
	rb_able_insert(new_thread->tid);

	/* Return the Thread Identifier*/
	return new_thread->tid;
};

void* test(void *arg){
    printf("void test");
}

int main(){
	TCB_t* raffle;
	ccreate_test(test, NULL, 10);
	ccreate_test(test, NULL, 5);
	ccreate_test(test, NULL, 6);
	ccreate_test(test, NULL, 9);
	ccreate_test(test, NULL, 3);
	ccreate_test(test, NULL, 11);
	ccreate_test(test, NULL, 1);
	ccreate_test(test, NULL, 11);
	ccreate_test(test, NULL, 11);

	// print informations about tree
	rb_able_print_tree(control.able_threads);
	rb_print_tree(control.all_threads);
	raffle = rb_able_search(0);
	printf("TID RAFFLE: %d\n", raffle->tid);
	raffle = rb_able_search(4);
	printf("TID RAFFLE: %d\n", raffle->tid);
	raffle = rb_able_search(6);
	printf("TID RAFFLE: %d\n", raffle->tid);
	raffle = rb_able_search(7);
	printf("TID RAFFLE: %d\n", raffle->tid);
	raffle = rb_able_search(8);
	printf("TID RAFFLE: %d\n", raffle->tid);
	raffle = rb_able_search(10);
	printf("TID RAFFLE: %d\n", raffle->tid);
	raffle = rb_able_search(20);
	printf("TID RAFFLE: %d\n", raffle->tid);

	raffle = (TCB_t*)rb_search(control.all_threads, 8);
	rb_able_delete(8, raffle->ticket);
	rb_able_print_tree(control.able_threads);
	return 0;
}
