#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cthread.h"
#include "cdata.h"
#include "able_threads.h"

void* test(void* arg){
    cyield();
    return NULL;
}

#define NUMBER_THREADS 2000
int main(){
    int i;
    int tids[NUMBER_THREADS];
    int numbers[NUMBER_THREADS];
    srand(time(NULL));

    for (i = 0; i < NUMBER_THREADS; i++){
        // Test insert at tree
        tids[i] = ccreate(test, NULL);
        numbers[i] = 0;
    }
    rb_able_print_tree(control.able_threads);
    i = 0;
    while (i < NUMBER_THREADS){
        int index = rand()%NUMBER_THREADS;
        if (numbers[index] == 0){
            numbers[index] = 1;
            rb_able_delete(tids[index]);
            i++;
        }
    }

    cclean();
    return 0;
}
