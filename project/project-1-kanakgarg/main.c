#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C99 standard boolean type
#include <stdbool.h>

#include "preprocessor.h"
#include "tokenizer.h"
#include "tokens.h"
#include "utils.h"

/*
  Helper function to print display information about how to run the executable.
 */
void usage(char* binary) {
  printf("Usage: %s {flag} filename1 ... \n", binary);
  printf("Filename is the dinky1c program to interpret.  Flags supported\n");
  printf(" -t: TOKENIZE.  Just do the lexical analysis on the file\n");
}

/*
  Main function. Takes in a series of input files and either just tokenizes
  the input or also parses the input to follow the grammar depending on the
  flag being used.
 */
int main(int argc, char** argv) {
  bool tokenize = false;
  TokenList** tokens;
  char** files = NULL;
  int* boundaries = NULL;
  int i;
  int exitcode = 0;
  char* modeflag = argv[1];

  if (argc < 3) {
    usage(argv[0]);
    exit(-1);
  }

  if (!strcmp(modeflag, "-t")) {
    tokenize = true;
  } else {
    usage(argv[0]);
    exit(-1);
  }
  /* Preprocessor step to handle any includes */
  int err = ProcessIncludes(argv + 2, argc - 2, &files, &boundaries);
  if (err) {
    exit(1);
  }
  tokens = (TokenList**)malloc(sizeof(TokenList*) * boundaries[argc - 2]);
  if (tokens == NULL) {
    allocation_failed();
  }

  /* Actual lexing of input files. */
  for (i = 0; i < boundaries[argc - 2]; i++) {
    tokens[i] = TokenFile(files[i]);
  }
  if (tokenize) {
    for (i = 0; i < boundaries[argc - 2]; i++) {
      PrintTokens(tokens[i]);
    }
  }
  
  for (int i = 0; i < boundaries[argc - 2]; i++) {
    FreeTokenList(tokens[i]);
  }
  for (int i = 0; i < boundaries[argc - 2]; i++) {
    free(files[i]);
  }
  free(files);
  free(boundaries);
  free(tokens);

  /* Exit if any file has errored. */
  if (exitcode) {
    exit(exitcode);
  }
}
