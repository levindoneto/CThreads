#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "cthread.h"
#include "support.h"
#include "cdata.h"

/*
 * Test of semaphore's functions and its structures:
 * csem_init, cwait(P(s)) and csignal(V(s))
 */

void* test(void *args){
    printf("void test");
    return NULL;
}

int main() {
    TCB_t* thread_n;
    thread_n = NULL;

    csem_t test_sem;        //Go to the stack, without malloc/free
    int t_s, t_w, tid_t;    // Returns

    tid_t = ccreate(test, NULL);
    printf("TID_t: %d\n\n", tid_t);

    /* Test of csem_init */
    printf("@TEST csem_init:\n");
    // Test of csem_init (csem_init is working)
    t_s = csem_init(&test_sem, test_sem.count%256);
    printf("\tReturn of csem_init: %d\n", t_s);
    printf("\tCount_test: %d\n", test_sem.count);

    /* Test of cwait */
    printf("@TEST cwait:\n");
    t_w = cwait(&test_sem);  // P(s): Decrement resources' counter and test resources' counter
    printf("\tReturn cwait: %d\n", t_w);

    cwait(&test_sem);
    printf("\tP(s) -> Updated count_test: %d\n", test_sem.count);
    cwait(&test_sem);
    printf("\tP(s) -> Updated count_test: %d\n", test_sem.count);
    cwait(&test_sem);
    printf("\tP(s) -> Updated count_test: %d\n", test_sem.count);

    /* Test of csignal */
    printf("\n@TEST csignal:\n");
    csignal(&test_sem);      // V(s): Increment resources' counter
    printf("\tV(s) -> Updated count_test: %d\n", test_sem.count);
    csignal(&test_sem);
    printf("\tV(s) -> Updated count_test: %d\n", test_sem.count);
    csignal(&test_sem);
    printf("\tV(s) -> Updated count_test:: %d\n", test_sem.count);
    csignal(&test_sem);
    /* Test CSignal even with no threads in semaphore FIFO */
    printf("\n@TEST with no threads in semaphore's FIFO:\n");
    printf("\tV(s) -> Updated count_test:: %d\n", test_sem.count);
    csignal(&test_sem);
    printf("\tV(s) -> Updated count_test:: %d\n", test_sem.count);
    csignal(&test_sem);
    printf("\tV(s) -> Updated count_test:: %d\n", test_sem.count);
    csignal(&test_sem);
    printf("\tV(s) -> Updated count_test:: %d\n", test_sem.count);
    csignal(&test_sem);

return 0;
}
