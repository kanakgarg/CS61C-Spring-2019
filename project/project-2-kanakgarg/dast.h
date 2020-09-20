#ifndef DAST_H
#define DAST_H

#include <stddef.h>
#include "ast.h"

#define WORDSIZE 4
#define SAVED_REG_COUNT 12

/*
  Types of Decls
*/
enum DeclGroup {
  DECL_VAR,           /* Variable                          */
  DECL_DEFAULT_TYPE,  /* bool, int, and char               */
  DECL_STRUCT,        /* Declared Struct                   */
  DECL_FUNC_DEF,      /* Function fully defined            */
  DECL_FUNC_NO_DEF,   /* Function declaration without body */
  DECL_BLOCK,         /* Used for Blocks and Globals       */
  DECL_STR            /* Used for String Literals          */
};


/*
  Types of levels at which a declaration can be stored.
*/
enum DeclLevel {
  STRUCT,             /* Used for declarations of struct members.   */
  GLOBAL,             /* Used for declarations at the global level. */
  FUNCTION            /* Used for declarations inside functions.    */
};

/*
  Struct used to declare an element.
*/
typedef struct decl {
  char *identifier;             /* Holds the name of the declaration               */
  struct decl **types;          /* List of Decls to hold types                     */
  size_t *type_pointers;        /* List of Number of pointers each type has        */
  size_t type_size;             /* Numbe of type decl                              */
  size_t type_capacity;         /* Memory allocated for types.                     */
  enum DeclGroup group;         /* Used to identify the type of decl               */
  size_t decl_count;            /* Used to track when the declaration is in scope  */
  struct decl **children;       /* Used to track fields                            */
  size_t children_size;         /* Number of children                              */
  size_t children_capacity;     /* Memory allocated for children                   */
  struct decl *parent;          /* Pointer back to parent                          */
  int linenum;                  /* Linenumber the decl occurs on                   */
  char *filename;               /* File name the decl occurs in.                   */
  int is_const;                 /* If a variable cannot be a LHS except on init    */
  enum DeclLevel level;         /* Variable to track where a var should be offset  */
  int offset;                   /* Tracks how far offset a decl is.                */
  int data_size;                /* Tracks how many bytes a decl uses.              */
  union {
    char value [WORDSIZE];      /* Value the decl is initialized to.               */
    struct {
      char *str_literal;        /* Value of the string literal.                    */
      int str_size;             /* Length of the string literal.                   */
    } str_info;
  } contents;
  int is_string;                /* Whether or not a global var decl contains a string */
} Decl;


/*
  Decorated AST that tracks where the element is declared. 
*/
typedef struct dast {
  /* Essential for functionality */

  enum NodeType type;       /* What kind of node                  */
  union NodeData data;      /* Data needed for literals.          */
  struct dast** children;   /* Children of a given node           */
  size_t size;              /* How many children are there        */
  size_t capacity;          /* Memory allocated for children      */
  Decl *decl;               /* Where is the element declared      */
  size_t decl_count;        /* Used for scoping                   */

  /* Information used for error reporting. */

  int linenum;            /* What line does this node start at. */
  char *filename;         /* What file is this node in.         */
} DAST;

/*
  Takes in an AST and a declaration count number and returns
  a DAST from the AST initialized with the passed in count.
*/
DAST *ConvertAST (AST *ast, size_t count);

/*
  Takes in a DAST node and free all memory allocated for that particular
  node. It does not recurse onto its children.
*/
void FreeDASTNode (DAST *dast);


/*
  Takes in a group, identifier, declaration count, parent Decl, linenum,
  and filename, and creates a Decl initialized with those parameters.
*/
Decl *CreateDecl (enum DeclGroup group, char *identifier, size_t count, Decl *parent, int line, char *file, int is_const, enum DeclLevel level, int offset, int data_size);

/*
  Takes in a pointer to a declaration list, a pointer to its size,
  a pointer to its capacity, and a declaration, and appends the
  declaration to the declaration list, updating the size and possibly
  resizing if necessary.
*/
void AppendDecl (Decl ***decl_list, size_t *size_ptr, size_t *capacity_ptr, Decl *decl);

/*
  Takes in a Declaration list of a particular size and two decls, old
  and new, and replaces old in the declaration list with new if it
  exists. Otherwise it does nothing.
*/
void ReplaceDecl (Decl **decl_list, size_t size, Decl *new, Decl *old);

/*
  Takes in a declaration and frees all memory allocated in the declaration
  and all of its children.
*/
void FreeDecl (Decl *decl);

/*
  Takes in a pointer to a decl_list, a pointer to its size, a pointer to its
  capacity, a decl, a pointer to a list of pointers totals, and a pointer
  value. Appends the decl to the decl list passed in and appends the pointer
  to the pointer list, updating the size and resizing as necessary.
*/
void AppendType (Decl ***decl_list, size_t *size_ptr, size_t *capacity_ptr, Decl *decl, size_t **pointer_list, size_t pointer);

#endif
