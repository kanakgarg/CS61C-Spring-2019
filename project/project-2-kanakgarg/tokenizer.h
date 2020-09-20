#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tokens.h"

/*
  Takes in a filename and parses the file contents into a series of tokens.
  Returns a the list of tokens in the file or NULL if there are no tokens
  in the file. Files can have unbounded total input size as well as unbounded
  line size (see the spec for grading breakdown if you do not handle unbounded
  input sizes). If a file cannot be opened or memory cannot be allocated the
  program should exit with a nonzero exit code. It is NEVER acceptable for the
  program to SEGFAULT. 
*/
TokenList *TokenFile(char *filename);

#endif
