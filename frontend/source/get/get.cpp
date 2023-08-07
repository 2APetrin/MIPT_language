#include "get.h"
#include "../main_frontend/frontend.h"
#include "../tree/log_tree.h"
#include "../../../file_work/file_work.h"

FILE* log_file;


token_t* get_general(text_t* text)
{
    ASSERT(text);
    POS = 0;

    open_write_file("frontend/logs/frontend_log.html", &log_file);
    fprintf(log_file, "<html>\n");

    token_t* temp = get_def_function(text);
    token_t* curr = temp;

    token_t* def_tree_root = temp;

    while (temp)
    {
        temp = get_def_function(text);
        curr->right_child = temp;
        curr = temp;
    }

    token_t* main_block = get_start(text);

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();

    ret_node->left_child    = def_tree_root;
    ret_node->right_child   = main_block;

    if (main_block)       main_block->parent = ret_node;
    if (def_tree_root) def_tree_root->parent = ret_node;

    fprintf(log_file, "\n</html>\n");
    fclose(log_file);

    return ret_node;
}


token_t* get_start(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type == TYPE_START) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in get start. Start (born) is missing");

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in get start. Open bracket (enter_mipt) is missing");

    token_t* ret = get_comp(text);

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in get start. Close bracket (enter_mipt) is missing");

    if (POS < WORDS_CNT && TOKEN_BUFF[POS]->type == TYPE_FINISH)
    {
        free(TOKEN_BUFF[POS]->word);
        free(TOKEN_BUFF[POS]);
    }
    else
    {
        fprintf(log_file, "<pre>\nERROR in get start. Finish (die) is missing\n</pre>");
        text->status++;
        return nullptr;
    }

    return ret;
}


token_t* get_comp(text_t* text)
{
    ASSERT(text);

    token_t* temp = get_operator(text);
    token_t* curr = temp;

    token_t* comp_tree_root = temp;

    while (temp)
    {
        temp                   = get_operator(text);
        curr->right_child      = temp;
        if (temp) temp->parent = curr;

        curr = temp;
    }

    return comp_tree_root;
}


token_t* get_operator(text_t* text)
{
    ASSERT(text);

    token_type type = TOKEN_BUFF[POS]->type;

    if (type == TYPE_IF)                           return get_If(text);

    if (type == TYPE_VAR_INIT || type == TYPE_VAR) return get_variable_op(text);

    if (type == TYPE_PRINT)                        return get_print(text);

    if (type == TYPE_LOOP_FIRST)                   return get_loop(text);

    if (type == TYPE_DECREASE)                     return get_decrease(text);

    if (type == TYPE_RETURN)                       return get_return(text);

    return nullptr;
}


token_t* get_loop(text_t* text)
{
    ASSERT(text);

    token_t* curr_node  = TOKEN_BUFF[POS];
    token_t* first_node = TOKEN_BUFF[POS];

    POS++;

    token_t* logical_expr = get_expr(text);

    if (TOKEN_BUFF[POS]->type != TYPE_LOOP_CLOSE)
        GET_SYNTAX_ERROR("ERROR in loop. Next node after expression isn't close loop bracket (nulevok_untill_they_run_out)");

    FREE();

    curr_node->left_child                  = logical_expr;
    if (logical_expr) logical_expr->parent = curr_node;

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in loop. Open bracket (enter_mipt) is missing");

    token_t* right_subtree = get_comp(text);

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in loop. Close bracket (get_sent_down) is missing");

    curr_node->right_child                   = right_subtree;
    if (right_subtree) right_subtree->parent = curr_node;

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    ret_node->left_child = first_node;
    first_node->parent   = ret_node;

    return ret_node;
}


token_t* get_decrease(text_t* text)
{
    ASSERT(text);

    token_t* curr_node  = TOKEN_BUFF[POS];
    token_t* first_node = TOKEN_BUFF[POS];

    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
        GET_SYNTAX_ERROR("ERROR in decrease operator. Next node after solve_nulevka init isn't variable");

    curr_node->left_child   = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = curr_node;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
        GET_SYNTAX_ERROR("ERROR in decrease operator. Dot in the end is missing");

    FREE();

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    ret_node->left_child = first_node;
    first_node->parent   = ret_node;

    return ret_node;
}


