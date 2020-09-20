#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "ast.h"
#include "parser.h"
#include "parser-helpers.h"
#include "parser-errors.h"

/* Functions that generate AST elements */
AST* Global(TokenList** tokens);
AST* Global_Decl(TokenList** tokens);
AST* Constant(TokenList** tokens);
AST* Star(TokenList** tokens);
AST* ID(TokenList** tokens);
AST* Type(TokenList** tokens);
AST* StructDef(TokenList** tokens);
AST* Func(TokenList** tokens);
AST* ParamList(TokenList** tokens);
AST* CallExpr(TokenList** tokens);
AST* ExprList(TokenList** tokens);
AST* IfElse(TokenList** tokens);
AST* Expr(TokenList** tokens);
AST* VariableDeclList(TokenList** tokens);
AST* StructVariableDeclList(TokenList** tokens);
AST* ContinueExpr(TokenList** tokens);
AST* BreakExpr(TokenList** tokens);
AST* ReturnExpr(TokenList** tokens);
AST* ForLoop(TokenList** tokens);
AST* OptionalExpr(TokenList** tokens);
AST* Block(TokenList** tokens);
AST* BinaryExprLogicalOr(TokenList** tokens);
AST* BinaryExprLogicalAnd(TokenList** tokens);
AST* BinaryExprBitwiseOr(TokenList** tokens);
AST* BinaryExprBitwiseXor(TokenList** tokens);
AST* BinaryExprBitwiseAnd(TokenList** tokens);
AST* BinaryExprEquality(TokenList** tokens);
AST* BinaryExprComparison(TokenList** tokens);
AST* BinaryExprAddSub(TokenList** tokens);
AST* BinaryExprMultDiv(TokenList** tokens);
AST* BinaryExpr(TokenList** tokens,
                enum TokenType* possible_tokens,
                enum NodeType* ast_choices,
                size_t size,
                enum BinaryExprType type);
AST* GetNextBinaryExpr(TokenList** tokens, enum BinaryExprType type);
AST* PostfixExpr(TokenList** tokens);
AST* PrefixExpr(TokenList** tokens);
AST* BaseExpr(TokenList** tokens);
AST* CastExpr(TokenList** tokens);
AST* Expr(TokenList** tokens);

/*
  Takes in a list of token and produces an AST for those tokens.
*/
AST* ParseTokens(TokenList* tokens, char* filename) {
  curr_filename = filename;
  /* Create the global node */
  return Global(&tokens);
}

/*
  Function for producing the global AST node
*/
AST* Global(TokenList** tokens) {
  AST* ast = MakeAST(NODETYPE_GLOBAL, curr_filename, 0);
  int is_error = 0;
  while (*tokens != NULL) {
    while (is_error) {
      if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON)) {
        is_error = 0;
      } else if (*tokens == NULL) {
        is_error = 0;
      } else {
        *tokens = (*tokens)->next;
      }
    }
    
    while (ProcessToken(tokens, TOKEN_SYM_SEMICOLON)) {
    }
    if (*tokens == NULL) {
      break;
    }
    Token* curr_token = (*tokens)->t;
    if ((*tokens)->t->type == TOKEN_KW_STRUCT) {
      TokenList* nextToken = (*tokens)->next;
      if (nextToken == NULL) {
        /* Error */
        generate_eof_error();
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        break;
      } else if (nextToken->t->type != TOKEN_IDENTIFIER) {
        /* Error */
        curr_token = (*tokens)->t;
        generate_error_message("Identifier", curr_token->filename,
                               curr_token->linenum);
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        is_error = 1;
        continue;
      }
      curr_token = (*tokens)->t;
      nextToken = nextToken->next;
      if (nextToken == NULL) {
        /* Error */
        generate_eof_error();
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        break;
      } else if (nextToken->t->type == TOKEN_SYM_OPENBRACE) {
        AppendAST(ast, StructDef(tokens));
      } else {
        while (nextToken != NULL && nextToken->t->type == TOKEN_SYM_TIMES) {
          curr_token = (*tokens)->t;
          nextToken = nextToken->next;
        }
        if (nextToken == NULL) {
          /* Error */
          generate_eof_error();
          AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                                 curr_token->linenum));
          break;
        } else if (nextToken->t->type == TOKEN_KW_CONST) {
          AppendAST(ast, VariableDeclList(tokens));
        } else {
          if (nextToken->t->type != TOKEN_IDENTIFIER) {
            /* Error */
            curr_token = (*tokens)->t;
            generate_error_message("Identifier", curr_token->filename,
                                   curr_token->linenum);
            AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                                   curr_token->linenum));
            is_error = 1;
            continue;
          }
          curr_token = (*tokens)->t;
          nextToken = nextToken->next;
          if (nextToken == NULL) {
            /* Error */
            generate_eof_error();
            AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                                   curr_token->linenum));
            break;
          } else if (nextToken->t->type == TOKEN_SYM_OPENPREN) {
            AppendAST(ast, Func(tokens));
          } else {
            AppendAST(ast, VariableDeclList(tokens));
          }
        }
      }
    } else if (is_type_keyword((*tokens)->t->type)) {
      TokenList* nextToken = (*tokens)->next;
      while (nextToken != NULL && nextToken->t->type == TOKEN_SYM_TIMES) {
        curr_token = (*tokens)->t;
        nextToken = nextToken->next;
      }
      if (nextToken == NULL) {
        /* Error */
        generate_eof_error();
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        break;
      } else if (nextToken->t->type == TOKEN_KW_CONST) {
        AppendAST(ast, VariableDeclList(tokens));
      } else if (nextToken->t->type != TOKEN_IDENTIFIER) {
        /* Error */
        curr_token = (*tokens)->t;
        generate_error_message("Identifier", curr_token->filename,
                               curr_token->linenum);
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        is_error = 1;
        continue;
      } else {
        curr_token = (*tokens)->t;
        nextToken = nextToken->next;
        if (nextToken == NULL) {
          /* Error */
          generate_eof_error();
          AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                                 curr_token->linenum));
          break;
        } else if (nextToken->t->type == TOKEN_SYM_OPENPREN) {
          AppendAST(ast, Func(tokens));
        } else {
          AppendAST(ast, VariableDeclList(tokens));
        }
      }
    } else if ((*tokens)->t->type == TOKEN_KW_CONST) {
      AppendAST(ast, VariableDeclList(tokens));
    } else {
      /* Error */
      generate_error_message("Global Statement", curr_token->filename,
                             curr_token->linenum);
      AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                             curr_token->linenum));
      is_error = 1;
    }
  }
  return ast;
}

