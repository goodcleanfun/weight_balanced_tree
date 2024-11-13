#ifndef WEIGHT_BALANCED_TREE_H
#define WEIGHT_BALANCED_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#endif // WEIGHT_BALANCED_TREE_H

#ifndef WEIGHT_BALANCED_TREE_NAME
#error "Must define WEIGHT_BALANCED_TREE_NAME"
#endif

#ifndef WEIGHT_BALANCED_TREE_KEY_TYPE
#error "Must define WEIGHT_BALANCED_TREE_TYPE"
#endif

#ifndef WEIGHT_BALANCED_TREE_VALUE_TYPE
#error "Must define WEIGHT_BALANCED_TREE_VALUE_TYPE"
#endif

#ifndef WEIGHT_BALANCED_TREE_MAX_HEIGHT
#define WEIGHT_BALANCED_TREE_MAX_HEIGHT 128
#endif

#ifndef WEIGHT_BALANCED_TREE_ALPHA
#define WEIGHT_BALANCED_TREE_ALPHA 0.288
#endif

#ifndef WEIGHT_BALANCED_TREE_EPSILON
#define WEIGHT_BALANCED_TREE_EPSILON 0.005
#endif

#define WEIGHT_BALANCED_TREE_CONCAT_(a, b) a ## b
#define WEIGHT_BALANCED_TREE_CONCAT(a, b) WEIGHT_BALANCED_TREE_CONCAT_(a, b)
#define WEIGHT_BALANCED_TREE_TYPED(name) WEIGHT_BALANCED_TREE_CONCAT(WEIGHT_BALANCED_TREE_NAME, _##name)
#define WEIGHT_BALANCED_TREE_FUNC(func) WEIGHT_BALANCED_TREE_CONCAT(WEIGHT_BALANCED_TREE_NAME, _##func)


#define BST_NAME WEIGHT_BALANCED_TREE_NAME
#define BST_KEY_TYPE WEIGHT_BALANCED_TREE_KEY_TYPE
#define BST_VALUE_TYPE WEIGHT_BALANCED_TREE_VALUE_TYPE
#define BST_NODE_EXTRA \
    size_t weight;

#ifdef WEIGHT_BALANCED_TREE_KEY_EQUALS
#define BST_KEY_EQUALS WEIGHT_BALANCED_TREE_KEY_EQUALS
#endif

#ifdef WEIGHT_BALANCED_TREE_KEY_LESS_THAN
#define BST_KEY_LESS_THAN WEIGHT_BALANCED_TREE_KEY_LESS_THAN
#endif

#include "binary_tree/binary_tree.h"

#undef BST_NAME
#undef BST_KEY_TYPE
#undef BST_VALUE_TYPE
#undef BST_NODE_EXTRA
#ifndef WEIGHT_BALANCED_TREE_KEY_EQUALS
#define WEIGHT_BALANCED_TREE_KEY_EQUALS WEIGHT_BALANCED_TREE_TYPED(key_equals)
#endif
#ifndef WEIGHT_BALANCED_TREE_KEY_LESS_THAN
#define WEIGHT_BALANCED_TREE_KEY_LESS_THAN WEIGHT_BALANCED_TREE_TYPED(key_less_than)
#endif

#define WEIGHT_BALANCED_TREE_NODE WEIGHT_BALANCED_TREE_TYPED(node_t)

#define WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_NAME WEIGHT_BALANCED_TREE_TYPED(node_memory_pool)

#define MEMORY_POOL_NAME WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_NAME
#define MEMORY_POOL_TYPE WEIGHT_BALANCED_TREE_NODE
#include "memory_pool/memory_pool.h"
#undef MEMORY_POOL_NAME
#undef MEMORY_POOL_TYPE

#define WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(name) WEIGHT_BALANCED_TREE_CONCAT(WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_NAME, _##name)


typedef struct WEIGHT_BALANCED_TREE_NAME {
    WEIGHT_BALANCED_TREE_TYPED(node_t) *root;
    WEIGHT_BALANCED_TREE_TYPED(node_memory_pool) *pool;
} WEIGHT_BALANCED_TREE_NAME;

