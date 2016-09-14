/*
 * cdata.h: arquivo de inclusão de uso apenas na geração da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida
 *
 */
#ifndef __cdata__
#define __cdata__

#define	PROCST_CRIACAO	0
#define	PROCST_APTO	1
#define	PROCST_EXEC	2
#define	PROCST_BLOQ	3
#define	PROCST_TERMINO	4

// It's used for to resolve the problem of there is more than one
// ticket associed to TID. This violates the binary tree principle. 
typedef struct _thr_list{
    TCB_t* curr_tcb;
    int tcb_id;
    struct _thr_list* next;
}THREAD_LIST;

struct _cth{
    // Verify if the main thread was initialized 
    bool init;
    // Tree of all threads
    RB_BST_TREE* all_threads;
    // Tree of able threads
    RB_BST_TREE* able_threads;
    // Tree of releaser threads
    RB_BST_TREE* releaser_threads;
    // Current thread pointer running
    TCB_t* running_thread;
}control;/* NÃO ALTERAR ESSA struct */

typedef struct s_TCB { 
	int		tid; 		// identificador da thread
	int		state;		// estado em que a thread se encontra
					// 0: Criação; 1: Apto; 2: Execução; 3: Bloqueado e 4: Término
	int 		ticket;		// "bilhete" de loteria da thread, para uso do escalonador
	ucontext_t 	context;	// contexto de execução da thread (SP, PC, GPRs e recursos) 
} TCB_t; 

#endif
