#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C99 standard boolean type
#include <stdbool.h>

#include "utils.h"
#include "preprocessor.h"
#include "tokens.h"
#include "tokenizer.h"
#include "parser.h"
#include "parser-errors.h"
#include "ast.h"
#include "ast-print.h"
#include "decorate.h"
#include "decorate-errors.h"
#include "dast.h"
#include "dast-print.h"
#include "cgen.h"

/*
  Helper function to print display information about how to run the executable.
 */
void usage(char *binary){
  printf("Usage: %s {flag} filename1 ... \n", binary);
  printf("Filename is the dinky1c program to interpret.  Flags supported\n");
  printf(" -t: TOKENIZE.  Just do the lexical analysis on the file\n");
  printf(" -p: PARSE.  Both tokenize and parse the program\n");
  printf(" -d: DECORATE.  Tokenize, parse, and decorate the program\n");
  printf(" -c: CODE GEN.  Tokenize, parse, decorate, and generate assembly for the program\n");
}

int main(int argc, char **argv){
  bool tokenize = false;
  bool parse = false;
  bool decorate = false;
  bool cgen = false;
  TokenList **tokens;
  char **files = NULL;
  int *boundaries = NULL;
  AST **asts;
  DAST **dasts;
  int i;
  int exitcode = 0;
  char *modeflag = argv[1];
  
  if (argc < 3) {
    usage(argv[0]);
    exit(-1);
  }

  if (!strcmp(modeflag, "-t")) {
    tokenize = true;
  } else if (!strcmp(modeflag, "-p")) {
    parse = true;
  } else if (!strcmp(modeflag, "-d")) {
    decorate = true;
  } else if (!strcmp(modeflag, "-c")) {
    cgen = true;
  } else {
    usage(argv[0]);
    exit(-1);
  }
  int err = ProcessIncludes (argv + 2, argc - 2, &files, &boundaries);
  if (err) {
    exit (1);
  }
  tokens = (TokenList **) malloc (sizeof (TokenList *) * boundaries[argc - 2]);
  if (tokens == NULL) {
    allocation_failed ();
  }


  for (i = 0; i < boundaries[argc - 2]; i++){
    tokens[i] = TokenFile(files[i]);
  }
  if (tokenize) {
    for (i = 0; i < boundaries[argc - 2]; i++){
      PrintTokens(tokens[i]);
    }
  }
  if (parse || decorate || cgen) {
    for (int i = 0; i < argc - 2; i++) {
      tokens[i] = tokens[boundaries[i]];
      for (int j = boundaries[i] + 1; j < boundaries[i + 1]; j++) {
        AppendTokenList (tokens[i], tokens[j]);
      }
    }
    asts = (AST **) malloc(sizeof (AST *) * (argc - 2));
    if (asts == NULL) {
      allocation_failed ();
    }
    for (i = 0; i < (argc - 2); i++) {
      if (DisplayErrors (tokens[i]) == 0) {
        asts[i] = ParseTokens (tokens[i], argv[i + 2]);
	      exitcode = exitcode || CheckErrors (asts[i]);
        if (parse) {
          PrintAST (asts[i]);
        }
      } else {
        exitcode = 1;
      }
    }
  }
  for (int i = 0; i < argc - 2; i++) {
    FreeTokenList (tokens[i]);
  }
  for (int i = 0; i < boundaries[argc - 2]; i++) {
    free (files[i]);
  }
  free (files);
  free (boundaries);
  free (tokens);
  if (exitcode) {
    exit (exitcode);
  }
  if (decorate || cgen) {
    dasts = (DAST **) malloc(sizeof (DAST *) * (argc - 2));
    if (dasts == NULL) {
      allocation_failed ();
    }
    for (int i = 0; i < (argc - 2); i++) {
      dasts[i] = DecorateTree (asts[i], argv[i + 2]);
      if (decorate) {
        PrintDAST (dasts[i]);
      }   
    }
  }
  exitcode = error_code;
  if (exitcode) {
    exit (exitcode);
  }
  if (cgen) {
    // do cgen-ish things
    for (int i = 0; i < (argc - 2); i++) {
      GenerateCode (dasts[i], argv[i + 2]);
      // PrintCode
    }
  }
  if (decorate || parse || cgen) {
    for (int i = 0; i < (argc - 2); i++) {
      FreeAST (asts[i]);
    }
    free (asts);
  }
  return 0;
}
