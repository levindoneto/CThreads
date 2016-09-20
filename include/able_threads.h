#ifndef _ABLE_THREADS_H_
#define _ABLE_THREADS_H_

#include "cdata.h"

int rb_able_insert(int tid);
int rb_able_delete(int tid, int ticket);
TCB_t* rb_able_search(int ticket);
void rb_able_order_print(RB_BST_TREE*, RB_BST_NODE*, int);
void rb_able_print_tree();

#endif
