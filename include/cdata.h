/*
 * cdata.h: arquivo de inclus�o de uso apenas na gera��o da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida
 *
 */
#ifndef __cdata__
#define __cdata__

#include "red_black_tree.h"
#include <ucontext.h>
/* random2*/
#include "support.h"

#define DEBUG FALSE

#define	PROCST_CRIACAO	0
#define	PROCST_APTO	1
#define	PROCST_EXEC	2
#define	PROCST_BLOQ	3
#define	PROCST_TERMINO	4

#if DEBUG == TRUE
#define NEW_TICKET Random2() % 100
#else
#define NEW_TICKET Random2() % 256
#endif

#define TRUE 0
#define FALSE -1

/* NÃO ALTERAR ESSA struct */
typedef struct s_TCB {
	int		tid; 		// identificador da thread
	int		state;		// estado em que a thread se encontra
	// 0: Criação; 1: Apto; 2: Execução; 3: Bloqueado e 4: Término
	int 		ticket;		// "bilhete" de loteria da thread, para uso do escalonador
	ucontext_t 	context;	// contexto de execu��o da thread (SP, PC, GPRs e recursos)
} TCB_t;

typedef struct _thr_list{
    TCB_t* curr_tcb;
    struct _thr_list* next;
}THREAD_LIST;

struct _cth{
    /* Verify if the main thread was initialized */
    int init;
    /* Tree of all threads */
    RB_BST_TREE* all_threads;
    /* Tree of ablle threads */
    RB_BST_TREE* able_threads;
    /* Tree of releaser threads */
    RB_BST_TREE* releaser_threads;
    /* Current thread pointer running */
    TCB_t* running_thread;
	/* UCP to delete a thread that end */
	ucontext_t ended_thread;
};

extern struct _cth control;

/* Function prototypes*/
void init_lib(void);
void ended_thread(void);
void release_verification(void);
void dispatcher(void);
void cclean(void);
#endif
