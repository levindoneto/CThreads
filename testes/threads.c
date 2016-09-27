#include <stdio.h>
#include <stdlib.h>

#include "cthread.h"

/* INCLUDES TO TEST*/
#include "cdata.h"
#include "able_threads.h"

void* thread_2(void *arg){
    printf("->>THREAD 2:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cyield();
    printf("->>THREAD 2:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("RELEASER THREADS\n");
    rb_print_tree(control.releaser_threads);

    printf("->>THREAD 2:\n RUNNING THREAD TID: %d", control.running_thread->tid);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cyield();

    printf("->>THREAD 2:\n RUNNING THREAD TID: %d", control.running_thread->tid);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cyield();

    return NULL;
}

void* thread_1(void *arg){
    printf("->>THREAD 1:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cyield();
    printf("->>THREAD 1:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("RELEASER THREADS\n");
    rb_print_tree(control.releaser_threads);

    printf("->>THREAD 1:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cyield();

    printf("->>THREAD 1:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);
    cyield();

    return NULL;
}

int main(){
    int tid_1, tid_2, join;
    char str[120];
    csem_t sem1;

    cidentify(str, 120);
    printf("%s", str);

    tid_1 = ccreate(thread_1, NULL);
    tid_2 = ccreate(thread_2, NULL);

    printf("->>MAIN THREAD:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf("TID 1: %d, TID 2: %d\n", tid_1, tid_2);
    printf("ABLE THREADS\n");
    rb_able_print_tree(control.able_threads);

    csem_init(&sem1, 0);

    printf("\n->>MAIN THREAD:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    printf(" RUNNING THREAD STATE :%d\n", control.running_thread->state);

    join = cjoin(tid_1);
    printf("\nJOIN 1 RETURN: %s\n", join == TRUE ? "TRUE": "FALSE");
    join = cjoin(tid_2);
    printf("\nJOIN 2 RETURN: %s\n", join == TRUE ? "TRUE": "FALSE");

    cclean();
    return 0;
}
