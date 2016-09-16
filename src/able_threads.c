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
        return FALSE;

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
    return TRUE;
}

int rb_able_delete(int tid, int ticket){
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

TCB_t* rb_able_search(int ticket){
    THREAD_LIST* this;
    RB_BST_TREE* self = control.able_threads;
    RB_BST_NODE* pt = self->root;
    RB_BST_NODE* last = self->nil;
    TCB_t* node;
    TCB_t* parent;

    int delta_parent, delta_node;

    /* Searching for the key*/
    while (pt != self->nil){
        last = pt;
        if (ticket > pt->key)
            pt = pt->right;
        else if (ticket < pt->key)
            pt = pt->left;
        else
            break;
    }
    /* Found the ticket on tree*/
    if (last == pt){
        this = (THREAD_LIST*) pt->info;
        return (TCB_t*)this->curr_tcb;
    }

    /* Calcuting delta in module*/
    pt = last->parent;
    delta_node = (last->key > ticket) ? (last->key - ticket) : (ticket - last->key);
    delta_parent = (pt->key > ticket) ? (pt->key - ticket) : (ticket - pt->key);

    /* Getting tcb pointers*/
    this = (THREAD_LIST*) last->info;
    node = (TCB_t*) this->curr_tcb;
    this = (THREAD_LIST*) pt->info;
    parent = (TCB_t*) this->curr_tcb;

    if (delta_parent == delta_node){
        /*Searching for smallest tid*/
        if(node->tid < parent->tid)
            return parent;
        else
            return node;
    }
    else if (delta_node < delta_parent)
        return node;
    else
        return parent;
}
