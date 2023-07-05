#pragma once

#include "tree.h"

//! @brief log_file
//!
extern FILE * log_file;


//! @brief graphviz file
//!
extern FILE * graphviz_file;


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