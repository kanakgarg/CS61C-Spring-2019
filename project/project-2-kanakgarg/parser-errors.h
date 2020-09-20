#ifndef PARSER_ERRORS_H
#define PARSER_ERRORS_H

#include "ast.h"

/* 
  Tracks if when we have run out of tokens. Used to prevent redundant error
  messages.
*/
extern int eof_reached;

/*
  Used to track the name of the current file. This is show if an unexpect eof
  occurs we can determine in what file it occurred.
*/
extern char *curr_filename;

/*
  Function that takes in an AST and determines if any error has occurred.
  Returns 0 if no errors are in the AST and otherwise returns a nonzero value.
*/
int CheckErrors (AST *ast);

/* 
  Generates the error message for when there are not enough tokens to continue
  parsing rules. It switches eof_reached to 1 to prevent redundant print
  statements.
*/
void generate_eof_error (); 

/*
  Function to assist the output of error messages when a token is needed to
  complete a rule in the grammar but is not found.
*/
void generate_error_message (char *missing, char *filename, int linenum);

#endif
