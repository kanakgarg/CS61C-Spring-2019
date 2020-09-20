#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C99 standard boolean type
#include <stdbool.h>

#include "ast-print.h"
#include "ast.h"
#include "parser-errors.h"
#include "parser.h"
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
  printf(" -p: PARSE.  Both tokenize and parse the program\n");
}

/*
  Main function. Takes in a series of input files and either just tokenizes
  the input or also parses the input to follow the grammar depending on the
  flag being used.
 */
int main(int argc, char** argv) {
  bool tokenize = false;
  bool parse = false;
  TokenList** tokens;
  int lexer_errors[argc - 2];
  char** files = NULL;
  int* boundaries = NULL;
  AST** asts;
  int i;
  int exitcode = 0;
  char* modeflag = argv[1];

  if (argc < 3) {
    usage(argv[0]);
    exit(-1);
  }

  if (!strcmp(modeflag, "-t")) {
    tokenize = true;
  } else if (!strcmp(modeflag, "-p")) {
    parse = true;
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
  if (parse) {
    for (int i = 0; i < argc - 2; i++) {
      tokens[i] = tokens[boundaries[i]];
      for (int j = boundaries[i] + 1; j < boundaries[i + 1]; j++) {
        AppendTokenList(tokens[i], tokens[j]);
      }
    }
    asts = (AST**)malloc(sizeof(AST*) * (argc - 2));
    if (asts == NULL) {
      allocation_failed();
    }
    for (i = 0; i < (argc - 2); i++) {
      /* Only attempt to parse files with no lexing errors. */
      lexer_errors[i] = DisplayErrors(tokens[i]);
      if (lexer_errors[i] == 0) {
        asts[i] = ParseTokens(tokens[i], argv[i + 2]);
        exitcode = exitcode || CheckErrors(asts[i]);
        PrintAST(asts[i]);
      } else {
        exitcode = 1;
      }
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
  if (parse) {
    for (i = 0; i < (argc - 2); i++) {
      if (lexer_errors[i] == 0) {
        FreeAST (asts[i]);
      }
    }
    free (asts);
  }

  /* Exit if any file has errored. */
  if (exitcode) {
    exit(exitcode);
  }
}
