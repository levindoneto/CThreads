#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "cthread.h"
#include "cdata.h"
#include "able_threads.h"
#include "support.h"

/*
 * Test of semaphore's functions and its structures in a stress way:
 * csem_init, cwait(P(s)) and csignal(V(s))
 */

void* test(void* arg){
    cyield();
    return NULL;
}

#define SEMAPHORES 15000

int main() {
    TCB_t* thread_n;
    thread_n = NULL;
    int i;
    int tid_t;
    int t_s[SEMAPHORES];
    //srand(time(NULL));
    tid_t = ccreate(test, NULL);

    for (i=0; i<SEMAPHORES; i++) {
        csem_t test_sem;
        t_s[i] = csem_init(&test_sem, test_sem.count%256);
        printf("\tReturn of csem_init %d: %d\n", i, t_s[i]);
        printf("\tCount_test[%d]: %d\n", i, test_sem.count);
        cwait(&test_sem);
        printf("\tP(s) -> Updated count_test in the semaphore[%d]: %d\n", i, test_sem.count);
        //csignal(&test_sem);    // Descomment for do not let the semaphore blocked
        printf("\tV(s) -> Updated count_test in the semaphore[%d]: %d\n", i, test_sem.count);
    }
    cclean();
return 0;
}
