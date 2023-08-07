#pragma once

#include "tree.h"

//! @brief log_file
//!
extern FILE* log_file;


//! @brief graphviz file
//!
extern FILE* graphviz_file;


//! @brief extern counter for graphviz pngs
//!
extern int graphviz_png_count;


//! @brief initialises graphviz file
//!
//! @return 1 if error, 0 if ok
//!
int init_graphviz_file(void);


//! @brief opens graphviz file
//!
int open_graphviz_file(void);


//! @brief closes graphviz file
//!
int close_graphviz_file(void);


//! @brief adds node to graphviz file
//!
int graphviz_add_node(token_t * node);


//! @brief creates png from dot file
//!
int print_dump(void);


int node_print(token_t* node);
int node_link(token_t* node);
int link_nodes(token_t* node1, token_t* node2);


//! @brief makes and prints tree_dump
int tree_print_dump(token_t* root);


//! @brief gets color of node from its type
unsigned get_node_color_from_type(token_type type);


//! @brief
int ast_tree_print_dump(token_t* root);


//! @brief prints ast nodes in it's style
int ast_node_print(token_t* node);


int graphviz_add_ast_node(token_t* node);



int ast_link_nodes(token_t * node1, token_t * node2);
int ast_node_link(token_t* node);