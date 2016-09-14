#include "red_black_tree.h"
/*
    Implementation based on book:
    Introduction to Algorithms,
        Thomas H. Coremn
        Charles E. Leisserson
        Ronald L. Rivest
        Clifford Stein
    third edition

    Some things was adapted.
    The cases in the fixups can be look at the book.
*/

//INTERNAL FUNCTIONS PROTOTYPES
void rotate_left(RB_BST_TREE* self, RB_BST_NODE* pt);
void rotate_right(RB_BST_TREE* self, RB_BST_NODE* pt);

bool insert_node(RB_BST_TREE* self, RB_BST_NODE* pt);
void insert_fixup(RB_BST_TREE* self, RB_BST_NODE* pt);

void transplant(RB_BST_TREE* self, RB_BST_NODE* u, RB_BST_NODE* v);
void delete_node(RB_BST_TREE* self, RB_BST_NODE* pt);
void delete_fixup(RB_BST_TREE* self, RB_BST_NODE* pt);

void order_print(RB_BST_TREE* self, RB_BST_NODE* node, int level);
void destroy_tree(RB_BST_TREE* self, RB_BST_NODE* node);

// INTERNAL FUNCTIONS

void rotate_left(RB_BST_TREE* self, RB_BST_NODE* pt){
    RB_BST_NODE* pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != self->nil)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == self->nil)
        self->root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void rotate_right(RB_BST_TREE* self, RB_BST_NODE* pt){
    RB_BST_NODE* pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != self->nil)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == self->nil)
        self->root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