/*
  Function for producing the AST nodes for any constants
*/
AST* Constant(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_INTEGER)) {
    AST* ast = MakeAST(NODETYPE_CONSTANT_INTEGER, curr_token->filename,
                       curr_token->linenum);
    ast->data.integer = curr_token->data.integer;
    return ast;
  } else if (ProcessToken(tokens, TOKEN_STRING)) {
    AST* ast = MakeAST(NODETYPE_CONSTANT_STRING, curr_token->filename,
                       curr_token->linenum);
    ast->data.string =
        (char*)malloc(sizeof(char) * (strlen(curr_token->data.string) + 1));
    if (ast->data.string == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.string, curr_token->data.string);
    return ast;
  } else if (ProcessToken(tokens, TOKEN_CHARACTER)) {
    AST* ast = MakeAST(NODETYPE_CONSTANT_CHARACTER, curr_token->filename,
                       curr_token->linenum);
    ast->data.character = curr_token->data.character;
    return ast;
  } else if (ProcessToken(tokens, TOKEN_KW_TRUE)) {
    return MakeAST(NODETYPE_CONSTANT_TRUE, curr_token->filename,
                   curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_KW_FALSE)) {
    return MakeAST(NODETYPE_CONSTANT_FALSE, curr_token->filename,
                   curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_KW_NULL)) {
    return MakeAST(NODETYPE_CONSTANT_NULL, curr_token->filename,
                   curr_token->linenum);
  } else {
    /* Error */
    generate_error_message("a constant", curr_token->filename,
                           curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
}

/*
  Function for producing the AST nodes for any type keywords
*/
AST* TypeKeyword(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_BOOL)) {
    return MakeAST(NODETYPE_TYPE_BOOL, curr_token->filename,
                   curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_KW_CHAR)) {
    return MakeAST(NODETYPE_TYPE_CHAR, curr_token->filename,
                   curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_KW_INT)) {
    return MakeAST(NODETYPE_TYPE_INT, curr_token->filename,
                   curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_KW_STRUCT)) {
    AST* ast = MakeAST(NODETYPE_TYPE_STRUCT, curr_token->filename,
                       curr_token->linenum);
    AppendAST(ast, ID(tokens));
    return ast;
  } else {
    /* Error */
    generate_error_message("Type Keyword", curr_token->filename,
                           curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
}

/*
  Function for producing the AST nodes for IDs
*/
AST* ID(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_IDENTIFIER)) {
    AST* ast = MakeAST(NODETYPE_ID, curr_token->filename, curr_token->linenum);
    ast->data.identifier =
        (char*)malloc(sizeof(char) * (strlen(curr_token->data.identifier) + 1));
    if (ast->data.identifier == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.identifier, curr_token->data.identifier);
    return ast;
  } else {
    /* Error */
    generate_error_message("identifier", curr_token->filename,
                           curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
}

/*
  Function for producing the AST nodes for pointers
*/
AST* Star(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_SYM_TIMES)) {
    return MakeAST(NODETYPE_TYPE_POINTER, curr_token->filename,
                   curr_token->linenum);
  } else {
    /* Error */
    generate_error_message("*", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
}

/*
  Function for producing the AST nodes for any types
*/
AST* Type(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast = MakeAST(NODETYPE_TYPE, curr_token->filename, curr_token->linenum);
  AppendAST(ast, TypeKeyword(tokens));
  while (*tokens != NULL && (*tokens)->t->type == TOKEN_SYM_TIMES) {
    AppendAST(ast, Star(tokens));
  }
  return ast;
}

/*
  Function for producing the AST nodes for structs
*/
AST* StructDef(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_STRUCT) == 0) {
    /* Error */
    generate_error_message("struct keyword", curr_token->filename,
                           curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast =
      MakeAST(NODETYPE_STRUCT_DEF, curr_token->filename, curr_token->linenum);
  AppendAST(ast, ID(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_OPENBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("{", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  int is_error = 0;
  while (*tokens != NULL && ProcessToken(tokens, TOKEN_SYM_CLOSEBRACE) == 0) {
    if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
      if (is_type_keyword((*tokens)->t->type)) {
        AppendAST(ast, StructVariableDeclList(tokens));
      } else {
        curr_token = (*tokens)->t;
        generate_error_message("Struct field", curr_token->filename,
                               curr_token->linenum);
        is_error = 1;
      }
    }
    while (is_error) {
      if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON)) {
        is_error = 0;
      } else if (*tokens == NULL ||
                 (*tokens)->t->type == TOKEN_SYM_CLOSEBRACE) {
        is_error = 0;
      } else {
        *tokens = (*tokens)->next;
      }
    }
  }
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(";", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
  }
  return ast;
}

/*
  Function for producing the AST nodes for functions
*/
AST* Func(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast =
      MakeAST(NODETYPE_FUNC_DECL, curr_token->filename, curr_token->linenum);
  AppendAST(ast, Type(tokens));
  AppendAST(ast, ID(tokens));
  AppendAST(ast, ParamList(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON)) {
    return ast;
  } else if (ProcessToken(tokens, TOKEN_SYM_OPENBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("{", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  AppendAST(ast, Block(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_CLOSEBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("}", curr_token->filename, curr_token->linenum);
    }
    FreeAST(ast);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  return ast;
}

/*
  Function for producing the AST nodes for function parameters
*/
AST* ParamList(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_SYM_OPENPREN) == 0) {
    /* Error */
    generate_error_message("(", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast = MakeAST(NODETYPE_FUNC_PARAMLIST, curr_token->filename,
                     curr_token->linenum);
  while (ProcessToken(tokens, TOKEN_SYM_CLOSEPREN) == 0) {
    AppendAST(ast, Type(tokens));
    AppendAST(ast, ID(tokens));
    if (ProcessToken(tokens, TOKEN_SYM_COMMA) == 0) {
      if ((*tokens) == NULL) {
        /* Error */
        generate_eof_error();
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        return ast;
      } else if ((*tokens)->t->type != TOKEN_SYM_CLOSEPREN) {
        /* Error */
        curr_token = (*tokens)->t;
        generate_error_message(")", curr_token->filename, curr_token->linenum);
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        return ast;
      }
    }
  }
  return ast;
}

/*
  Function for producing the AST nodes for call expressions
*/
AST* CallExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast =
      MakeAST(NODETYPE_EXPR_CALL, curr_token->filename, curr_token->linenum);
  AppendAST(ast, ID(tokens));
  AppendAST(ast, ExprList(tokens));
  return ast;
}

/*
  Function for producing the AST nodes for function arugments
*/
AST* ExprList(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_SYM_OPENPREN) == 0) {
    /* Error */
  }
  AST* ast =
      MakeAST(NODETYPE_FUNC_ARGLIST, curr_token->filename, curr_token->linenum);
  while (ProcessToken(tokens, TOKEN_SYM_CLOSEPREN) == 0) {
    AppendAST(ast, Expr(tokens));
    if (ProcessToken(tokens, TOKEN_SYM_COMMA) == 0) {
      if ((*tokens) == NULL) {
        /* Error */
        generate_eof_error();
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        return ast;
      } else if ((*tokens)->t->type != TOKEN_SYM_CLOSEPREN) {
        /* Error */
        curr_token = (*tokens)->t;
        generate_error_message(")", curr_token->filename, curr_token->linenum);
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
        return ast;
      }
    }
  }
  return ast;
}

/*
  Function for producing the AST nodes for if statements
*/
AST* IfElse(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_IF) == 0) {
    /* Error */
    generate_error_message("if", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast = MakeAST(NODETYPE_CONTROL_IF_ELSE, curr_token->filename,
                     curr_token->linenum);
  if (ProcessToken(tokens, TOKEN_SYM_OPENPREN) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("(", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  AppendAST(ast, Expr(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_CLOSEPREN) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(")", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  if (ProcessToken(tokens, TOKEN_SYM_OPENBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("{", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  AppendAST(ast, Block(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_CLOSEBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("}", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  if (ProcessToken(tokens, TOKEN_KW_ELSE)) {
    if (ProcessToken(tokens, TOKEN_SYM_OPENBRACE) == 0) {
      /* Error */
      if (*tokens == NULL) {
        generate_eof_error();
      } else {
        curr_token = (*tokens)->t;
        generate_error_message("{", curr_token->filename, curr_token->linenum);
      }
      AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                             curr_token->linenum));
      return ast;
    }
    AppendAST(ast, Block(tokens));
    if (ProcessToken(tokens, TOKEN_SYM_CLOSEBRACE) == 0) {
      /* Error */
      if (*tokens == NULL) {
        generate_eof_error();
      } else {
        curr_token = (*tokens)->t;
        generate_error_message("}", curr_token->filename, curr_token->linenum);
      }
      FreeAST(ast);
      return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
    }
  }
  return ast;
}

/*
  Function for producing the AST nodes for Variable Declarations
*/
AST* VariableDeclList(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast = MakeAST(NODETYPE_VAR_DECL_LIST, curr_token->filename,
                     curr_token->linenum);
  AST* const_ast = NULL;
  AST* base_type = NULL;
  unsigned star_count = 0;
  unsigned star_capacity = INITIAL_CAPACITY;
  AST** initial_stars = (AST**)malloc(sizeof(AST*) * star_capacity);
  if (initial_stars == NULL) {
    allocation_failed();
  }
  if (*tokens != NULL) {
    curr_token = (*tokens)->t;
  }
  if (ProcessToken(tokens, TOKEN_KW_CONST)) {
    /* Begins with a const. */
    const_ast =
        MakeAST(NODETYPE_CONST, curr_token->filename, curr_token->linenum);
    base_type = TypeKeyword(tokens);
  } else {
    base_type = TypeKeyword(tokens);
    if (*tokens != NULL) {
      curr_token = (*tokens)->t;
    }
    if (ProcessToken(tokens, TOKEN_KW_CONST)) {
      const_ast =
          MakeAST(NODETYPE_CONST, curr_token->filename, curr_token->linenum);
    } else {
      /* There are possible stars before const keyword */
      while (*tokens != NULL && (*tokens)->t->type == TOKEN_SYM_TIMES) {
        if (star_count >= star_capacity) {
          star_capacity *= 2;
          initial_stars =
              (AST**)realloc(initial_stars, sizeof(AST*) * star_capacity);
          if (initial_stars == NULL) {
            allocation_failed();
          }
        }
        initial_stars[star_count++] = Star(tokens);
      }
      if (*tokens != NULL) {
        curr_token = (*tokens)->t;
      }
      /* One final check for const. */
      if (ProcessToken(tokens, TOKEN_KW_CONST)) {
        const_ast =
            MakeAST(NODETYPE_CONST, curr_token->filename, curr_token->linenum);
      }
    }
  }
  AST* var_ast;
  AST* type_ast;
  if (const_ast == NULL) {
    const_ast =
        MakeAST(NODETYPE_NOT_CONST, curr_token->filename, curr_token->linenum);
  }
  do {
    var_ast =
        MakeAST(NODETYPE_VAR_DECL, curr_token->filename, curr_token->linenum);
    AppendAST(var_ast, CopyAST(const_ast));
    type_ast =
        MakeAST(NODETYPE_TYPE, curr_token->filename, curr_token->linenum);
    AppendAST(type_ast, CopyAST(base_type));
    /* Check if this is the initial iteration and there are starter stars. */
    if (initial_stars != NULL) {
      for (unsigned i = 0; i < star_count; i++) {
        AppendAST(type_ast, initial_stars[i]);
      }
      free(initial_stars);
      initial_stars = NULL;
    }
    while (*tokens != NULL && (*tokens)->t->type == TOKEN_SYM_TIMES) {
      AppendAST(type_ast, Star(tokens));
    }
    AppendAST(var_ast, type_ast);
    AppendAST(var_ast, ID(tokens));
    if (ProcessToken(tokens, TOKEN_EQUAL)) {
      AppendAST(var_ast, Expr(tokens));
    }
    AppendAST(ast, var_ast);
    if (*tokens != NULL) {
      curr_token = (*tokens)->t;
    }
  } while (ProcessToken(tokens, TOKEN_SYM_COMMA));
  FreeAST(const_ast);
  FreeAST(base_type);
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(";", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
  }
  return ast;
}

/*
  Function for producing the AST nodes for Struct Fields
*/
AST* StructVariableDeclList(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast = MakeAST(NODETYPE_VAR_DECL_LIST, curr_token->filename,
                     curr_token->linenum);
  AST* base_type = TypeKeyword(tokens);
  if (*tokens != NULL) {
    curr_token = (*tokens)->t;
  }
  AST* var_ast;
  AST* type_ast;
  AST* const_ast =
      MakeAST(NODETYPE_NOT_CONST, curr_token->filename, curr_token->linenum);
  do {
    var_ast =
        MakeAST(NODETYPE_VAR_DECL, curr_token->filename, curr_token->linenum);
    AppendAST(var_ast, CopyAST(const_ast));
    type_ast =
        MakeAST(NODETYPE_TYPE, curr_token->filename, curr_token->linenum);
    AppendAST(type_ast, CopyAST(base_type));
    while (*tokens != NULL && (*tokens)->t->type == TOKEN_SYM_TIMES) {
      AppendAST(type_ast, Star(tokens));
    }
    AppendAST(var_ast, type_ast);
    AppendAST(var_ast, ID(tokens));
    if (ProcessToken(tokens, TOKEN_EQUAL)) {
      AppendAST(var_ast, Expr(tokens));
    }
    AppendAST(ast, var_ast);
    if (*tokens != NULL) {
      curr_token = (*tokens)->t;
    }
  } while (ProcessToken(tokens, TOKEN_SYM_COMMA));
  FreeAST(const_ast);
  FreeAST(base_type);
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(";", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
  }
  return ast;
}

/*
  Function for producing the AST nodes for continue
*/
AST* ContinueExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_CONTINUE) == 0) {
    /* Error */
    generate_error_message("Continue", curr_token->filename,
                           curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast =
      MakeAST(NODETYPE_CONTINUE, curr_token->filename, curr_token->linenum);
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(";", curr_token->filename, curr_token->linenum);
    }
    FreeAST(ast);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  return ast;
}

/*
  Function for producing the AST nodes for break
*/
AST* BreakExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_BREAK) == 0) {
    /* Error */
    generate_error_message("Break", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast = MakeAST(NODETYPE_BREAK, curr_token->filename, curr_token->linenum);
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(";", curr_token->filename, curr_token->linenum);
    }
    FreeAST(ast);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  return ast;
}

/*
  Function for producing the AST nodes for return
*/
AST* ReturnExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_RETURN) == 0) {
    /* Error */
    generate_error_message("Return", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast =
      MakeAST(NODETYPE_RETURN, curr_token->filename, curr_token->linenum);
  AppendAST(ast, Expr(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(";", curr_token->filename, curr_token->linenum);
    }
    FreeAST(ast);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  return ast;
}

/*
  Function for producing the AST nodes for for loops
*/
AST* ForLoop(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_KW_FOR) == 0) {
    /* Error */
    generate_error_message("For", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast =
      MakeAST(NODETYPE_CONTROL_FOR, curr_token->filename, curr_token->linenum);
  if (ProcessToken(tokens, TOKEN_SYM_OPENPREN) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("(", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  for (int i = 0; i < 2; i++) {
    AppendAST(ast, OptionalExpr(tokens));
    if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
      /* Error */
      if (*tokens == NULL) {
        generate_eof_error();
      } else {
        curr_token = (*tokens)->t;
        generate_error_message(";", curr_token->filename, curr_token->linenum);
        for (int j = i; j < 3; j++) {
          AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                                 curr_token->linenum));
        }
        return ast;
      }
    }
  }
  AppendAST(ast, OptionalExpr(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_CLOSEPREN) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(")", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  if (ProcessToken(tokens, TOKEN_SYM_OPENBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("{", curr_token->filename, curr_token->linenum);
    }
    AppendAST(ast,
              MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum));
    return ast;
  }
  AppendAST(ast, Block(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_CLOSEBRACE) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message("(", curr_token->filename, curr_token->linenum);
    }
    FreeAST(ast);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  return ast;
}

/*
  Function for producing the AST nodes for expressions where 1 may exist
  but is not required
*/
AST* OptionalExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error. */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  } else if ((*tokens)->t->type == TOKEN_SYM_SEMICOLON ||
             (*tokens)->t->type == TOKEN_SYM_CLOSEPREN) {
    /* Create a dummy Expr Constant as a filler */
    AST* ast = MakeAST(NODETYPE_CONSTANT_INTEGER, (*tokens)->t->filename,
                       (*tokens)->t->linenum);
    ast->data.integer = 1;
    return ast;
  } else {
    return Expr(tokens);
  }
}

/*
  Function for producing the AST nodes for blocks
*/
AST* Block(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error. */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast = MakeAST(NODETYPE_BLOCK, curr_token->filename, curr_token->linenum);
  int is_error = 0;
  while (*tokens != NULL && (*tokens)->t->type != TOKEN_SYM_CLOSEBRACE) {
    while (ProcessToken(tokens, TOKEN_SYM_SEMICOLON)) {
    }
    if (tokens == NULL) {
      generate_eof_error();
      break;
    }
    if ((*tokens)->t->type == TOKEN_KW_RETURN) {
      AppendAST(ast, ReturnExpr(tokens));
    } else if ((*tokens)->t->type == TOKEN_KW_CONTINUE) {
      AppendAST(ast, ContinueExpr(tokens));
    } else if ((*tokens)->t->type == TOKEN_KW_BREAK) {
      AppendAST(ast, BreakExpr(tokens));
    } else if ((*tokens)->t->type == TOKEN_KW_FOR) {
      AppendAST(ast, ForLoop(tokens));
    } else if ((*tokens)->t->type == TOKEN_KW_IF) {
      AppendAST(ast, IfElse(tokens));
    } else if ((*tokens)->t->type == TOKEN_KW_STRUCT) {
      AppendAST(ast, VariableDeclList(tokens));
    } else if ((*tokens)->t->type == TOKEN_KW_CONST) {
      AppendAST(ast, VariableDeclList(tokens));
    } else if (is_type_keyword((*tokens)->t->type)) {
      AppendAST(ast, VariableDeclList(tokens));
    } else if (is_expr_start((*tokens)->t->type)) {
      AppendAST(ast, Expr(tokens));
      if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON) == 0) {
        if (*tokens == NULL) {
          generate_eof_error();
        } else {
          curr_token = (*tokens)->t;
          generate_error_message("{", curr_token->filename,
                                 curr_token->linenum);
        }
        AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                               curr_token->linenum));
      }
    } else if ((*tokens)->t->type != TOKEN_SYM_CLOSEBRACE) {
      /* Error */
      curr_token = (*tokens)->t;
      generate_error_message("} or Block Content", curr_token->filename,
                             curr_token->linenum);
      AppendAST(ast, MakeAST(NODETYPE_ERR, curr_token->filename,
                             curr_token->linenum));
      is_error = 1;
    }
    while (is_error) {
      if (ProcessToken(tokens, TOKEN_SYM_SEMICOLON)) {
        is_error = 0;
      } else if (*tokens == NULL ||
                 (*tokens)->t->type == TOKEN_SYM_CLOSEBRACE) {
        is_error = 0;
      } else {
        *tokens = (*tokens)->next;
      }
    }
  }
  return ast;
}

