#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "cthread.h"
#include "support.h"
#include "cdata.h"

void* test(void *args){
    printf("void test");
    return NULL;
}

int main() {
    //TCB_t* thread_t;    
    csem_t test_sem; //Go to the stack, without malloc/free
    int count_test=1;
    int t_s, t_w, tid_t;    // Returns
    
    //tid_t = ccreate(test, NULL);
    //printf("TID_t: %d\n\n", tid_t);

    printf("Test csem_init:\n");
    // Test of csem_init (csem_init is working)
    t_s = csem_init(&test_sem, count_test);
    printf("\tReturn of csem_init: %d\n", t_s);
    printf("\tCount_test: %d\n\n", count_test);    

    printf("Test cwait:\n");
    t_w = cwait(&test_sem);
    printf("\tReturn cwait: %d\n", t_w);
    printf("\tUpdated count_test: %d\n\n", test_sem.count);

    return 0;
}
