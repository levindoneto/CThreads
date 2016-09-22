#include <stdbool.h>
#include "cthread.h"
#include "cdata.h"
#include "red_black_tree.h"

int rb_able_insert(int tid){
    THREAD_LIST* list_node;
    THREAD_LIST* this;
    TCB_t* new_thread = NULL;

    /* Searching TCB from tid*/
    new_thread = rb_search(control.all_threads, tid);
    if (new_thread == NULL)
        return false;

    /* Creating new list node with the TCB with this tib*/
    list_node = (THREAD_LIST*)malloc(sizeof(THREAD_LIST));
    list_node->curr_tcb = new_thread;
    list_node->next = NULL;

    /* Verifing if the new_thread ticket is on tree*/
    this = (THREAD_LIST*)rb_search(control.able_threads, new_thread->ticket);
    /* Ticket isn't on tree*/
    if (this == NULL)
        rb_insert(control.able_threads, new_thread->ticket, list_node);
    /* Ticket is already on tree*/
    else {
        THREAD_LIST* last;
        /* Keep tid order in the list*/
        while (this != NULL){
            if(this->curr_tcb->tid > tid)
                break;
            last = this;
            this = this->next;
        }
        /* This tid is the bigger than the tids in list*/
        if (this == NULL)
            last->next = list_node;
        /* There is a tid smallest than this tid*/
        else{
            last->next = list_node;
            list_node->next = this;
        }
    }
    return true;
}

int rb_able_delete_ticket(int tid, int ticket){
    THREAD_LIST* this;

    this = rb_search(control.able_threads, ticket);
    if (this == NULL)
        return FALSE;

    /* Only one list node */
    if (this->next == NULL){
        rb_delete(control.able_threads, ticket);
        free(this);
    }
    /* More than one list node*/
    else{
        THREAD_LIST* last = NULL;
        /* Searching for the TID in list*/
        while (this != NULL){
            if (this->curr_tcb->tid == tid)
                break;
            last = this;
            this = this->next;
        }
        /* First node in the list*/
        if (last == NULL){
            this->curr_tcb = this->next->curr_tcb;
            this->next = this->next->next;
            free(this->next);
        }
        else{
            last->next = this->next;
            free(this);
        }
    }
    return TRUE;
}

int rb_able_delete(int tid){
    THREAD_LIST* this;
    TCB_t* ticket_by_tid;  // This saves the ticket given a tid

    // Search for the ticket
    ticket_by_tid = (TCB_t*)rb_search(control.all_threads, tid);

    this = rb_search(control.able_threads, ticket_by_tid->ticket);
    if (this == NULL)
        return FALSE;

    if (this->next == NULL){
        rb_delete(control.able_threads, ticket_by_tid->ticket);
        free(this);
    }

    else{
        THREAD_LIST* last = NULL;
        /* Searching for the TID in list*/
        while (this != NULL){
            if (this->curr_tcb->tid == tid)
                break;
            last = this;
            this = this->next;
        }
        /* First node in the list*/
        if (last == NULL){
            this->curr_tcb = this->next->curr_tcb;
            this->next = this->next->next;
            free(this->next);
        }
        else{
            last->next = this->next;
            free(this);
        }
    }
    return TRUE;
}

TCB_t* rb_able_search(int ticket){
    THREAD_LIST* this;
    RB_BST_TREE* self = control.able_threads;
    RB_BST_NODE* pt = self->root;
    RB_BST_NODE* smallest = self->nil;
    TCB_t* node;

    int delta_min, delta;

    /* Empty Tree*/
    if (self->root == self->nil)
        return NULL;

    /* MAX TICKET*/
    delta_min = 256;

    /* Searching for the key*/
    while (pt != self->nil){
        /* Calcute delta between ticket and node key */
        delta = (pt->key > ticket) ? (pt->key - ticket) : (ticket - pt->key);
        /* Search for smallest delta*/
        if (delta < delta_min){
            delta_min = delta;
            smallest = pt;
        }
        else if (delta == delta_min){
            int tid_smallest;

            this = (THREAD_LIST*)smallest->info;
            node = (TCB_t*)this->curr_tcb;
            tid_smallest = node->tid;

            this = (THREAD_LIST*)pt->info;
            node = (TCB_t*)this->curr_tcb;
            /* Search for smallest TID*/
            if (tid_smallest > node->tid)
                smallest = pt;
        }
        /* Walking through tree*/
        if (ticket > pt->key){
            pt = pt->right;
        }
        else if (ticket < pt->key)
            pt = pt->left;
        else
            break;
    }
    this = (THREAD_LIST*)smallest->info;
    return (TCB_t*)this->curr_tcb;
}

void rb_able_order_print(RB_BST_TREE* self, RB_BST_NODE* node, int level){
    THREAD_LIST* list_node;
    /* Leaf node, returning*/
    if (node == self->nil)
        return;

    /* Go to left*/
    rb_able_order_print(self, node->left, level + 1);

    /* Pass the information of a node for a list node*/
    list_node = (THREAD_LIST*)node->info;

    printf("%i(%i)[%s]: ", node->key, level, node->color == RED ? "RED": "BLACK");
    while(list_node != NULL){
        printf("TID(%i) ", list_node->curr_tcb->tid);
        list_node = list_node->next;
    }
    printf("\n");

    /* Go to right*/
    rb_able_order_print(self, node->right, level + 1);
}

void rb_able_print_tree(RB_BST_TREE* self){
    printf("-RED BLACK TREE, notation:: KEY(LEVEL)[COLOR]\n");
    rb_able_order_print(self, self->root, 0);
}
