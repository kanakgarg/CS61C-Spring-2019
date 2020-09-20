#ifndef TOKEN_H
#define TOKEN_H

/* Definition for Dinky-1C token */
enum TokenType {
  TOKEN_IDENTIFIER,
  TOKEN_INTEGER,
  TOKEN_STRING,
  TOKEN_CHARACTER,
  
  TOKEN_KW_INT,      /* Keyword "int"      */
  TOKEN_KW_CHAR,     /* Keyword "char"     */
  TOKEN_KW_CONST,    /* Keyword "const"    */
  TOKEN_KW_STRUCT,   /* Keyword "struct"   */
  TOKEN_KW_BOOL,     /* Keyword "bool"     */
  TOKEN_KW_IF,       /* Keyword "if"       */
  TOKEN_KW_ELSE,     /* Keyword "else"     */
  TOKEN_KW_TRUE,     /* Keyword "true"     */
  TOKEN_KW_FALSE,    /* Keyword "false"    */
  TOKEN_KW_RETURN,   /* Keyword "return"   */
  TOKEN_KW_FOR,      /* Keyword "for"      */
  TOKEN_KW_CONTINUE, /* Keyword "continue" */
  TOKEN_KW_BREAK,    /* Keyword "break"    */
  TOKEN_KW_NULL,     /* Keyword "null"     */
  
  /* Math Tokens */
  TOKEN_SYM_PLUS,       /* + */
  TOKEN_SYM_MINUS,      /* - */
  TOKEN_SYM_TIMES,      /* * */
  TOKEN_SYM_SLASH,      /* / */
  TOKEN_SYM_PLUSPLUS,   /* ++ */
  TOKEN_SYM_MINUSMINUS, /* -- */

  /* Assignment and equality */
  TOKEN_EQUAL,      /* = */
  TOKEN_EQUALEQUAL, /* == */
  TOKEN_BANGEQUAL,  /* != */

  /* Comparison Tokens */
  TOKEN_GREATER,       /* > */
  TOKEN_GREATEREQUAL,  /* >= */
  TOKEN_LESS,          /* < */
  TOKEN_LESSEQUAL,     /* <= */
  
  /* Logical Tokens */
  TOKEN_SYM_ANDAND,  /* && */
  TOKEN_SYM_OROR,   /* || */
  TOKEN_SYM_NOTNOT, /* ! */

  /* Bitwise Tokens */
  TOKEN_SYM_AND,  /* & */
  TOKEN_SYM_OR,   /* | */
  TOKEN_SYM_XOR,  /* ^ */
  TOKEN_SYM_NOT,  /* ~ */

  /* Struct Tokens */
  TOKEN_SYM_DOT,   /* . */
  TOKEN_SYM_ARROW, /* -> */

  /* Misc symbols */
  TOKEN_SYM_OPENPREN,   /* ( */
  TOKEN_SYM_CLOSEPREN,  /* ) */
  TOKEN_SYM_OPENBRACE,  /* { */
  TOKEN_SYM_CLOSEBRACE, /* } */
  TOKEN_SYM_COMMA,	/* , */
  TOKEN_SYM_SEMICOLON,  /* ; */

  /* An error */
  TOKEN_ERR
};


/* For tokens which have semantics, we use the
   union to start it */
typedef union tokendata {
  char *string;
  char *identifier;
  char character;
  int integer;

  /* Used for error reporting. */
  char *error;
} TokenData;

typedef struct token {
  enum TokenType type;
  TokenData data;

  /* Used for error reporting */
  int linenum;
  char *filename;
} Token;

typedef struct tokenlist {
  Token *t;
  struct tokenlist *next;
} TokenList;

/*
  Mallocs space to create a new token.
*/
Token *create_token ();

/*
  Mallocs space to create a new list whose
  token is t.
*/
TokenList *create_list (Token *t);

/*
  Frees the memory allocated by a particular token.
*/
void FreeToken (Token *t);

/*
  Frees the memory allocated by a Token_List.
*/
void FreeTokenList (TokenList *node);

/* 
  Takes two token lists and concatenates them together by placing 
  next at the end of the first list.
*/
void AppendTokenList (TokenList *first, TokenList *next);

/*
  Produces a printed output a particular token.
*/
void PrintToken(Token *t);

/*
  Produces a printed output for all tokens in the list.
*/
void PrintTokens(TokenList *tl);

/* 
  Used for reporting any errors. Returns 0 if there are no errors.
*/
int DisplayErrors(TokenList *tl);
#endif
