#include <stdio.h>
#include "ast-print.h"

/* Helper Functions */
void PrintASTHelper(AST* ast, unsigned indent);
void PrintNodeContents(AST* ast);

/*
   Displays output for an entire AST.
*/
void PrintAST(AST* ast) {
  PrintASTHelper(ast, 0);
  printf("\n");
}

/*
   Helper function to display output for an entire AST. The indent tells the
   depth of the current node in the AST and will print a tab for each level
   (assuming the root is depth 0).
*/
void PrintASTHelper(AST* ast, unsigned indent) {
  for (unsigned i = 0; i < indent; i++) {
    printf("\t");
  }
  printf("(");
  PrintNodeContents(ast);
  for (unsigned i = 0; i < ast->size; i++) {
    printf("\n");
    PrintASTHelper(ast->children[i], indent + 1);
  }
  printf(")");
}

/*
   Helper Function to print information for a particular AST node based
   upon its type.
*/
void PrintNodeContents(AST* ast) {
  switch (ast->type) {
    case (NODETYPE_CONSTANT_INTEGER):
      printf("NODE = INTEGER %d, FILE = %s, LINENUM = %d", ast->data.integer,
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_CONSTANT_STRING):
      printf("NODE = STRING %s, FILE = %s, LINENUM = %d", ast->data.string,
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_CONSTANT_CHARACTER):
      printf("NODE = CHARACTER %c, FILE = %s, LINENUM = %d",
             ast->data.character, ast->filename, ast->linenum);
      break;
    case (NODETYPE_CONSTANT_TRUE):
      printf("NODE = TRUE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_CONSTANT_FALSE):
      printf("NODE = FALSE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_CONSTANT_NULL):
      printf("NODE = NULL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_CONTROL_IF_ELSE):
      printf("NODE = IF_ELSE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_CONTROL_FOR):
      printf("NODE = FOR, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_VAR_DECL):
      printf("NODE = VAR_DECL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_VAR_DECL_LIST):
      printf("NODE = VAR_DECL_LIST, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_FUNC_DECL):
      printf("NODE = FUNC_DECL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_FUNC_PARAMLIST):
      printf("NODE = PARAMLIST, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_FUNC_ARGLIST):
      printf("NODE = ARGLIST, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_TYPE):
      printf("NODE = TYPE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_TYPE_BOOL):
      printf("NODE = BOOL_TYPE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_TYPE_INT):
      printf("NODE = INT_TYPE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_TYPE_CHAR):
      printf("NODE = CHAR_TYPE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_TYPE_STRUCT):
      printf("NODE = STRUCT_TYPE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_TYPE_POINTER):
      printf("NODE = POINTER, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_CONST):
      printf("NODE = CONST, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_NOT_CONST):
      printf("NODE = NOT_CONST, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_STRUCT_DEF):
      printf("NODE = STRUCT, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_ADD):
      printf("NODE = BINARY_ADD, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_SUB):
      printf("NODE = BINARY_SUB, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_MULT):
      printf("NODE = BINARY_MULT, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_DIV):
      printf("NODE = BINARY_DIV, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_EQUAL):
      printf("NODE = BINARY_EQUAL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_NOTEQUAL):
      printf("NODE = BINARY_NOTEQUAL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_GREATEREQUAL):
      printf("NODE = BINARY_GREATERTHANEQUAL, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_GREATER):
      printf("NODE = BINARY_GREATERTHAN, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_LESSEQUAL):
      printf("NODE = BINARY_LESSTHANEQUAL, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_LESS):
      printf("NODE = BINARY_LESSTHAN, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_ANDAND):
      printf("NODE = BINARY_LOGICAL_AND, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_OROR):
      printf("NODE = BINARY_LOGICAL_OR, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_AND):
      printf("NODE = BINARY_BITWISE_AND, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_OR):
      printf("NODE = BINARY_BITWISE_OR, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_BINARY_XOR):
      printf("NODE = BINARY_BITWISE_XOR, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_PLUSPLUS):
      printf("NODE = PREFIX_PLUSPLUS, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_MINUSMINUS):
      printf("NODE = PREFIX_MINUSMINUS, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_NEGATE):
      printf("NODE = PREFIX_NEGATE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_ADDRESS):
      printf("NODE = PREFIX_ADDRESS, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_DEREFERENCE):
      printf("NODE = PREFIX_DEREFERENCE, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_NOTNOT):
      printf("NODE = PREFIX_LOGICAL_NOT, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_PREFIX_NOT):
      printf("NODE = PREFIX_BITWISE_NOT, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_POSTFIX_PLUSPLUS):
      printf("NODE = POSTFIX_PLUSPLUS, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_POSTFIX_MINUSMINUS):
      printf("NODE = POSTFIX_MINUSMINUS, FILE = %s, LINENUM = %d",
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_EXPR_STRUCT_DOT):
      printf("NODE = POSTFIX_DOT, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_STRUCT_ARROW):
      printf("NODE = POSTFIX_ARROW, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_CALL):
      printf("NODE = CALL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_CAST):
      printf("NODE = CAST, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_ASSIGN):
      printf("NODE = ASSIGN, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_ID):
      printf("NODE = ID %s, FILE = %s, LINENUM = %d", ast->data.identifier,
             ast->filename, ast->linenum);
      break;
    case (NODETYPE_GLOBAL):
      printf("NODE = GLOBAL, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_BLOCK):
      printf("NODE = BLOCK, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_BREAK):
      printf("NODE = BREAK, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_CONTINUE):
      printf("NODE = CONTINUE, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_RETURN):
      printf("NODE = RETURN, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    case (NODETYPE_EXPR_PREN):
      printf("NODE = PREN_EXPR, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
    default:
      printf("NODE = ERROR, FILE = %s, LINENUM = %d", ast->filename,
             ast->linenum);
      break;
  }
}
