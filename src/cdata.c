#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "../include/cthread.h"
#include "../include/cdata.h"
#include <stdio.h>

int TID_counter = 0;

ucontext_t* allocator_init()
{
	// Alloc memory for the context
	allocation_context = (ucontext_t*)malloc(sizeof(ucontext_t*));
	
	// Verify if the allocation was work
	if (allocation_context == NULL)
		return NULL;
	
	// Create the context for the threads output
	getcontext(allocation_context);
	// Alloc a stack for the thread for the new context
    allocation_context->uc_stack.ss_sp = (char*) malloc(16384);
    allocation_context->uc_stack.ss_size = 16384;
    allocation_context->uc_link = NULL;
	// Modify the context for allocatation_context
    makecontext(allocation_context, (void (*)(void))EndPoint, 0);

	return allocation_context;
}


