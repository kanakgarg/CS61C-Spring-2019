#include <stddef.h>
#include "parser-helpers.h"

/*
  Helper function to determine a particular token type is either
  an integer, a string, a character, true, false or null.
*/
int is_constant(enum TokenType type) {
  return type == TOKEN_INTEGER || type == TOKEN_STRING ||
         type == TOKEN_CHARACTER || type == TOKEN_KW_TRUE ||
         type == TOKEN_KW_FALSE || type == TOKEN_KW_NULL;
}

/*
  Helper function to determine if a token is the bool, char,
  int, or struct keyword.
*/
int is_type_keyword(enum TokenType type) {
  return type == TOKEN_KW_CHAR || type == TOKEN_KW_INT ||
         type == TOKEN_KW_BOOL || type == TOKEN_KW_STRUCT;
}

/*
  Helper function to determine if a token is the bool, char,
  int, or struct keyword.
*/
int is_expr_start(enum TokenType type) {
  return is_constant(type) || type == TOKEN_SYM_OPENPREN ||
         type == TOKEN_SYM_TIMES || type == TOKEN_SYM_PLUSPLUS ||
         type == TOKEN_SYM_MINUSMINUS || type == TOKEN_IDENTIFIER ||
         type == TOKEN_SYM_AND || type == TOKEN_SYM_NOT ||
         type == TOKEN_SYM_MINUS || type == TOKEN_SYM_NOTNOT;
}

/*
  Helper function to determine if the next token exists and is
  a valid postfix operator.
*/
int is_postfix_expr(TokenList* tokens) {
  return tokens != NULL && (tokens->t->type == TOKEN_SYM_ARROW ||
                            tokens->t->type == TOKEN_SYM_DOT ||
                            tokens->t->type == TOKEN_SYM_PLUSPLUS ||
                            tokens->t->type == TOKEN_SYM_MINUSMINUS);
}

/*
  Helper functions for processing tokens. If the next token exists
  and matches type it replaces token with the next token and returns
  a nonzero value. Otherwise it returns 0 and does not modify tokens.
*/
int ProcessToken(TokenList** tokens, enum TokenType type) {
  if (*tokens == NULL || ((*tokens)->t->type != type)) {
    return 0;
  } else {
    *tokens = (*tokens)->next;
    return 1;
  }
}