/*
  Function for producing the AST nodes for ||
*/
AST* BinaryExprLogicalOr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_LOGICAL_OR;
  enum TokenType token_choices[1] = {TOKEN_SYM_OROR};
  enum NodeType ast_choices[1] = {NODETYPE_EXPR_BINARY_OROR};
  return BinaryExpr(tokens, token_choices, ast_choices, 1, expr_type);
}

/*
  Function for producing the AST nodes for &&
*/
AST* BinaryExprLogicalAnd(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_LOGICAL_AND;
  enum TokenType token_choices[1] = {TOKEN_SYM_ANDAND};
  enum NodeType ast_choices[1] = {NODETYPE_EXPR_BINARY_ANDAND};
  return BinaryExpr(tokens, token_choices, ast_choices, 1, expr_type);
}

/*
  Function for producing the AST nodes for |
*/
AST* BinaryExprBitwiseOr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_BITWISE_OR;
  enum TokenType token_choices[1] = {TOKEN_SYM_OR};
  enum NodeType ast_choices[1] = {NODETYPE_EXPR_BINARY_OR};
  return BinaryExpr(tokens, token_choices, ast_choices, 1, expr_type);
}

/*
  Function for producing the AST nodes for ^
*/
AST* BinaryExprBitwiseXor(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_BITWISE_XOR;
  enum TokenType token_choices[1] = {TOKEN_SYM_XOR};
  enum NodeType ast_choices[1] = {NODETYPE_EXPR_BINARY_XOR};
  return BinaryExpr(tokens, token_choices, ast_choices, 1, expr_type);
}

