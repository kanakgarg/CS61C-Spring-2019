#ifndef DECORATE_H
#define DECORATE_H

#include "dast.h"
#include "ast.h"

#define BLOCK_ID ""

/*
  Declarations for the program
*/
extern Decl *global;

enum literal_type {
  STRING_LITERAL,
  FALSE_LITERAL,
  TRUE_LITERAL,
  NULL_LITERAL,
  CHAR_LITERAL,
  INT_LITERAL,
  NOT_LITERAL
};

/*
  Data structure used to recursively track typing of various
  operations through the DAST.
*/
typedef struct type_info {
  Decl *decl;
  size_t pointer_count;
  int is_func;
  int valid_LHS;
  enum literal_type literal;
}Type_Info ;

/*
  Takes in an ast and the filename originally being build and converts
  the AST to a DAST, assigning declarations to all the identifiers and
  ensuring that all the types conform.
*/
DAST *DecorateTree (AST *ast, char *filename);

#endif
