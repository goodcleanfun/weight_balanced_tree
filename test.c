#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "greatest/greatest.h"

#define WEIGHT_BALANCED_TREE_NAME weight_balanced_tree_uint32
#define WEIGHT_BALANCED_TREE_KEY_TYPE uint32_t
#define WEIGHT_BALANCED_TREE_VALUE_TYPE char *
#include "weight_balanced_tree.h"
#undef WEIGHT_BALANCED_TREE_NAME
#undef WEIGHT_BALANCED_TREE_KEY_TYPE
#undef WEIGHT_BALANCED_TREE_VALUE_TYPE

TEST test_weight_balanced_tree(void) {
    weight_balanced_tree_uint32 *tree = weight_balanced_tree_uint32_new();

    char *val1 = "a";
    char *val2 = "b";
    char *val3 = "c";

    weight_balanced_tree_uint32_insert(tree, 1, "a");
    weight_balanced_tree_uint32_insert(tree, 3, "b");
    weight_balanced_tree_uint32_insert(tree, 5, "c");
    weight_balanced_tree_uint32_insert(tree, 7, "d");
    weight_balanced_tree_uint32_insert(tree, 9, "e");

    char *a = weight_balanced_tree_uint32_search(tree->root, 1);
    ASSERT_STR_EQ(a, "a");

    char *b = weight_balanced_tree_uint32_search(tree->root, 3);
    ASSERT_STR_EQ(b, "b");

    char *c = weight_balanced_tree_uint32_search(tree->root, 5);
    ASSERT_STR_EQ(c, "c");

    char *d = weight_balanced_tree_uint32_search(tree->root, 7);
    ASSERT_STR_EQ(d, "d");

    char *e = weight_balanced_tree_uint32_search(tree->root, 9);
    ASSERT_STR_EQ(e, "e");

    a = weight_balanced_tree_uint32_delete(tree, 1);
    ASSERT_STR_EQ(a, "a");
  
    a = weight_balanced_tree_uint32_search(tree->root, 1);
    ASSERT(a == NULL);
  
    b = weight_balanced_tree_uint32_delete(tree, 3);
    ASSERT_STR_EQ(b, "b");

    e = weight_balanced_tree_uint32_delete(tree, 9);
    ASSERT_STR_EQ(e, "e");

    c = weight_balanced_tree_uint32_search(tree->root, 5);
    ASSERT_STR_EQ(c, "c");

    c = weight_balanced_tree_uint32_delete(tree, 5);
    ASSERT_STR_EQ(c, "c");

    d = weight_balanced_tree_uint32_delete(tree, 7);
    ASSERT_STR_EQ(d, "d");

    d = weight_balanced_tree_uint32_search(tree->root, 7);
    ASSERT(d == NULL);

    weight_balanced_tree_uint32_insert(tree, 7, "d");
    d = weight_balanced_tree_uint32_search(tree->root, 7);

    weight_balanced_tree_uint32_destroy(tree);
    PASS();
}



/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_weight_balanced_tree);

    GREATEST_MAIN_END();        /* display results */
}