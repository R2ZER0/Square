#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

const char* program = "print Hello World";

enum EVAL_STATUS {
               EVAL_OK,
               EVAL_ERR_UNKNOWN,
};

enum TOK_TYPE {
               TOK_START,
               TOK_BLOCK,
               TOK_IDENT,
               TOK_VALUE,
};

struct token {
  const char* ident;
  TOK_TYPE type;
  token* next;
  token* block;
};

token* new_token(TOK_TYPE type) {
  token* t = (token*) calloc(1, sizeof(token));
  t->type = type;
  t->ident = NULL;
  t->next = NULL;
  t->block = NULL;
  return t;
}

token* new_token_ident(const char* ident) {
  token* t = new_token(TOK_IDENT);
  t->ident = ident;
  return t;
}

typedef enum EVAL_STATUS EVAL_STATUS;

bool is_valid_char(char c) {
  return (isalnum(c) || isspace(c) || c == '[' || c == ']');
}

struct ident_list {
  const char* ident;
  ident_list* next;
};

ident_list* new_ident_list(const char* ident) {
  ident_list* il = (ident_list*)calloc(1, sizeof(ident_list));
  il->ident = ident;
  il->next = NULL;
  return il;
};

const char next(const char** p) {
  return *(++*p);
}

const char peek(const char** p) {
  return **p;
}

bool check_valid(const char p) {
  if(p == '\0') {
    printf("Null char found");
    exit(-1);
  } else if(!is_valid_char(p)) {
    printf("Unknown char found");
    exit(-1);
  } else {
    return true;
  }
}

const char* new_string(const char* from, int length) {
  char* s = (char*)calloc(length+1, sizeof(char));
  strncpy(s, from, length+1);
  return (const char*)s;
}

void skip_white(const char** p) {
  while(is_valid_char(peek(p)) && isspace(peek(p))) { next(p); }
}

bool is_alnum(const char c) {
  return ((c >= '0' && c <= '9') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= 'a' && c <= 'z'));
}

const char* parse_ident(const char** p) {
  const char* end = *p;

  while(is_alnum(*end)) { ++end; }

  const char* s = new_string(*p, end-*p);
  *p = end;

  return new_token_ident(s);
}

token* parse_block(const char** p) {
  token* block = new_token(TOK_BLOCK);
  token* last_in_block = block;

  while(is_valid_char(peek(p))) {
    skip_white(p);
    if(peek(p) == ']') {
      // end block
      return block;
    } else if (peek(p) == '[') {
      // new nested block
      next(p);
      token* subblock = parse_block(p);
    }
    token* ident = parse_ident(p);
    last_in_block->next = ident;
    last_in_block = ident;
  }

  return il;
}

EVAL_STATUS eval_statement(const char* p) {
  skip_white(&p);

  ident_list* ilist = gather_ident_list(&p);

  for(ident_list* il = ilist; il != NULL; il = il->next) {
    printf("%s, ", il->ident);
  }

  return EVAL_OK;
}


int main(int argc, char** argv) {
  eval_statement(program);
}
