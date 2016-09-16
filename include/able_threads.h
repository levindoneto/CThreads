#ifndef _ABLE_THREADS_H_
#define _ABLE_THREADS_H_

#include "cdata.h"

int rb_able_insert(int tid);
int rb_able_delete(int tid, int ticket);
TCB_t* rb_able_search(int ticket);

#endif
