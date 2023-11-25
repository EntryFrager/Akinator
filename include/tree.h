#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "error.h"
#include "utils.h"

#define DEBUG

#ifdef DEBUG
    #define CALL_DUMP(tree, code_error)                                         \
    {                                                                           \
        tree_dump_text (tree, code_error, __FILE__, __func__, __LINE__);        \
        tree_dump_graph_viz (tree, __FILE__, __func__, __LINE__);               \
    }

    #define assert_tree(tree)                                                                   \
    {                                                                                           \
        int code_error = 0;                                                                     \
        if ((code_error = (tree_verificator (tree) | node_verificator (tree->root))) != ERR_NO) \
        {                                                                                       \
            CALL_DUMP(tree, code_error)                                                         \
            return code_error;                                                                  \
        }                                                                                       \
    }
#else
    #define CALL_DUMP(...)
    #define assert_tree(...)
#endif

const bool LEFT  = false;
const bool RIGHT = true;

const bool INIT = true;
const bool INIT_NOT = false;

typedef struct NODE{
    NODE *left   = NULL;
    NODE *right  = NULL;
    NODE *parent = NULL;

    const char *value = NULL;
} NODE;

typedef struct {
    char *fp_name_base = NULL;
    FILE *fp_base      = NULL;

    size_t size_file = 0;

    char *buf = NULL;

#ifdef DEBUG
    const char *fp_dump_text_name = NULL;
    const char *fp_dot_name       = NULL;
    const char *fp_name_html      = NULL;
    const char *fp_image          = NULL;

    FILE *fp_html_dot = NULL;
#endif
} INFO;

typedef struct {
    NODE *root = NULL;

    bool init_status = false;

    INFO info = {};
} TREE;

int create_tree (TREE *tree, const char *value);

NODE *create_node (const char *value, NODE *left, NODE *right, NODE *parent);

int input_base (TREE *tree);

NODE *split_node (TREE *tree, NODE *node);

int add_node (NODE *node, const char *value, const bool side);

int delete_node (NODE *node);

int print_tree (NODE *node, FILE *stream);

int destroy_tree (TREE *tree);

#ifdef DEBUG
    int tree_verificator (TREE *tree);

    int node_verificator (NODE *node);

    void tree_dump_text (TREE *tree, const int code_error, 
                         const char *file_err, const char *func_err, 
                         const int line_err);

    void tree_dump_graph_viz (TREE *tree, const char *file_err, 
                              const char *func_err, const int line_err);

    void tree_dump_html (TREE *tree);
#endif

#endif //TREE_H