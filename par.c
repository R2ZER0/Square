/* Parser */

/* expr := [statement] => expr
 * statement := [ITEM] ( ';' | '\0' ) => statement
 * ITEM := '(' expr ')' => subexpr
         |  string => string
         |  number => number
         |  variable => variable
         |  '[' expr ']' => block
 */

#include <stdlib.h>
#include "par.h"
#include "tok.h"

const char* ast_type_strmap[_NUM_AST_TYPES] = {
	"AST_ERROR",
	"AST_EXPRESSION",
	"AST_STATEMENT",
	"AST_STRING",
	"AST_NUMBER",
	"AST_VARIABLE",
	"AST_BLOCK"
};

#define MAX_AST_NODES (16*1024)

ast_node _alloc_ast_node[MAX_AST_NODES];
int _alloc_ast_node_next = 0;

ast_node* new_ast_node(ast_type type) {
	if(_alloc_ast_node_next >= MAX_AST_NODES) {
		return NULL;
	}

	ast_node* node = &_alloc_ast_node[_alloc_ast_node_next++];
	node->type = type;
	node->next = NULL;

	return node;
}

ast_node* new_ast_node_sub(ast_type type, ast_node_list* sub) {
	ast_node* node = new_ast_node(type);
	node->sub = sub;
	return node;
}

ast_node* new_ast_node_str(ast_type type, const char* str) {
	ast_node* node = new_ast_node(type);
	node->str = str;
	return node;
}

ast_node* new_ast_node_num(ast_type type, int num) {
	ast_node* node = new_ast_node(type);
	node->num = num;
	return node;
}

void ast_node_list_push(ast_node_list* list, ast_node* node) {
	if(list->first == NULL) {
		list->first = node;
	}
	if(list->last != NULL) {
		list->last->next = node;
	}
	list->last = node;
}

void next_tok(token** tokp) {
	*tokp = *tokp->next;
}

int parse_expression(ast_node** expr_node, token** tokp) {
	ast_node* node = new_ast_node(AST_EXPRESSION);
	ast_node_list* statements = node->sub;

	if(tok->type == TOKEN_START) {
		next_tok(tokp);
	}

	if(*tokp == NULL) {
		printf("%s\n", "Parse error: Token null");
		return -1;
	}

	if(*tokp->type == VARIABLE) {
		ast_node_list_push(statements, new_ast_node_str(AST_VARIABLE, *tokp->str));

	} else if(*tokp->type == TOKEN_LEFT_PAREN) {
		ast_node** subexpr = NULL;
		int err = parse_expression(subexpr, tokp);

		if(err < 0) {
			printf("Failed to parse subexpr\n");
			return err;
		}
	}


	*expr_node = node; 
	return 0;
}