token_t* get_return(text_t* text)
{
    ASSERT(text);

    token_t* curr_node   = TOKEN_BUFF[POS];
    token_t* first_node  = TOKEN_BUFF[POS];

    POS++;

    token_t* expr = get_expr(text);

    curr_node->left_child = expr;
    if (expr) expr->parent = curr_node;

    if (TOKEN_BUFF[POS]->type != TYPE_RETURN_BRCKT)
        GET_SYNTAX_ERROR("ERROR in return. Zadach in the end is missing");

    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
        GET_SYNTAX_ERROR("ERROR in return. Dot in the end is missing");

    FREE();

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    first_node->parent   = ret_node;
    ret_node->left_child = first_node;

    return ret_node;
}


// ============================================================== variables zone ==============================================================


token_t* get_variable_op(text_t* text)
{
    ASSERT(text);

    token_type type = TOKEN_BUFF[POS]->type;

    if (type == TYPE_VAR_INIT) return get_var_initialization(text);
    if (type == TYPE_VAR)
    {
        for (unsigned i = 0; i < FUNC_CNT; i++) 
            if (!strcmp(FUNC_BUFF[i]->name, TOKEN_BUFF[POS]->word)) return get_func_call(text);

        return get_assignment(text);
    }

    return nullptr;
}


token_t* get_func_call(text_t* text)
{
    if (TOKEN_BUFF[POS]->type != TYPE_VAR) return nullptr;

    TOKEN_BUFF[POS]->type = TYPE_FUNC_CALL;

    token_t* first_node = TOKEN_BUFF[POS];

    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_O_F_BR)
        GET_SYNTAX_ERROR("ERROR in function call. Expected [");

    FREE();

    token_t* temp      = nullptr;
    token_t* var_root  = nullptr;
    token_t* prev_node = nullptr;
    unsigned var_count = 0;

    if (TOKEN_BUFF[POS]->type != TYPE_C_F_BR)
    {
        temp      = get_expr(text);
        var_root  = create_node(TYPE_DOT, POISON, temp, nullptr);
        DOTS_BUFF[DOTS_CNT] = var_root;
        DOTS_CNT++;
        prev_node = var_root;
        var_count++;

        if (temp) temp->parent = prev_node;

        while (TOKEN_BUFF[POS]->type == TYPE_VAR_SEPARATE)
        {
            FREE();

            temp = create_node(TYPE_DOT, POISON, get_expr(text), nullptr);
            DOTS_BUFF[DOTS_CNT] = temp;
            DOTS_CNT++;

            prev_node->right_child = temp;

            if (temp) temp->parent = prev_node;
            if (temp->left_child) temp->left_child->parent = temp;

            prev_node = temp;
            var_count++;
        }
    }

    for (unsigned i = 0; i < FUNC_CNT; i++)
    {
        if (!strcmp(FUNC_BUFF[i]->name, first_node->word))
        {
            if (FUNC_BUFF[i]->var_cnt != var_count)
            {
                printf("%s has %u vars\nbut in def is has %u vars\n", first_node->word, var_count, FUNC_BUFF[i]->var_cnt);
                GET_SYNTAX_ERROR("ERROR in function call. Number of variables des not match");
            }
        }
    }
    if (TOKEN_BUFF[POS]->type != TYPE_C_F_BR)
        GET_SYNTAX_ERROR("ERROR in function call. Expected ]");

    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
        GET_SYNTAX_ERROR("ERROR in function call. Dot in the end is missing");

    FREE();

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    first_node->parent   = ret_node;
    ret_node->left_child = first_node;

    ret_node->left_child->left_child = var_root;
    if (var_root) var_root->parent = ret_node->left_child;

    return ret_node;
}


