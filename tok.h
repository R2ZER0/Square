#ifndef _TOK_H_
#define _TOK_H_

#define MAX_TOKENS (16*1024)

typedef enum token_type_t {
	TOKEN_ERROR = 0,
	TOKEN_START,
	TOKEN_END,
	TOKEN_VARIABLE,
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_LEFT_SQUARE,
	TOKEN_RIGHT_SQUARE,
	TOKEN_STATEMENT_END,
	_NUM_TOKEN_TYPES
} token_type;

const char* token_type_strmap[_NUM_TOKEN_TYPES];

typedef struct token_t token;
struct token_t {
	token* next;
	token_type type;
	union {
		const char* str;
		int num;
	};
};

typedef struct token_list_t token_list;
struct token_list_t {
	token* first;
	token* last;
};

typedef struct token_cursor_t {
	const char* p;
	int index;
} token_cursor;


int tokenise(token_list* tokens, token_cursor* cur);
void print_token(token* tok);
void print_token_list(token_list* tokenlist);

#endif /* _TOK_H_ */