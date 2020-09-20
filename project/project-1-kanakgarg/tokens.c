#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*
  Produces a printed output a particular token.
*/
void PrintToken(Token* t) {
  printf("[Token: ");
  switch (t->type) {
    case TOKEN_IDENTIFIER:
      printf("Identifier: %s", t->data.identifier);
      break;
    case TOKEN_INTEGER:
      printf("Integer: %d", t->data.integer);
      break;
    case TOKEN_STRING:
      printf("String: %s", t->data.string);
      break;
    case TOKEN_CHARACTER:
      printf("Character: %c", t->data.character);
      break;
    case TOKEN_KW_INT:
      printf("int");
      break;
    case TOKEN_KW_CHAR:
      printf("char");
      break;
    case TOKEN_KW_CONST:
      printf("const");
      break;
    case TOKEN_KW_STRUCT:
      printf("struct");
      break;
    case TOKEN_KW_BOOL:
      printf("bool");
      break;
    case TOKEN_KW_FOR:
      printf("for");
      break;
    case TOKEN_KW_CONTINUE:
      printf("continue");
      break;
    case TOKEN_KW_BREAK:
      printf("break");
      break;
    case TOKEN_KW_IF:
      printf("if");
      break;
    case TOKEN_KW_ELSE:
      printf("else");
      break;
    case TOKEN_KW_TRUE:
      printf("true");
      break;
    case TOKEN_KW_FALSE:
      printf("false");
      break;
    case TOKEN_KW_RETURN:
      printf("return");
      break;
    case TOKEN_KW_NULL:
      printf("null");
      break;
    case TOKEN_SYM_PLUS:
      printf("+");
      break;
    case TOKEN_SYM_MINUS:
      printf("-");
      break;
    case TOKEN_SYM_TIMES:
      printf("*");
      break;
    case TOKEN_SYM_SLASH:
      printf("/");
      break;
    case TOKEN_SYM_PLUSPLUS:
      printf("++");
      break;
    case TOKEN_SYM_MINUSMINUS:
      printf("--");
      break;
    case TOKEN_EQUAL:
      printf("=");
      break;
    case TOKEN_EQUALEQUAL:
      printf("==");
      break;
    case TOKEN_BANGEQUAL:
      printf("!=");
      break;
    case TOKEN_GREATER:
      printf(">");
      break;
    case TOKEN_GREATEREQUAL:
      printf(">=");
      break;
    case TOKEN_LESS:
      printf("<");
      break;
    case TOKEN_LESSEQUAL:
      printf("<=");
      break;
    case TOKEN_SYM_NOTNOT:
      printf("!");
      break;
    case TOKEN_SYM_ANDAND:
      printf("&&");
      break;
    case TOKEN_SYM_OROR:
      printf("||");
      break;
    case TOKEN_SYM_NOT:
      printf("~");
      break;
    case TOKEN_SYM_AND:
      printf("&");
      break;
    case TOKEN_SYM_OR:
      printf("|");
      break;
    case TOKEN_SYM_XOR:
      printf("^");
      break;
    case TOKEN_SYM_DOT:
      printf(".");
      break;
    case TOKEN_SYM_ARROW:
      printf("->");
      break;
    case TOKEN_SYM_OPENPREN:
      printf("(");
      break;
    case TOKEN_SYM_CLOSEPREN:
      printf(")");
      break;
    case TOKEN_SYM_OPENBRACE:
      printf("{");
      break;
    case TOKEN_SYM_CLOSEBRACE:
      printf("}");
      break;
    case TOKEN_SYM_COMMA:
      printf(",");
      break;
    case TOKEN_SYM_SEMICOLON:
      printf(";");
      break;
    default:
      printf("Illegal symbol %s", t->data.error);
      break;
  }
  printf(" ]\n");
}

/*
  Mallocs space to create a new token.
*/
Token* create_token(char* filename) {
  Token* t = (Token*)malloc(sizeof(Token));
  if (t == NULL) {
    allocation_failed();
  }
  t->filename = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
  if (t->filename == NULL) {
    allocation_failed();
  }
  strcpy(t->filename, filename);
  return t;
}

/*
  Mallocs space to create a new list whose
  token is t.
*/
TokenList* create_list(Token* t) {
  TokenList* node = (TokenList*)malloc(sizeof(TokenList));
  if (node == NULL) {
    allocation_failed();
  }
  node->t = t;
  node->next = NULL;
  return node;
}

/*
  Frees the memory allocated by a particular token.
*/
void FreeToken(Token* t) {
  free(t->filename);
  switch (t->type) {
    case TOKEN_IDENTIFIER:
      free(t->data.identifier);
      break;
    case TOKEN_STRING:
      free(t->data.string);
      break;
    case TOKEN_ERR:
      free(t->data.error);
      break;
    default:;
  }
  free(t);
}

/*
  Frees the memory allocated by a Token_List.
*/
void FreeTokenList(TokenList* node) {
  if (node == NULL) {
    return;
  }
  FreeTokenList(node->next);
  FreeToken(node->t);
  free(node);
}

/*
  Takes two token lists and concatenates them together by placing
  next at the end of the first list.
*/
void AppendTokenList(TokenList* first, TokenList* next) {
  if (first == NULL) {
    return;
  }
  while (first->next != NULL) {
    first = first->next;
  }
  first->next = next;
}

/*
  Produces a printed output for all tokens in the list.
*/
void PrintTokens(TokenList* tl) {
  while (tl != NULL) {
    PrintToken(tl->t);
    tl = tl->next;
  }
}

/*
   Prints any erroneous tokens that were encountered.
   Returns 0 if no errors were encountered.
*/

int DisplayErrors(TokenList* tl) {
  int seen_errors = 0;
  while (tl != NULL) {
    if (tl->t->type == TOKEN_ERR) {
      fprintf(stderr, "Error in file %s line %d: Illegal symbol %s\n",
              tl->t->filename, tl->t->linenum, tl->t->data.error);
      seen_errors = 1;
    }
    tl = tl->next;
  }
  return seen_errors;
}