/*
  Function for producing the AST nodes for &
*/
AST* BinaryExprBitwiseAnd(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_BITWISE_AND;
  enum TokenType token_choices[1] = {TOKEN_SYM_AND};
  enum NodeType ast_choices[1] = {NODETYPE_EXPR_BINARY_AND};
  return BinaryExpr(tokens, token_choices, ast_choices, 1, expr_type);
}

/*
  Function for producing the AST nodes for == and !=
*/
AST* BinaryExprEquality(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_EQUALITY;
  enum TokenType token_choices[2] = {TOKEN_EQUALEQUAL, TOKEN_BANGEQUAL};
  enum NodeType ast_choices[2] = {NODETYPE_EXPR_BINARY_EQUAL,
                                  NODETYPE_EXPR_BINARY_NOTEQUAL};
  return BinaryExpr(tokens, token_choices, ast_choices, 2, expr_type);
}

/*
  Function for producing the AST nodes for >, >=, <, <=
*/
AST* BinaryExprComparison(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }

  enum BinaryExprType expr_type = BINARY_EXPR_COMPARISON;
  enum TokenType token_choices[4] = {TOKEN_GREATER, TOKEN_GREATEREQUAL,
                                     TOKEN_LESS, TOKEN_LESSEQUAL};
  enum NodeType ast_choices[4] = {
      NODETYPE_EXPR_BINARY_GREATER, NODETYPE_EXPR_BINARY_GREATEREQUAL,
      NODETYPE_EXPR_BINARY_LESS, NODETYPE_EXPR_BINARY_LESSEQUAL};
  return BinaryExpr(tokens, token_choices, ast_choices, 4, expr_type);
}

