#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/* For some reason memcpy need the string.h to don't appear warnings*/
#include <string.h>
/*
*******************RED*BLACK*BINARY*SEARCH*TREE*********************************
  This code implements a red black tree, with a integer KEY that is used to
  order the search and a INFO to store any kind of data.
  by Beuren Bechlin
  https://gitlab.com/maplex/RedBlackBST
********************************************************************************
*/
enum Color{RED,BLACK};

struct rb_bst_node{
  /* Struct pointer to data that will be stored in the tree */
  void* info;
  /* INTERNAL STRUCTURE */
  int   key;
  bool  color;
  struct rb_bst_node*  left;
  struct rb_bst_node*  right;
  struct rb_bst_node*  parent;
};

struct rb_bst_tree{
  struct rb_bst_node* root;
  struct rb_bst_node* nil;
  int info_size;
  bool info_copy;
};

typedef struct rb_bst_node  RB_BST_NODE;
typedef struct rb_bst_tree RB_BST_TREE;

/* ***Initialing tree***
    Create the tree
    Arguments:
     1- int info_size:
        size of struct that will be saved as info on each node
     2- bool info_copy:(include <stdbool>)
        true: the data in the struct send when insert a node
            will be copied to a new variable with malloc
        false: only save the struct pointer send when insert a node
    Return:
        RB_BST_TREE* self: instance of the tree, it's will be used
        in all interface functions
*/
RB_BST_TREE* rb_init_tree(int info_size, bool info_copy);


/* ***Insering nodes***
    Insert a node with key 'key' and information 'info'
    Arguments:
     1 - RB_BST_TREE* self:
        instance of the tree
     2 - int key:
        key value of a node
     3 - void* info:
        pointer to info data that will be stored, behavior depends
        the init tree second argument
    Return:
        true: if the node was inserted
        false: if don't, probably there are a node with this key
*/
bool rb_insert(RB_BST_TREE* self, int key, void* info);


/* ***Searching nodes***
    Search for a node with key 'key'
    Arguments:
     1 - RB_BST_TREE* self:
        instance of the tree
     2 - int key:
        key value of the node that want to be found
    Return:
        void *info: if was found
        NULL: if don't

    BE CAREFUL: if you modify any field with this pointer it will imply
     that the info field on tree node will change too
     To do local copies use 'memcpy(destination, source, size_in_bytes)'
    EXAMPLE:
        memcpy(rb_search(TREE, KEY), local_copy, sizeof(STRUCT_INFO));
*/
void* rb_search(RB_BST_TREE* self, int key);


/* ***Delete nodes***
    Delete a node with key 'key'.
    Arguments:
     1 - RB_BST_TREE* self:
        instance of the tree
     2 - int key:
        key value of the node that want to be found
    Return:
        true: if the node as deleted
        false: if don't, probably the key wasn't in the tree
*/
bool rb_delete(RB_BST_TREE* self, int key);

/* ***Printing Tree***
    Arguments:
     1 - RB_BST_TREE* self:
        instance of the tree
*/
void rb_print_tree(RB_BST_TREE* self);

/* ***Destroy Tree***
    Free all pointers that was created by the module
    Arguments:
     1 - RB_BST_TREE* self:
        instance of the tree
*/
void rb_destroy_tree(RB_BST_TREE* self);
#endif
