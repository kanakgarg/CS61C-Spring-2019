#ifndef PARSER_HELPERS_H
#define PARSER_HELPERS_H

#include "tokens.h"

/*
  Helper function to determine a particular token type is either
  an integer, a string, a character, true, false or null. 
*/
int is_constant (enum TokenType type);

/*
  Helper function to determine if a token is the bool, char,
  int, or struct keyword.
*/
int is_type_keyword (enum TokenType type);

/*
  helper function to determine if a token can start an expression.
*/
int is_expr_start (enum TokenType type);

/*
  Helper function to determine if the next token exists and is
  a valid postfix operator.
*/ 
int is_postfix_expr (TokenList *tokens);

/*
  Helper functions for processing tokens. If the next token exists
  and matches type it replaces token with the next token and returns
  a nonzero value. Otherwise it returns 0 and does not modify tokens.
*/
int ProcessToken (TokenList **tokens, enum TokenType type);

#endif