token_t* get_var_initialization(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_VAR_INIT) return nullptr;

    token_t* curr_node  = TOKEN_BUFF[POS];
    token_t* first_node = TOKEN_BUFF[POS];

    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
        GET_SYNTAX_ERROR("ERROR in var initialization. Next node after invent init isn't variable");

    curr_node->left_child   = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = curr_node;

    int flag = 1;
    unsigned var_pos = VAR_CNT;

    for (unsigned i = 0; i < VAR_CNT; i++)
        if (!strcmp(VAR_BUFF[i]->name, TOKEN_BUFF[POS]->word))
        {
            flag = 0;
            var_pos = i;
        }

    if (flag)
    {
        VAR_BUFF[VAR_CNT]       = (var_t*) calloc (1, sizeof(var_t));
        VAR_BUFF[VAR_CNT]->name = (char*)  calloc (MAX_WORD_LEN, sizeof(char));
        strncpy(VAR_BUFF[VAR_CNT]->name, TOKEN_BUFF[POS]->word, MAX_WORD_LEN-1);
        VAR_CNT++;
    }
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_NUM)
        GET_SYNTAX_ERROR("ERROR in var initialization. Next node after invent init isn't number");

    curr_node->right_child  = TOKEN_BUFF[POS];
    TOKEN_BUFF[POS]->parent = curr_node;

    sscanf(TOKEN_BUFF[POS]->word, "%lg", &VAR_BUFF[var_pos]->value);
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
        GET_SYNTAX_ERROR("ERROR in var initialization. Dot in the end is missing");

    FREE();

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    first_node->parent   = ret_node;
    ret_node->left_child = first_node;

    return ret_node;
}


token_t* get_assignment(text_t* text)
{
    token_t* first_node = nullptr; 

    for (unsigned i = 0; i < VAR_CNT; i++)
        if (!strcmp(VAR_BUFF[i]->name, TOKEN_BUFF[POS]->word)) first_node = TOKEN_BUFF[POS];

    if (!first_node)
        GET_SYNTAX_ERROR("ERROR in assignment. No such variable exists");

    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_ASSIGNMENT)
        GET_SYNTAX_ERROR("ERROR in assignment. podgon_pod is missing");

    token_t* middle_part    = TOKEN_BUFF[POS];
    middle_part->left_child = first_node;
    first_node->parent      = middle_part;
    POS++;

    token_t* right_part = nullptr;
    for (unsigned i = 0; i < FUNC_CNT; i++) 
        if (!strcmp(FUNC_BUFF[i]->name, TOKEN_BUFF[POS]->word)) right_part = get_func_call(text);

    if (!right_part)
    {
        right_part = get_expr(text);

        if (TOKEN_BUFF[POS]->type != TYPE_DOT)
            GET_SYNTAX_ERROR("ERROR in assignment. Dot in the end is missing");

        FREE();
    }

    middle_part->right_child           = right_part;
    if (right_part) right_part->parent = middle_part;

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    first_node->parent   = ret_node;
    ret_node->left_child = first_node;

    ret_node->left_child = middle_part;
    middle_part->parent  = ret_node;

    return ret_node;
}



// ============================================================== if zone ==============================================================


token_t* get_If(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_IF) return nullptr;

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    token_t* curr        = TOKEN_BUFF[POS];
    ret_node->left_child = TOKEN_BUFF[POS];

    TOKEN_BUFF[POS]->parent = ret_node;
    POS++;

    curr->left_child = get_logical_expr(text);
    if (curr->left_child) curr->left_child->parent = curr;

    if (TOKEN_BUFF[POS]->type == TYPE_IF_BRCKET) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in getting if. Close bracket (togda) is missing");

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in getting if. Open bracket (enter_mipt) is missing");

    token_t* if_code_tree   = get_comp(text);
    token_t* else_code_tree = nullptr;

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in getting if. Close bracket (get_sent_down) is missing");

    if (TOKEN_BUFF[POS]->type == TYPE_ELSE)
    {
        token_t* else_token = TOKEN_BUFF[POS];
        POS++;

        if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
        else
            GET_SYNTAX_ERROR("ERROR in getting if. Open bracket (enter_mipt) is missing");

        else_code_tree = get_comp(text);

        if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
        else
            GET_SYNTAX_ERROR("ERROR in getting if. Close bracket (get_sent_down) is missing");

        curr->right_child  = else_token;
        else_token->parent = curr;

        curr->right_child->left_child          = if_code_tree;
        if (if_code_tree) if_code_tree->parent = curr->right_child;

        curr->right_child->right_child             = else_code_tree;
        if (else_code_tree) else_code_tree->parent = curr->right_child;

        return ret_node;
    }

    curr->right_child                                = if_code_tree;
    if (curr->right_child) curr->right_child->parent = curr;

    return ret_node;
}


