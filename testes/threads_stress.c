#include <stdio.h>
#include <stdlib.h>

#include "cthread.h"

/* INCLUDES TO TEST*/
#include "cdata.h"
#include "able_threads.h"

#define NUMBER_THREADS 3000

void* thread_1(void *arg){
    printf("->>RUNNING THREAD TID: %d\n ->>WILL WAIT FOR:%d\n",
    control.running_thread->tid, *(int*)arg);
    cjoin(*(int*)arg);
    return NULL;
}

int main(){
    int tids[NUMBER_THREADS+1];
    int i = 0;
    tids[NUMBER_THREADS] = NUMBER_THREADS + 1;
    for (i = 0; i < NUMBER_THREADS; i++){
        tids[i] = ccreate(thread_1, (void*)&tids[i+1]);
    }
    printf("->>MAIN THREAD:\n RUNNING THREAD TID: %d\n", control.running_thread->tid);
    cjoin(tids[0]);

    cclean();
    return 0;
}