WEIGHT_BALANCED_TREE_NAME *WEIGHT_BALANCED_TREE_FUNC(new)(void) {
    WEIGHT_BALANCED_TREE_NAME *tree = malloc(sizeof(WEIGHT_BALANCED_TREE_NAME));
    if (tree == NULL) return NULL;

    tree->pool = WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(new)();
    if (tree->pool == NULL) {
        free(tree);
        return NULL;
    }
    tree->root = WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(get)(tree->pool);
    if (tree->root == NULL) {
        WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(destroy)(tree->pool);
        free(tree);
        return NULL;
    }
    tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->weight = 1;
    return tree;
}

void WEIGHT_BALANCED_TREE_FUNC(destroy)(WEIGHT_BALANCED_TREE_NAME *tree) {
    if (tree == NULL) return;
    WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(destroy)(tree->pool);
    free(tree);
}

void WEIGHT_BALANCED_TREE_FUNC(rebalance)(WEIGHT_BALANCED_TREE_TYPED(stack_t) *stack) {
    WEIGHT_BALANCED_TREE_NODE *tmp_node;
    // rebalance
    while (!WEIGHT_BALANCED_TREE_FUNC(stack_empty)(stack)) {
        tmp_node = WEIGHT_BALANCED_TREE_FUNC(stack_pop)(stack);
        if (WEIGHT_BALANCED_TREE_FUNC(node_is_leaf)(tmp_node)) {
            // skip if there's a leaf node
            continue;
        }
        tmp_node->weight = tmp_node->left->weight + tmp_node->right->weight;
        // left subtree is heavier
        if (tmp_node->right->weight < WEIGHT_BALANCED_TREE_ALPHA * tmp_node->left->weight) {
            if (tmp_node->left->left->weight > (WEIGHT_BALANCED_TREE_ALPHA + WEIGHT_BALANCED_TREE_EPSILON) * tmp_node->weight) {
                // left subtree's left subtree is heavier, rotate right
                WEIGHT_BALANCED_TREE_FUNC(rotate_right)(tmp_node);
                tmp_node->right->weight = tmp_node->right->left->weight + tmp_node->right->right->weight;
            } else {
                // left subtree's right subtree is heavier, rotate left then right
                WEIGHT_BALANCED_TREE_FUNC(rotate_left)(tmp_node->left);
                WEIGHT_BALANCED_TREE_FUNC(rotate_right)(tmp_node);
                tmp_node->right->weight = tmp_node->right->left->weight + tmp_node->right->right->weight;
                tmp_node->left->weight = tmp_node->left->left->weight + tmp_node->left->right->weight;
            }
        // right subtree is heavier
        } else if (tmp_node->left->weight < WEIGHT_BALANCED_TREE_ALPHA * tmp_node->weight) {
            if (tmp_node->right->right->weight > (WEIGHT_BALANCED_TREE_ALPHA + WEIGHT_BALANCED_TREE_EPSILON) * tmp_node->weight) {
                // right subtree's right subtree is heavier, rotate left
                WEIGHT_BALANCED_TREE_FUNC(rotate_left)(tmp_node);
                tmp_node->left->weight = tmp_node->left->left->weight + tmp_node->left->right->weight;
            } else {
                // right subtree's left subtree is heavier, rotate right then left
                WEIGHT_BALANCED_TREE_FUNC(rotate_right)(tmp_node->right);
                WEIGHT_BALANCED_TREE_FUNC(rotate_left)(tmp_node);
                tmp_node->right->weight = tmp_node->right->left->weight + tmp_node->right->right->weight;
                tmp_node->left->weight = tmp_node->left->left->weight + tmp_node->left->right->weight;
            }
        } 
    }
}