token_t* get_logical_expr(text_t* text)
{
    ASSERT(text);

    token_t* left_part = get_expr(text);

    if ((TOKEN_BUFF[POS]->type < TYPE_EQ) || (TOKEN_BUFF[POS]->type > TYPE_LESS_EQ))
        GET_SYNTAX_ERROR("ERROR in getting logical expression. Sign is missing");

    token_t* sign = TOKEN_BUFF[POS];
    POS++;

    token_t* right_part = get_expr(text);

    sign->left_child                 = left_part;
    if (left_part) left_part->parent = sign;

    sign->right_child                  = right_part;
    if (right_part) right_part->parent = sign;

    return sign;
}


// ============================================================== expr zone ==============================================================


token_t* get_expr(text_t* text)
{
    ASSERT(text);

    token_t* node1 = get_T(text);
    if (!node1) return nullptr;

    token_t* node = nullptr;

    while (TOKEN_BUFF[POS]->type == OP_ADD || TOKEN_BUFF[POS]->type == OP_SUB)
    {
        node = TOKEN_BUFF[POS];
        POS++;

        token_t* node2 = get_T(text);
        if (!node2) return nullptr;

        node->left_child  = node1;
        node->right_child = node2;

        node1->parent = node;
        node2->parent = node;

        node1 = node;
    }

    return node1;
}


token_t* get_T(text_t* text)
{
    ASSERT(text);

    token_t* node1 = get_P(text);
    if (!node1) return nullptr;

    token_t* node = nullptr;

    while (TOKEN_BUFF[POS]->type == OP_MUL || TOKEN_BUFF[POS]->type == OP_DIV)
    {
        node = TOKEN_BUFF[POS];
        POS++;

        token_t* node2 = get_T(text);
        if (!node2) return nullptr;

        node->left_child  = node1;
        node->right_child = node2;

        node1->parent = node;
        node2->parent = node;

        node1 = node;
    }

    return node1;
}


token_t* get_P(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type == TYPE_EXPR_O_BR)
    {
        FREE();
        token_t* expr_root = get_expr(text);

        if (TOKEN_BUFF[POS]->type != TYPE_EXPR_C_BR)
            GET_SYNTAX_ERROR("ERROR in getting expression. Expected )");

        FREE();

        return expr_root;
    }

    token_t* node = get_elem(text);
    return node;
}


token_t* get_elem(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type == TYPE_NUM)
    {
        POS++;
        return TOKEN_BUFF[POS-1];
    }

     if (TOKEN_BUFF[POS]->type == TYPE_VAR)
    {
        for (unsigned i = 0; i < VAR_CNT; i++)
        {
            if (!strcmp(VAR_BUFF[i]->name, TOKEN_BUFF[POS]->word))
            {
                //printf("%s - %s\n", VAR_BUFF[i]->name, TOKEN_BUFF[POS]->word);
                POS++;
                return TOKEN_BUFF[POS-1];
            }
        }
        GET_SYNTAX_ERROR("ERROR in getting expression. No such variable found");
    }
    GET_SYNTAX_ERROR("ERROR in getting expression. Expected number or variable");
}


// ============================================================== func zone ==============================================================


token_t* get_print(text_t* text)
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_PRINT) return nullptr;

    token_t* first_node = TOKEN_BUFF[POS];
    POS++;

    token_t* node = get_expr(text);
    first_node->left_child = node;

    if (node) node->parent = first_node;

    if (TOKEN_BUFF[POS]->type != TYPE_PRINT_BRCKET)
        GET_SYNTAX_ERROR("ERROR in print. Print bracket is missing");

    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_DOT)
        GET_SYNTAX_ERROR("ERROR in print. Dot in the end is missing");

    FREE();

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    first_node->parent   = ret_node;
    ret_node->left_child = first_node;

    return ret_node;
}


#define NEW_VAR()                                                        \
VAR_BUFF[VAR_CNT]       = (var_t*) calloc (1, sizeof(var_t));             \
VAR_BUFF[VAR_CNT]->name = (char*)  calloc (MAX_WORD_LEN, sizeof(char));    \
strncpy(VAR_BUFF[VAR_CNT]->name, TOKEN_BUFF[POS]->word, MAX_WORD_LEN-1);    \
VAR_CNT++;                                                                   \


