#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"

/*
   Creates a new AST from a given type filename and linenum. You should not
   assume that filename will remain a legal pointer after this function
   terminates.
*/
AST* MakeAST(enum NodeType type, char* filename, int linenum) {
  AST* tree = (AST*)malloc(sizeof(AST));
  tree->type = type;
  tree->filename = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
  strcpy(tree->filename, filename);
  tree->linenum = linenum;
  tree->size = 0;
  tree->capacity = INITIAL_CAPACITY;
  tree->children = (AST**)malloc((tree->capacity) * sizeof(AST*));
  return tree;
}

/*
   Takes in a given AST and mallocs a new AST with the exact same details as the
   original. Again you should assume the original may be freed at any point
   after this function terminates.
*/
AST* CopyAST(AST* original) {
  AST* ast = MakeAST(original->type, original->filename, original->linenum);
  ast->size = original->size;
  ast->capacity = original->capacity;
  ast->children = (AST**)realloc(ast->children, sizeof(AST*) * ast->capacity);
  if (ast->children == NULL) {
    allocation_failed();
  }
  for (int i = 0; i < ast->size; i++) {
    ast->children[i] = CopyAST(original->children[i]);
  }

  /* Start Unique to Copy */
  if (ast->type == NODETYPE_ID) {
    ast->data.identifier =
        (char*)malloc(sizeof(char) * (strlen(original->data.identifier) + 1));
    if (ast->data.identifier == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.identifier, original->data.identifier);
  } else if (ast->type == NODETYPE_CONSTANT_STRING) {
    ast->data.string =
        (char*)malloc(sizeof(char) * (strlen(original->data.string) + 1));
    if (ast->data.string == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.string, original->data.string);
  }
  /* End of Unique to Copy */
  return ast;
}

/*
   Takes in an two ASTs and concatenates the two by making node a child
   of tree.
*/
void AppendAST(AST* tree, AST* node) {
  if ((tree->size + 1) >= tree->capacity) {
    tree->children = realloc(tree->children, (tree->capacity + 1)* sizeof(AST*));
    tree->capacity += 1;
  }
  tree->children[tree->size] = node;
  tree->size += 1;
}

/*
   Frees the memory allocated by a single AST node.
*/
void FreeNode(AST* ast) {

  free(ast->filename);
  if (ast-> type == NODETYPE_ID){
    free(ast->data.identifier);
  }
  if (ast->type == NODETYPE_CONSTANT_STRING){
    free(ast-> data.string);
  }
  free(ast->children);
  free(ast);
}

/*
   Frees all the memory allocated by an AST.
*/
void FreeAST(AST* ast) {
  for (int i = 0; i < ast-> size; i++){
    FreeAST(ast->children[i]);
  }
  FreeNode(ast);
}