bool WEIGHT_BALANCED_TREE_FUNC(insert)(WEIGHT_BALANCED_TREE_NAME *tree, WEIGHT_BALANCED_TREE_KEY_TYPE key, void *value) {
    if (tree == NULL) return false;
    WEIGHT_BALANCED_TREE_NODE *node = tree->root;
    WEIGHT_BALANCED_TREE_TYPED(node_memory_pool) *pool = tree->pool;

    WEIGHT_BALANCED_TREE_NODE *tmp_node;
    bool done = false;
    if (node->left == NULL) {
        // empty tree
        node->left = (WEIGHT_BALANCED_TREE_NODE *)value;
        node->key = key;
        node->weight = 1;
        node->right = NULL;
    } else {
        // non-empty tree
        WEIGHT_BALANCED_TREE_TYPED(stack_t) stack = (WEIGHT_BALANCED_TREE_TYPED(stack_t)){
            .stack = {NULL},
            .stack_size = 0
        };
        tmp_node = WEIGHT_BALANCED_TREE_TYPED(candidate_leaf)(node, key, &stack);
        if (WEIGHT_BALANCED_TREE_KEY_EQUALS(key, tmp_node->key)) {
            // key already exists
            return false;
        }
        // tmp_node is the leaf that will become the parent of the new leaf
        WEIGHT_BALANCED_TREE_NODE *old_leaf = WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(get)(pool);
        if (old_leaf == NULL) return false;
        old_leaf->key = tmp_node->key;
        old_leaf->left = tmp_node->left;
        old_leaf->right = NULL;
        old_leaf->weight = 1;
        WEIGHT_BALANCED_TREE_NODE *new_leaf = WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(get)(pool);
        if (new_leaf == NULL) {
            WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(release)(pool, old_leaf);
            return false;
        }
        new_leaf->key = key;
        new_leaf->left = (WEIGHT_BALANCED_TREE_NODE *)value;
        new_leaf->right = NULL;
        new_leaf->weight = 1;
        if (WEIGHT_BALANCED_TREE_KEY_LESS_THAN(tmp_node->key, key)) {
            tmp_node->left = old_leaf;
            tmp_node->right = new_leaf;
            tmp_node->key = key;
        } else {
            tmp_node->left = new_leaf;
            tmp_node->right = old_leaf;
        }
        tmp_node->weight = 2;
        WEIGHT_BALANCED_TREE_FUNC(rebalance)(&stack);
    }
    return true;
}

void *WEIGHT_BALANCED_TREE_FUNC(delete)(WEIGHT_BALANCED_TREE_NAME *tree, WEIGHT_BALANCED_TREE_KEY_TYPE key) {
    if (tree == NULL) return NULL;
    WEIGHT_BALANCED_TREE_NODE *node = tree->root;
    WEIGHT_BALANCED_TREE_TYPED(node_memory_pool) *pool = tree->pool;

    WEIGHT_BALANCED_TREE_NODE *tmp_node, *upper_node, *other_node;
    void *deleted_value;
    if (node->left == NULL) {
        return NULL;
    } else if (node->right == NULL) {
        if (WEIGHT_BALANCED_TREE_KEY_EQUALS(key, node->key)) {
            deleted_value = (void *)node->left;
            node->left = NULL;
            return deleted_value;
        } else {
            return NULL;
        }
    } else {
        tmp_node = node;
        WEIGHT_BALANCED_TREE_TYPED(stack_t) stack = (WEIGHT_BALANCED_TREE_TYPED(stack_t)){
            .stack = {NULL},
            .stack_size = 0
        };
        while (WEIGHT_BALANCED_TREE_FUNC(node_is_internal)(tmp_node)) {
            if (!WEIGHT_BALANCED_TREE_FUNC(stack_push(&stack, tmp_node))) return NULL;
            upper_node = tmp_node;
            if (WEIGHT_BALANCED_TREE_KEY_LESS_THAN(key, tmp_node->key)) {
                tmp_node = upper_node->left;
                other_node = upper_node->right;
            } else {
                tmp_node = upper_node->right;
                other_node = upper_node->left;
            }
        }
        if (!WEIGHT_BALANCED_TREE_KEY_EQUALS(key, tmp_node->key)) {
            return NULL;
        } else {
            upper_node->key = other_node->key;
            upper_node->left = other_node->left;
            upper_node->right = other_node->right;
            upper_node->weight = other_node->weight;
            deleted_value = (void *)tmp_node->left;
            // remove upper_node from the stack since it's now a leaf node
            WEIGHT_BALANCED_TREE_FUNC(stack_pop)(&stack);

            WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(release)(pool, tmp_node);
            WEIGHT_BALANCED_TREE_NODE_MEMORY_POOL_FUNC(release)(pool, other_node);
            WEIGHT_BALANCED_TREE_FUNC(rebalance)(&stack);
            return deleted_value;
        }
    }

}



#undef WEIGHT_BALANCED_TREE_CONCAT_
#undef WEIGHT_BALANCED_TREE_CONCAT
#undef WEIGHT_BALANCED_TREE_TYPED
#undef WEIGHT_BALANCED_TREE_FUNC
