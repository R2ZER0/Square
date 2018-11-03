/* Tokeniser
 *
 * Types of Token:
 * - variable lookup
 * - right paren
 * - left paren
 * - string literal
 * - number literal
 * - left square
 * - right square
 */

#include <stdlib.h>
#include <stdio.h>

const char* example = "def (concat \"thing_\" $var1) [print \"Hello, world!\" (+ 2 $1)]";

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
	TOKEN_RIGHT_SQUARE
} token_type;

const char* token_type_strmap[] = {
	"TOKEN_ERROR",
	"TOKEN_START",
	"TOKEN_END",
	"TOKEN_VARIABLE",
	"TOKEN_LEFT_PAREN",
	"TOKEN_RIGHT_PAREN",
	"TOKEN_STRING",
	"TOKEN_NUMBER",
	"TOKEN_LEFT_SQUARE",
	"TOKEN_RIGHT_SQUARE",
};

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

void push_token(token_list* list, token* next_tok) {
	if(list->first == NULL) {
		list->first = next_tok;
	}
	if(list->last != NULL) {
		list->last->next = next_tok;
	}
	list->last = next_tok;
}

token _alloc_tokens[MAX_TOKENS] = {0};
int _alloc_tokens_next = 0;

token* new_token(token_type type) {
	if(_alloc_tokens_next >= MAX_TOKENS) {
		return NULL;
	}

	token* tok = &_alloc_tokens[_alloc_tokens_next++];
	tok->next = NULL;
	tok->type = type;
	tok->str = NULL;
	return tok;
}

token* new_token_str(token_type type, const char* program, int start_index, int end_index) {
	int length = end_index - start_index;
	char* str = (char*) calloc(length+1, sizeof(char));

	if(str == NULL) {
		return NULL;
	}

	for(int i = 0; i < length; ++i) {
		str[i] = program[start_index + i];
	}

	token* tok = new_token(type);
	tok->str = (const char*)str;
	return tok;
}

token* new_token_num(token_type type, int num) {
	token* tok = new_token(type);
	tok->num = num;
	return tok;
}

typedef struct cursor_t {
	const char* p;
	int index;
} cursor;

char peek(cursor* c) {
	return c->p[c->index];
}

char next(cursor* c) {
	c->index++;
	return peek(c);
}

int tokenise_number(token_list* tokens, cursor* cur) {
	int num = 0;
	char c = peek(cur);
	while(c >= '0' && c <= '9') {
		num *= 10;
		num += (c - '0');
		c = next(cur);
	}
	push_token(tokens, new_token_num(TOKEN_NUMBER, num));
	return 0;
}

int tokenise_string(token_list* tokens, cursor* cur) {
	int quoted = 0;

	if(peek(cur) == '"') {
		quoted = 1;
		next(cur);
	}

	char c;
	int start_index = cur->index;
	int end_index = start_index-1;
	while(1) {
		c = peek(cur);
		if( (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			(quoted && c != '"' && c != '\0')) {
			end_index++;
			next(cur);
		} else {
			break;
		}
	}

	if(quoted) {
		if(c == '"') {
			next(cur);
		} else if(c == '\0') {
			printf("Unexpected end in quoted string");
			return -1;
		} else {
			printf("Unexpected char '%c' in quoted string", c);
			return -1;
		}
	}

	push_token(tokens, new_token_str(TOKEN_STRING, cur->p, start_index, end_index));
	return 0;
}

int tokenise(token_list* tokens, cursor* cur) {
	push_token(tokens, new_token(TOKEN_START));

	while(1) {
		char c = peek(cur);
		if(c == ' ' || c == '\n' || c == '\t') {
			// whitespace, no token
			next(cur);

		} else if(c == '(') {
			push_token(tokens, new_token(TOKEN_LEFT_PAREN));
			next(cur);

		} else if(c == ')') {
			push_token(tokens, new_token(TOKEN_RIGHT_PAREN));
			next(cur);

		} else if(c == '[') {
			push_token(tokens, new_token(TOKEN_LEFT_SQUARE));
			next(cur);

		} else if(c == ']') {
			push_token(tokens, new_token(TOKEN_RIGHT_SQUARE));
			next(cur);

		} else if(c >= '0' && c <= '9') {
			int err = tokenise_number(tokens, cur);

			if(err < 0) {
				return err;
			}

		} else if(c == '\0') {
			push_token(tokens, new_token(TOKEN_END));
			return 0;

		} else if((c == '"') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			int err = tokenise_string(tokens, cur);

			if(err < 0) {
				return err;
			}

		} else {
			printf("Unknown Token '%c'\n", c);
			return -1;
		}
	}
}

void print_token_list(token_list* tokenlist) {
	token* t = tokenlist->first;
	while(t != NULL) {
		printf("%s\n", token_type_strmap[t->type]);
		t = t->next;
	}
}

int main() {
	cursor cur;
	cur.p = example;
	cur.index = 0;

	token_list tokenlist;
	tokenlist.first = NULL;
	tokenlist.last = NULL;

	tokenise(&tokenlist, &cur);

	print_token_list(&tokenlist);

	return 0;
}