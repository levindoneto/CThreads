#include <stdio.h>
#include <stdlib.h>

#include "cthread.h"

/* INCLUDES TO TEST*/
#include "cdata.h"
#include "able_threads.h"


void* thread_1(void *arg){
    printf("->>RUNNING THREAD TID: %d\n ->>WILL WAIT FOR:%d\n", control.running_thread->tid, (int*)arg);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cjoin((int*)arg);
    printf("->>RUNNING THREAD TID: %d\n ->>WILL WAIT FOR:%d\n", control.running_thread->tid, (int*)arg);
    printf("RELEASER THREADS\n");
    rb_print_tree(control.releaser_threads);

    return NULL;
}


int main(){
    int tid_1;
    int i = 0;
    tid_1 = ccreate(thread_1, NULL);
	printf("->>First thread:\n %d\n",tid_1);
    printf("->>MAIN THREAD:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    while(i<160){
    int aux = ccreate(thread_1, (void*)tid_1);
	tid_1 = aux; 
    i++;
    }
    printf("->>MAIN THREAD:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    cjoin(tid_1);
	
    return 0;
}