/*
  Function for producing the AST nodes for + and -
*/
AST* BinaryExprAddSub(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_ADDSUB;
  enum TokenType token_choices[2] = {TOKEN_SYM_PLUS, TOKEN_SYM_MINUS};
  enum NodeType ast_choices[2] = {NODETYPE_EXPR_BINARY_ADD,
                                  NODETYPE_EXPR_BINARY_SUB};
  return BinaryExpr(tokens, token_choices, ast_choices, 2, expr_type);
}

/*
  Function for producing the AST nodes for * and /
*/
AST* BinaryExprMultDiv(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  enum BinaryExprType expr_type = BINARY_EXPR_MULTDIV;
  enum TokenType token_choices[2] = {TOKEN_SYM_TIMES, TOKEN_SYM_SLASH};
  enum NodeType ast_choices[2] = {NODETYPE_EXPR_BINARY_MULT,
                                  NODETYPE_EXPR_BINARY_DIV};
  return BinaryExpr(tokens, token_choices, ast_choices, 2, expr_type);
}

/*
  Function for producing the AST nodes for postfix expressions
*/
AST* PostfixExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  AST* ast = BaseExpr(tokens);
  Token* curr_token = NULL;
  if (*tokens != NULL) {
    curr_token = (*tokens)->t;
  }
  while (is_postfix_expr(*tokens)) {
    AST* new_ast = NULL;
    int is_struct = 0;
    if (ProcessToken(tokens, TOKEN_SYM_ARROW)) {
      new_ast = MakeAST(NODETYPE_EXPR_STRUCT_ARROW, curr_token->filename,
                        curr_token->linenum);
      is_struct = 1;
    } else if (ProcessToken(tokens, TOKEN_SYM_DOT)) {
      new_ast = MakeAST(NODETYPE_EXPR_STRUCT_DOT, curr_token->filename,
                        curr_token->linenum);
      is_struct = 1;
    } else if (ProcessToken(tokens, TOKEN_SYM_PLUSPLUS)) {
      new_ast = MakeAST(NODETYPE_EXPR_POSTFIX_PLUSPLUS, curr_token->filename,
                        curr_token->linenum);
    } else if (ProcessToken(tokens, TOKEN_SYM_MINUSMINUS)) {
      new_ast = MakeAST(NODETYPE_EXPR_POSTFIX_MINUSMINUS, curr_token->filename,
                        curr_token->linenum);
    }
    AppendAST(new_ast, ast);
    ast = new_ast;
    if (is_struct) {
      AppendAST(ast, ID(tokens));
    }
  }
  return ast;
}

