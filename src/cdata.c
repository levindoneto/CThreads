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

void* EndPoint()
{
	if(FirstFila2(block_join) == 0){
		join_t* join_t1; // Create a join, that have information about the 
						 // threads that aren't finished.
		do{
			join_t1 = GetAtIteratorFila2(block_join);

			if(join_t1 != NULL)
				if(run_t->tid == join_t1->wait){
					join_t1->thread->state = PROCST_APTO;
					/* Put threads from join in the able queue */
					AppendFila2(ables, join_t1->thread);
					/* Remove threads blocked from join */
					DeleteAtIteratorFila2(blocks_join);
					break;
				}
		}while(NextFila2(blocks_join)==0);
	}

	free(run_t); // Only free space that was used by a thread in execution
	dispatcher();
	return 0;
}

/* For create a main thread control block for a main*/
TCB_t* TCB_create_main()
{
	TCB_t *thread = TCB_alloc();
    
	if(thread != NULL)
	{
		// Set the TID_counter for tid of thread created
		thread->tid = TID_counter;
		// Put thread in the able state
		thread->state = PROCST_APTO;
		// For to have a unique TID for each thread		
		TID_counter += 1;
	}

	return thread;
}

/* For create a thread control block for a thread*/
TCB_t* TCB_create(ucontext_t* context)
{
	TCB_t *thread = TCB_alloc();

	if(thread != NULL)
	{
		// Set the TID_counter for tid of thread created
		thread->tid = TID_counter;
		// Put thread in the able state
		thread->state = PROCST_APTO;
		// For to have a unique TID for each thread	
		TID_counter += 1;
		// Set the context to the created thread
		thread->context = *context;
	}

	return thread;
}


void dispatcher(){
    int ticket;
    ticket = random2() % 256; // Discard numbers with more than 255 bits
    TCB_t *current_able;
    //TODO search will be modified to return the closest number to the node that you are looking for
    current_able = rb_search(control.able_threads,ticket);
    if (swapcontext(&control.running_thread->context, &current_able->context) == -1){
        printf("Error swapping");
    }
    else{
        control.running_thread->state = PROCST_APTO;
        running_thread = current_able;
        running_thread->state = PROCST_EXEC;
        
        //setcontext(&runing_thread -> context);   /* Stay here the question */
    }
}