token_t* get_def_function(text_t* text) 
{
    ASSERT(text);

    if (TOKEN_BUFF[POS]->type != TYPE_FUNC_INIT) return nullptr;
    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
        GET_SYNTAX_ERROR("ERROR in defining function. Error in name of function");

    token_t* first_node  = TOKEN_BUFF[POS];
    first_node->type     = TYPE_FUNC_ID;
    token_t* middle_node = TOKEN_BUFF[POS];

    for (unsigned i = 0; i < FUNC_CNT; i++)
        if (!strcmp(first_node->word, FUNC_BUFF[i]->name))
            GET_SYNTAX_ERROR("ERROR in defining function. Multiple definition of function");

    FUNC_BUFF[FUNC_CNT]       = (func_t*) calloc (1, sizeof(func_t));
    FUNC_BUFF[FUNC_CNT]->name = (char*)   calloc (MAX_WORD_LEN, sizeof(char));
    strncpy(FUNC_BUFF[FUNC_CNT]->name, TOKEN_BUFF[POS]->word, MAX_WORD_LEN-1);
    FUNC_CNT++;
    POS++;

    if (TOKEN_BUFF[POS]->type != TYPE_O_F_BR)
        GET_SYNTAX_ERROR("ERROR in defining function. Expexted [");

    FREE();

    if (TOKEN_BUFF[POS]->type != TYPE_VAR)
        GET_SYNTAX_ERROR("ERROR in defining function. Bed vars heheheh. word is not a variable");

    token_t* temp = TOKEN_BUFF[POS];

    int flag = 1;
    for (unsigned i = 0; i < VAR_CNT; i++)
        if (!strcmp(TOKEN_BUFF[POS]->word, VAR_BUFF[i]->name)) flag = 0;

    if (flag)
    {
        NEW_VAR();
    }

    FUNC_BUFF[FUNC_CNT-1]->var_cnt++;

    POS++;
    token_t* var_root  = create_node(TYPE_DOT, POISON, temp, nullptr);
    DOTS_BUFF[DOTS_CNT] = var_root;
    DOTS_CNT++;

    token_t* prev_node = var_root;

    if (temp) temp->parent = prev_node;

    while (TOKEN_BUFF[POS]->type == TYPE_VAR_SEPARATE)
    {
        FREE();

        if (TOKEN_BUFF[POS]->type != TYPE_VAR)
            GET_SYNTAX_ERROR("ERROR in defining function. Bed vars heheheh. word is not a variable");

        temp = create_node(TYPE_DOT, POISON, TOKEN_BUFF[POS], nullptr);
        DOTS_BUFF[DOTS_CNT] = temp;
        DOTS_CNT++;

        flag = 1;
        for (unsigned i = 0; i < VAR_CNT; i++)
        {
            if (!strcmp(TOKEN_BUFF[POS]->word, VAR_BUFF[i]->name)) flag = 0;
        }

        if (flag)
        {
            NEW_VAR();
        }
        FUNC_BUFF[FUNC_CNT-1]->var_cnt++;

        POS++;
        prev_node->right_child = temp;
        if (temp) temp->parent = prev_node;

        if (temp->left_child) temp->left_child->parent = temp;
        prev_node = temp;
    }

    if (TOKEN_BUFF[POS]->type != TYPE_C_F_BR)
        GET_SYNTAX_ERROR("ERROR in defining function. Expected [");

    FREE();

    if (TOKEN_BUFF[POS]->type == TYPE_O_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in defining function. Open bracket (enter_mipt) is missing");

    token_t* func_code = get_comp(text);

    if (TOKEN_BUFF[POS]->type == TYPE_C_BRCKT) FREE()
    else
        GET_SYNTAX_ERROR("ERROR in defining function. Close bracket (get_sent_down) is missing");

    middle_node->left_child  = var_root;
    if (var_root) var_root->parent = middle_node;

    middle_node->right_child = func_code;
    if (func_code) func_code->parent = middle_node;

    token_t* ret_node = nullptr;
    CREATE_RET_NODE();
    ret_node->left_child = first_node;
    first_node->parent   = ret_node;

    return ret_node;
}