/*
  Function for producing the AST nodes for prefix expressions
*/
AST* PrefixExpr(TokenList** tokens) {
  AST* ast = NULL;
  Token* curr_token = NULL;
  if (*tokens != NULL) {
    curr_token = (*tokens)->t;
  }
  if (ProcessToken(tokens, TOKEN_SYM_PLUSPLUS)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_PLUSPLUS, curr_token->filename,
                  curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_SYM_MINUSMINUS)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_MINUSMINUS, curr_token->filename,
                  curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_SYM_MINUS)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_NEGATE, curr_token->filename,
                  curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_SYM_AND)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_ADDRESS, curr_token->filename,
                  curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_SYM_TIMES)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_DEREFERENCE, curr_token->filename,
                  curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_SYM_NOTNOT)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_NOTNOT, curr_token->filename,
                  curr_token->linenum);
  } else if (ProcessToken(tokens, TOKEN_SYM_NOT)) {
    ast = MakeAST(NODETYPE_EXPR_PREFIX_NOT, curr_token->filename,
                  curr_token->linenum);
  } else if ((*tokens)->t->type == TOKEN_SYM_OPENPREN) {
    TokenList* nextToken = (*tokens)->next;
    if (nextToken == NULL) {
      /* Error */
      generate_eof_error();
      return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
    } else if (is_type_keyword(nextToken->t->type)) {
      return CastExpr(tokens);
    } else {
      return PostfixExpr(tokens);
    }
  } else {
    return PostfixExpr(tokens);
  }
  AppendAST(ast, PrefixExpr(tokens));
  return ast;
}