void insert_fixup(RB_BST_TREE* self, RB_BST_NODE* pt){
    RB_BST_NODE* parent_pt = self->nil;
    RB_BST_NODE* grand_parent_pt = self->nil;
    bool color_swap;

    while((pt != self->root) && (pt->color != BLACK) &&
         (pt->parent->color == RED)){
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
        if(parent_pt == grand_parent_pt->left){
            RB_BST_NODE* uncle_pt = grand_parent_pt->right;
            // Case 1:
            if(uncle_pt != self->nil && uncle_pt->color == RED){
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else{
                // Case 2:
                if (pt == parent_pt->right){
                    rotate_left(self, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                // Case 3:
                rotate_right(self, grand_parent_pt);
                color_swap = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = color_swap;
                pt = parent_pt;
            }
        }
        else{  // Equal above changing right to left
            RB_BST_NODE* uncle_pt = grand_parent_pt->left;
            if((uncle_pt != self->nil) && (uncle_pt->color == RED)){
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else{
                if (pt == parent_pt->left){
                    rotate_right(self, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotate_left(self, grand_parent_pt);
                color_swap = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = color_swap;
                pt = parent_pt;
            }
        }
    }
    self->root->color = BLACK;
}

bool insert_node(RB_BST_TREE* self, RB_BST_NODE* pt){
    RB_BST_NODE* child_pt = self->root;
    RB_BST_NODE* parent_pt = self->nil;

    if (self->root == self->nil){
        self->root = pt;
        return true;
    }

    while (child_pt != self->nil){
        parent_pt = child_pt;
        if (pt->key > child_pt->key)
            child_pt = child_pt->right;
        else if (pt->key < child_pt->key)
            child_pt = child_pt->left;
        else
            return false;
    }
    pt->parent = parent_pt;
    if (pt->key > parent_pt->key)
        parent_pt->right = pt;
    else
        parent_pt->left = pt;

    return true;
}

void transplant(RB_BST_TREE* self, RB_BST_NODE* u, RB_BST_NODE* v){
    if (u->parent == self->nil)
        self->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void delete_node(RB_BST_TREE* self, RB_BST_NODE* pt){
    RB_BST_NODE* new_pt = pt;
    RB_BST_NODE* child_pt;
    bool original_color = pt->color;

    if (pt->left == self->nil){
        child_pt = pt->right;
        transplant(self, pt, pt->right);
    }
    else if (pt->right == self->nil){
        child_pt = pt->left;
        transplant(self, pt, pt->left);
    }
    else{
        new_pt = pt->right;
        // Finding the minimum of the sub-tree
        while (new_pt->left != self->nil)
            new_pt = new_pt->left;
        original_color = new_pt->color;
        child_pt = new_pt->right;
        if (new_pt->parent == pt)
            child_pt->parent = new_pt;
        else{
            transplant(self, new_pt, new_pt->right);
            new_pt->right = pt->right;
            new_pt->right->parent = new_pt;
        }
        transplant(self, pt, new_pt);
        new_pt->left = pt->left;
        new_pt->left->parent = new_pt;
        new_pt->color = pt->color;
    }

    if (original_color == BLACK)
        delete_fixup(self, child_pt);
}

void delete_fixup(RB_BST_TREE* self, RB_BST_NODE* pt){
    RB_BST_NODE* uncle_pt;

    while (pt != self->root && pt->color == BLACK){
        if (pt == pt->parent->left){
            uncle_pt = pt->parent->right;
            // Case 1:
            if (uncle_pt->color == RED){
                uncle_pt->color = BLACK;
                pt->parent->color = RED;
                rotate_left(self, pt->parent);
                uncle_pt = pt->parent->right;
            }
            // Case 2:
            if (uncle_pt->left->color == BLACK &&
                uncle_pt->right->color == BLACK){
                uncle_pt->color = RED;
                pt = pt->parent;
            }
            else {
                // Case 3:
                if (uncle_pt->right->color == BLACK){
                    uncle_pt->left->color = BLACK;
                    uncle_pt->color = RED;
                    rotate_right(self, uncle_pt);
                    uncle_pt = pt->parent->right;
                }
                // Case 4:
                uncle_pt->color = pt->parent->color;
                pt->parent->color = BLACK;
                uncle_pt->right->color = BLACK;
                rotate_left(self, pt->parent);
                pt = self->root;
            }
        }
        else { // Equal above changing right to left
            uncle_pt = pt->parent->left;
            if (uncle_pt->color == RED){
                uncle_pt->color = BLACK;
                pt->parent->color = RED;
                rotate_right(self, pt->parent);
                uncle_pt = pt->parent->right;
            }
            if (uncle_pt->right->color == BLACK &&
                uncle_pt->left->color == BLACK){
                uncle_pt->color = RED;
                pt = pt->parent;
            }
            else{
                if (uncle_pt->left->color == BLACK){
                    uncle_pt->right->color = BLACK;
                    uncle_pt->color = RED;
                    rotate_left(self, uncle_pt);
                    uncle_pt = pt->parent->left;
                }
                uncle_pt->color = pt->parent->color;
                pt->parent->color = BLACK;
                uncle_pt->left->color = BLACK;
                rotate_right(self, pt->parent);
                pt = self->root;
            }
        }
    }
    pt->color = BLACK;
}

void order_print(RB_BST_TREE* self, RB_BST_NODE* node, int level){
    if (node == self->nil)
        return;
    order_print(self, node->left, level + 1);
    printf("%i(%i)[%s] ", node->key, level,
        node->color == RED ? "RED": "BLACK");
    order_print(self, node->right, level + 1);
}

void destroy_tree(RB_BST_TREE* self, RB_BST_NODE* node){
    if (node == self->nil)
        return;
    destroy_tree(self, node->left);
    destroy_tree(self, node->right);
    if (self->info_copy)
        free(node->info);
    free(node);
}

/* INTERFACE FUNCTIONS*/

RB_BST_TREE* rb_init_tree(int info_size, bool info_copy){
    // Allocating memory
    RB_BST_TREE* self = (RB_BST_TREE*)malloc(sizeof(RB_BST_TREE));
    self->nil = (RB_BST_NODE*)malloc(sizeof(RB_BST_NODE));

    // Nil have all fields Nil and color BLACK
    self->nil->left = self->nil;
    self->nil->right = self->nil;
    self->nil->parent = self->nil;
    self->nil->color = BLACK;

    self->root = self->nil;
    self->info_size = info_size;
    self->info_copy = info_copy;

    return self;
}

bool rb_insert(RB_BST_TREE* self, int key, void* info){
    RB_BST_NODE* pt = (RB_BST_NODE*)malloc(sizeof(RB_BST_NODE));
    if (self->info_copy == true){
        void* new_info = (void*)malloc(self->info_size);
        memcpy(new_info, info, self->info_size);
        pt->info = new_info;
    }
    else
        pt->info = info;
    pt->key = key;
    pt->color = RED;
    pt->left = self->nil;
    pt->right = self->nil;
    pt->parent = self->nil;

    if (!insert_node(self, pt)){
        if (self->info_copy)
            free(pt->info);
        free(pt);
        return false;
    }
    insert_fixup(self, pt);
    return true;
}

void rb_print_tree(RB_BST_TREE* self){
    printf("-RED BLACK TREE, notation:: KEY(LEVEL)[COLOR]\n");
    order_print(self, self->root, 0);
    printf("\n");
}

void* rb_search(RB_BST_TREE* self, int key){
    RB_BST_NODE* pt = self->root;
    while (pt != self->nil){
        if (key > pt->key)
            pt = pt->right;
        else if (key < pt->key)
            pt = pt->left;
        else
            break;
    }
    return pt->info;
}

bool rb_delete(RB_BST_TREE* self, int key){
    RB_BST_NODE* pt = self->root;
    //Searching node
    while (pt != self->nil){
        if (key > pt->key)
            pt = pt->right;
        else if (key < pt->key)
            pt = pt->left;
        else
            break;
    }
    if (pt == self->nil)
        return false;
    delete_node(self, pt);
    if (self->info_copy)
        free(pt->info);
    free(pt);
    return true;
}

void rb_destroy_tree(RB_BST_TREE* self){
    destroy_tree(self, self->root);
    free(self->nil);
    free(self);
}
