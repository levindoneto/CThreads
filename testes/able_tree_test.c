#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

#include "red_black_tree.h"
#include "able_threads.h"
#include "cthread.h"
#include "support.h"
#include "cdata.h"

int ccreate_test (void (*start)(void*), void *arg, int ticket_test){
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
	new_thread.ticket = ticket_test;

	/* Put into all_treads and able_threads */
	rb_insert(control.all_threads, new_thread.tid, &new_thread);
	rb_able_insert(new_thread.tid);

	/* Return the Thread Identifier*/
	return new_thread.tid;
};


void test(void *arg){
    printf("void test");
}

void RB_order_print(RB_BST_TREE* self, RB_BST_NODE* node, int level){
    THREAD_LIST* list_node;
    /* Leaf node, returning*/
    if (node == self->nil)
        return;

    /* Go to left*/
    RB_order_print(self, node->left, level + 1);

    /* Pass the information of a node for a list node*/
    list_node = (THREAD_LIST*)node->info;

    printf("%i(%i)[%s]: ", node->key, level, node->color == RED ? "RED": "BLACK");
    while(list_node != NULL){
        printf("TID(%i) ", list_node->curr_tcb->tid);
        list_node = list_node->next;
    }
    printf("\n");

    /* Go to right*/
    RB_order_print(self, node->right, level + 1);
}

void RB_able_print_tree(){

    RB_order_print(control.able_threads, control.able_threads->root, 0);

}

int main(){
	
	// Create a several tid_test
	int tid_test;
	tid_test = ccreate_test(test, NULL, 10);
	
	int tid_test1;
	tid_test = ccreate_test(test, NULL, 5);
	
	int tid_test2;
	tid_test = ccreate_test(test, NULL, 6);
	
	int tid_test3;
	tid_test = ccreat_test(test, NULL, 9);
	
	int tid_test4;
	tid_test = ccreat_test(test, NULL, 3);
	
	int tid_test5;
	tid_test = ccreat_test(test, NULL, 11);
	
	int tid_test6;
	tid_test = ccreat_test(test, NULL, 1);
	
	// print informations about tree
	RB_able_print_tree();
	return 0;
}