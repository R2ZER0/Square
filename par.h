/* Par H */
#ifndef _PAR_H_
#define _PAR_H_

typedef enum ast_type_t ast_type;
typedef struct ast_node_t ast_node;
typedef struct ast_node_list_t ast_node_list;

enum ast_type_t {
	AST_ERROR = 0,
	AST_EXPRESSION,
	AST_STATEMENT,
	AST_STRING,
	AST_NUMBER,
	AST_VARIABLE,
	AST_BLOCK,
	_NUM_AST_TYPES
};

struct ast_node_t {
	ast_type type;
	ast_node* next;
	union {
		ast_node_list* sub;
		const char* str;
		int num;
	};
};

const char* ast_type_strmap[_NUM_AST_TYPES];

struct ast_node_list_t {
	ast_node* first;
	ast_node* last;
};

#endif /* _PAR_H_ */