/*
  Function for producing the AST nodes for IDs, pren expressions, and constants
*/
AST* BaseExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_SYM_OPENPREN)) {
    AST* ast =
        MakeAST(NODETYPE_EXPR_PREN, curr_token->filename, curr_token->linenum);
    AppendAST(ast, Expr(tokens));
    if (ProcessToken(tokens, TOKEN_SYM_CLOSEPREN) == 0) {
      /* Error */
      if (*tokens == NULL) {
        generate_eof_error();
      } else {
        curr_token = (*tokens)->t;
        generate_error_message(")", curr_token->filename, curr_token->linenum);
      }
      FreeAST(ast);
      return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
    }
    return ast;
  } else if (is_constant((*tokens)->t->type)) {
    return Constant(tokens);
  } else if ((*tokens)->t->type == TOKEN_IDENTIFIER) {
    TokenList* nextToken = (*tokens)->next;
    if (nextToken == NULL) {
      /* Error */
      generate_eof_error();
      return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
    } else if (nextToken->t->type == TOKEN_SYM_OPENPREN) {
      return CallExpr(tokens);
    } else {
      return ID(tokens);
    }
  } else {
    /* Error */
    Token* curr_token = (*tokens)->t;
    generate_error_message("Expression", curr_token->filename,
                           curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
}

/*
  Function for producing the AST nodes for cast expressions
*/
AST* CastExpr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  if (ProcessToken(tokens, TOKEN_SYM_OPENPREN) == 0) {
    /* Error */
    generate_error_message("(", curr_token->filename, curr_token->linenum);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AST* ast =
      MakeAST(NODETYPE_EXPR_CAST, curr_token->filename, curr_token->linenum);
  AppendAST(ast, Type(tokens));
  if (ProcessToken(tokens, TOKEN_SYM_CLOSEPREN) == 0) {
    /* Error */
    if (*tokens == NULL) {
      generate_eof_error();
    } else {
      curr_token = (*tokens)->t;
      generate_error_message(")", curr_token->filename, curr_token->linenum);
    }
    FreeAST(ast);
    return MakeAST(NODETYPE_ERR, curr_token->filename, curr_token->linenum);
  }
  AppendAST(ast, PrefixExpr(tokens));
  return ast;
}

/*
  Function for producing the AST nodes for expressions
*/
AST* Expr(TokenList** tokens) {
  if (*tokens == NULL) {
    /* Error */
    generate_eof_error();
    return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
  Token* curr_token = (*tokens)->t;
  AST* ast = BinaryExprLogicalOr(tokens);
  if (ProcessToken(tokens, TOKEN_EQUAL)) {
    AST* first_ast = MakeAST(NODETYPE_EXPR_ASSIGN, curr_token->filename,
                             curr_token->linenum);
    AppendAST(first_ast, ast);
    ast = first_ast;
    AppendAST(ast, Expr(tokens));
  }
  return ast;
}

/*
  Function to handle AST Node production for Binary Expressions.
  Takes in a TokenList double pointer, an array of TokenTypes
  and an array of NodeTypes each with the given size and a BinaryExprType
  which states what type of expression is being built. This function
  returns the AST Nodes that result from properly building the BinaryExprType
  following proper precedence.
*/
AST* BinaryExpr(TokenList** tokens,
                enum TokenType* possible_tokens,
                enum NodeType* ast_choices,
                size_t size,
                enum BinaryExprType type) {
  /* FIX ME */
  int loop = 1;
  AST* first_ast = GetNextBinaryExpr(tokens, type);
  while (loop) {
    loop = 0;
    for (int i = 0; i < size; i++) {
      if (ProcessToken(tokens, possible_tokens[i])) {
        AST *ast = MakeAST(ast_choices[i], (*tokens)->t->filename,
                           (*tokens)->t->linenum);
        loop = 1;
        AppendAST(ast, first_ast);
        first_ast = ast;
        AppendAST(ast, GetNextBinaryExpr(tokens, type));
      }
    }
  }
  return first_ast;
}

/*
  Function which produces the proper AST child based upon the type of
  Binary Expression passed in.
*/
AST* GetNextBinaryExpr(TokenList** tokens, enum BinaryExprType type) {
  switch(type) {
    case BINARY_EXPR_LOGICAL_OR:
      return BinaryExprLogicalAnd(tokens);
    case BINARY_EXPR_LOGICAL_AND:
      return BinaryExprBitwiseOr(tokens);
    case BINARY_EXPR_BITWISE_OR:
      return BinaryExprBitwiseXor(tokens);
    case BINARY_EXPR_BITWISE_XOR:
      return BinaryExprBitwiseAnd(tokens);
    case BINARY_EXPR_BITWISE_AND:
      return BinaryExprEquality(tokens);
    case BINARY_EXPR_EQUALITY:
      return BinaryExprComparison(tokens);
    case BINARY_EXPR_COMPARISON:
      return BinaryExprAddSub(tokens);
    case BINARY_EXPR_ADDSUB:
      return BinaryExprMultDiv(tokens);
    case BINARY_EXPR_MULTDIV:
      return PrefixExpr(tokens);
    default:
      generate_eof_error();
      return MakeAST(NODETYPE_ERR, curr_filename, -1);
  }
}
