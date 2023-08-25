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
void init_graphviz_file(void);


//! @brief opens graphviz file
//!
int open_graphviz_file(void);


//! @brief closes graphviz file
//!
void close_graphviz_file(void);

/**
 * @brief adds node to graphviz file
 *
 * \param node pointer to node that is needed to be added
*/
void graphviz_add_node(token_t * node);


//! @brief creates png from dot file
//!
void print_dump(void);

/**
 * \brief main recursion tool to print trees to graphviz
 *
 * \param node pointer to node that is needed to be written
 *
 * \return 1 if error, 0 if ok
*/
int node_print(token_t* node);

/**
 * \brief recursive node linker
 *
 * \param node pointer to node that is needed to be written
 *
 * \return 1 if error, 0 if ok
*/
int node_link(token_t* node);


/**
 * \brief prints in graphviz log node links
 *
 * \param node1 pointer to first node
 * \param node2 pointer to second node
 *
 * \return 1 if error, 0 if ok
*/
int link_nodes(token_t* node1, token_t* node2);


/**
 * \brief makes and prints tree_dump
 *
 * \param [in] root pointer to root of tree that is needed to be dumped
 *
 * \return 1 if root in nullptr and 0 if all's ok
*/
int tree_print_dump(token_t* root);


//! @brief gets color of node from its type
unsigned get_node_color_from_type(token_type type);


/**
 * \brief makes dump of ast tree
 *
 * \param [in] root pointer to root of tree that is needed to be dumped
*/
int ast_tree_print_dump(token_t* root);


/**
 * \brief main recursion tool to print ast trees to graphviz
 *
 * \param node pointer to node that is needed to be written
 *
 * \return 1 if error, 0 if ok
*/
int ast_node_print(token_t* node);

/**
 * @brief adds ast node to graphviz file
 *
 * \param node pointer to node that is needed to be added
*/
void graphviz_add_ast_node(token_t* node);

/**
 * \brief prints in graphviz log ast node links
 *
 * \param node1 pointer to first node
 * \param node2 pointer to second node
 *
 * \return 1 if error, 0 if ok
*/
void ast_link_nodes(token_t * node1, token_t * node2);

/**
 * \brief recursive ast node linker
 *
 * \param node pointer to node that is needed to be written
 *
 * \return 1 if error, 0 if ok
*/
int ast_node_link(token_t* node);