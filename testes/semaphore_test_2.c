#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "cthread.h"
#include "support.h"
#include "cdata.h"

/* 
 * Test if csignal works and also if the process is added to the semaphore's FIFO 
 */

void* test(void *args){
    printf("void test");
    return NULL;
}

int main() {
    //TCB_t* thread_t;    
    csem_t test_sem; //Go to the stack, without malloc/free
    int count_test=2;
    int t_s, t_w, tid_t;    // Returns
    csem_init(&test_sem, count_test);
    printf("\tCount_test: %d\n\n", test_sem.count);
    printf("Test cwait:\n");
    cwait(&test_sem);
    printf("\tCount_test: %d\n\n", test_sem.count);
    cwait(&test_sem);
    printf("\tCount_test: %d\n\n", test_sem.count);
    cwait(&test_sem);
    printf("\tCount_test: %d\n\n", test_sem.count);
    printf("Test csignal:\n");
    csignal(&test_sem);
    printf("\tCount_test: %d\n\n", test_sem.count);
    csignal(&test_sem);
    printf("\tCount_test: %d\n\n", test_sem.count);
    csignal(&test_sem);
    printf("\tCount_test: %d\n\n", test_sem.count);
    csignal(&test_sem);

    return 0;
}
