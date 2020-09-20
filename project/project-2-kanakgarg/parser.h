#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include "ast.h"

/* 
  Enum of possible Binary Expr Function Choices
*/
enum BinaryExprType {
  BINARY_EXPR_LOGICAL_OR,
  BINARY_EXPR_LOGICAL_AND,
  BINARY_EXPR_BITWISE_OR,
  BINARY_EXPR_BITWISE_XOR,
  BINARY_EXPR_BITWISE_AND,
  BINARY_EXPR_EQUALITY,
  BINARY_EXPR_COMPARISON,
  BINARY_EXPR_ADDSUB,
  BINARY_EXPR_MULTDIV
};

/*
  Takes in a list of token and produces an AST for those tokens.
*/
AST* ParseTokens (TokenList *tokens, char *filename);


#endif
