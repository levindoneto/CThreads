#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include <stdio.h>

int init = 0;

int ccreate (void* (*start)(void*), void *arg){
	//__init__ queue and main's TCB
	if(init != 1){ // Thread has not initialized yet
		able_queue = (PFILA2) malloc(sizeof(PFILA2));
		block_join = (PFILA2) malloc(sizeof(PFILA2));
		block = (PFILA2) malloc(sizeof(PFILA2));
		
		CreateFila2(able_queue);
		CreateFila2(block_join);
		CreateFila2(block);

		thread_main = TCB_create_main();
		init = 1;    // __init__ thread
	}
	}
