#include "tokenizer.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "string-helpers.h"
#include "tokenizer-errors.h"
#include "tokens.h"
#include "utils.h"

// Helper Functions to produce tokens
enum TokenType check_keyword(char* str);
size_t SelectToken(char* buffer,
                   size_t size,
                   size_t* linenum,
                   TokenList** tokens,
                   char* filename);

/*
  Global variable to check if we are in a comment and if so to throw out all
  characters until a newline is reached.

  Hmmm... This might be helpful!
*/
int IS_COMMENT = 0;

/*
  Helper function to determine if a token is a keyword. It takes in a
  null terminated string and checks it against all of the keywords.
  If it makes a keyword it returns that token type and otherwise
  returns the error token type.
*/
enum TokenType check_keyword(char* str) {
  char lower_str[strlen(str) + 1];
  lower_strcpy(lower_str, str);
  if (strcmp(lower_str, "int") == 0) {
    return TOKEN_KW_INT;
  } else if (strcmp(lower_str, "char") == 0) {
    return TOKEN_KW_CHAR;
  } else if (strcmp(lower_str, "const") == 0) {
    return TOKEN_KW_CONST;
  } else if (strcmp(lower_str, "struct") == 0) {
    return TOKEN_KW_STRUCT;
  } else if (strcmp(lower_str, "bool") == 0) {
    return TOKEN_KW_BOOL;
  } else if (strcmp(lower_str, "for") == 0) {
    return TOKEN_KW_FOR;
  } else if (strcmp(lower_str, "continue") == 0) {
    return TOKEN_KW_CONTINUE;
  } else if (strcmp(lower_str, "break") == 0) {
    return TOKEN_KW_BREAK;
  } else if (strcmp(lower_str, "if") == 0) {
    return TOKEN_KW_IF;
  } else if (strcmp(lower_str, "else") == 0) {
    return TOKEN_KW_ELSE;
  } else if (strcmp(lower_str, "true") == 0) {
    return TOKEN_KW_TRUE;
  } else if (strcmp(lower_str, "false") == 0) {
    return TOKEN_KW_FALSE;
  } else if (strcmp(lower_str, "return") == 0) {
    return TOKEN_KW_RETURN;
  } else if (strcmp(lower_str, "null") == 0) {
    return TOKEN_KW_NULL;
  } else {
    return TOKEN_ERR;
  }
}

/*
  Takes in a buffer with a given size, a pointer to a linenumber which should be
  incremented every time a newline is encountered, a pointer to a list of tokens
  and a filename. Removes any unnecessary whitespace. Then it searches based
  upon
  first character to produce a token if it can find one to append to the tokens
  list,
  possibly an error if the input does not match any valid tokens. Returns the
  number
  of characters that were consumed to produce tokens (and thus will not be
  reused).
*/
size_t SelectToken(char* buffer,
                   size_t size,
                   size_t* linenum,
                   TokenList** tokens,
                   char* filename) {
  char token_contents[size + 1];
  size_t size_read = 0;

  /* First remove any leading whitespace. */
  while (size_read < size && (is_space(buffer[size_read]))) {
    if (buffer[size_read] == '\n') {
      (*linenum)++;
    }
    size_read++;
  }

  while (IS_COMMENT) {
      if (size_read >= size) {
          return size;
      }
      if (buffer[size_read] == '\n') {
          IS_COMMENT = 0;
          return size_read;
      }
      size_read++;
  }

  /* Check and make sure you haven't only read in whitespace. */
  if (size_read == size) {
    return size_read;
  }

  /*
     Token that will be appended to the list of tokens. Assumes that if
     a token cannot be produced it will return.
  */
  TokenList* node = NULL;
  Token* t = NULL;

  /* Should now be able to narrow tokens by first character. */
  if (buffer[size_read] == '+') {  // + and ++
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '+') {
      t->type = TOKEN_SYM_PLUSPLUS;
      size_read++;
    } else {
      t->type = TOKEN_SYM_PLUS;
    }
  } else if (buffer[size_read] == '-') {  // -, --, ->
    if (size_read + 1 == size) {
      return size_read;
    }
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read + 1] == '>') {
      t->type = TOKEN_SYM_ARROW;
      size_read += 2;
    } else if (buffer[size_read + 1] == '-') {
      t->type = TOKEN_SYM_MINUSMINUS;
      size_read += 2;
    } else {
      size_read++;
      t->type = TOKEN_SYM_MINUS;
    }
  } else if (buffer[size_read] == '*') {  // *
    t = create_token(filename);
    t->type = TOKEN_SYM_TIMES;
    t->linenum = *linenum;
    size_read++;
  } else if (buffer[size_read] == '/') {  // / and comments
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    if (buffer[size_read] == '/') {
      IS_COMMENT = 1;
      size_read++;
      while (IS_COMMENT) {
          if (size_read >= size) {
              return size;
          }
          if (buffer[size_read] == '\n') {
              IS_COMMENT = 0;
              return size_read;
          }
          size_read++;
      }
    }
     else {
      t = create_token(filename);
      t->type = TOKEN_SYM_SLASH;
      t->linenum = *linenum;
    }
  } else if (buffer[size_read] == '=') {  // = and ==
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '=') {
      t->type = TOKEN_EQUALEQUAL;
      size_read++;
    } else {
      t->type = TOKEN_EQUAL;
    }
  } else if (buffer[size_read] == '!') {  // ! and !=
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '=') {
      size_read++;
      t->type = TOKEN_BANGEQUAL;
    } else {
      t->type = TOKEN_SYM_NOTNOT;
    }
  } else if (buffer[size_read] == '>') {  // > and >=
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '=') {
      t->type = TOKEN_GREATEREQUAL;
      size_read++;
    } else {
      t->type = TOKEN_GREATER;
    }
  } else if (buffer[size_read] == '<') {  // < and <=
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '=') {
      t->type = TOKEN_LESSEQUAL;
      size_read++;
    } else {
      t->type = TOKEN_LESS;
    }
  } else if (buffer[size_read] == '.') {  // .
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_DOT;
  } else if (buffer[size_read] == '&') {  // & and &&
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '&') {
      t->type = TOKEN_SYM_ANDAND;
      size_read++;
    } else {
      t->type = TOKEN_SYM_AND;
    }
  } else if (buffer[size_read] == '|') {  // | and ||
    if (size_read + 1 == size) {
      return size_read;
    }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    if (buffer[size_read] == '|') {
      t->type = TOKEN_SYM_OROR;
      size_read++;
    } else {
      t->type = TOKEN_SYM_OR;
    }
  } else if (buffer[size_read] == '^') {  // ^
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_XOR;
  } else if (buffer[size_read] == '~') {  // ~
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_NOT;
  } else if (buffer[size_read] == '(') {  // (
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_OPENPREN;
  } else if (buffer[size_read] == ')') {  // )
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_CLOSEPREN;
  } else if (buffer[size_read] == '{') {  // {
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_OPENBRACE;
  } else if (buffer[size_read] == '}') {  // }
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_CLOSEBRACE;
  } else if (buffer[size_read] == ',') {  // ,
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_COMMA;
  } else if (buffer[size_read] == ';') {  // ;
    size_read++;
    t = create_token(filename);
    t->linenum = *linenum;
    t->type = TOKEN_SYM_SEMICOLON;
  } else if (buffer[size_read] == '#') {  // #include /s* "string" or error
    int match = 1;
    int i = 0;
    for (i = 0; i < strlen (PREPROCESSOR_PRAGMA) && size_read + i < size; i++) {
      if (buffer[size_read + i] != PREPROCESSOR_PRAGMA[i]) {
        match = 0;
	      break;
      }
    }
    size_t str_len = 0;
    if (match) {
      str_len = strlen(PREPROCESSOR_PRAGMA);
      int search = 1;
      while (size_read + str_len < size && buffer[size_read + str_len] == ' ') {
        size_read++;
      }
      if (size_read + str_len >= size) {
        return size_read;
      } else if (buffer[size_read + str_len] == '"') {
        str_len++;
        while (size_read + str_len < size && search) {
          if (buffer[size_read + str_len] == '\\') {
            if (size_read + str_len + 1 < size &&
                replace_escape_in_string(buffer + size_read + str_len) != -1) {
              str_len += 2;
            } else {
              str_len += 1;
            }
          } else if (buffer[size_read + str_len] == '"') {
            search = 0;
            size_read += str_len + 1;
            return size_read;
          } else if (!isprint(buffer[size_read + str_len])) {
            search = 0;
            int total = generate_string_error(&t, buffer, size_read, size,
                                              *linenum, filename);
            if (total == 0) {
              return size_read;
            } else {
              size_read += total;
            }
          } else {
            str_len += 1;
          }
        }
      } else {
        int total = generate_string_error(&t, buffer, size_read, size, *linenum,
                                          filename);
        if (total == 0) {
          return size_read;
        } else {
          size_read += total;
        }
      }
    } else {
      int total = generate_generic_error(&t, buffer, size_read, size, *linenum,
                                         filename);
      if (total == 0) {
        return size_read;
      } else {
        size_read += total;
      }
    }
    if (size_read + str_len >= size) {
      return size_read;
    }
      } else if (buffer[size_read] == '\'') {// characters and some errors
          if (size_read + 1 == size) {
              return size_read;
          }
          size_read++;
          if (buffer[size_read] == '\\' && buffer[size_read + 2] == '\'') {
            if (replace_escape_in_character(buffer + size_read) != -1){
                t = create_token(filename);
                t->linenum = *linenum;
                t->data.character = replace_escape_in_character(buffer + size_read);
                t->type = TOKEN_CHARACTER;
                size_read += 3;
            }
          } else if(isprint(buffer[size_read]) && buffer[size_read + 1] == '\'') {
              //alphanumericspecial char
              t = create_token(filename);
              t->linenum = *linenum;
              t->data.character = buffer[size_read];
              t->type = TOKEN_CHARACTER;
              size_read += 2;
          } else {
              //Error
              int total = generate_character_error(&t, buffer, size_read - 1, size, *linenum, filename);
              if (total == 0) {
                  return size_read;
              } else {
                  size_read += total;
              }
          }

  } else if (buffer[size_read] == '"') {  // strings and some errors
    size_t str_len = 1;
    int search = 1;
    while (size_read + str_len < size && search) {
      if (buffer[size_read + str_len] == '\\') {
        if (size_read + str_len + 1 < size &&
            replace_escape_in_string(buffer + size_read + str_len) != -1) {
          str_len += 2;
        } else {
          str_len += 1;
        }
      } else if (buffer[size_read + str_len] == '"') {
        search = 0;
        /* Create a string token. */
        for (int j = 0; j < str_len - 1; j++) {
          token_contents[j] = buffer[size_read + j + 1];
        }
        token_contents[str_len - 1] = '\0';
        size_read += str_len + 1;
        t = create_token(filename);
        t->linenum = *linenum;
        t->data.string =
            (char*)malloc(sizeof(char) * strlen(token_contents) + 1);
        size_t i = 0;
        size_t j = 0;
        while (i < strlen(token_contents)) {
          int escaped = replace_escape_in_string(token_contents + i);
          if (escaped == -1) {
            t->data.string[j++] = token_contents[i++];
          } else {
            t->data.string[j++] = escaped;
            i += 2;
          }
          t->data.string[j] = '\0';
        }
        t->type = TOKEN_STRING;
      } else if (!isprint(buffer[size_read + str_len])) {
        search = 0;
        int total = generate_string_error(&t, buffer, size_read, size, *linenum,
                                          filename);
        if (total == 0) {
          return size_read;
        } else {
          size_read += total;
        }
      } else {
        str_len += 1;
      }
    }
    if (search) {
      return size_read;
    }
  } else if (is_digit(
                 buffer[size_read])) {  // positive integers and some errors
    size_t int_len = 1;
    int search = 1;
    while (size_read + int_len < size && search) {
      if (is_digit(buffer[size_read + int_len])) {
        int_len++;
      } else {
        search = 0;
        /* Create an int token. Hint: you may find the function strtol helpful
         */
        for (int j = 0; j < int_len; j++) {

            token_contents[j] = buffer[size_read + j];
        }
        token_contents[int_len] = '\0';

        int num = strtol(token_contents, NULL, 10);
	    t = create_token(filename);
	    t->linenum = *linenum;
	    t->type = TOKEN_INTEGER;
	    t->data.integer = num;

	    if(buffer[size_read] == '0' && int_len > 1) {
            int total = generate_generic_error(&t, buffer, size_read, size,
                                               *linenum, filename);
            if (total == 0) {
                return size_read;
            } else {
                size_read += total;
            }
        } else {
            size_read += int_len;
        }
        }
      }

    if (search) {
      return size_read;
    }
  } else {  // Identifiers, keywords, and errors
    size_t id_len = 1;
    int search = 1;
    while (size_read + id_len < size && search) {
      if (is_identifier_component(buffer[size_read + id_len])) {
        id_len++;
      } else {
        search = 0;
        /* Create a token. */
        for (int j = 0; j < id_len; j++) {
          token_contents[j] = buffer[size_read + j];
        }
        token_contents[id_len] = '\0';
        enum TokenType type = check_keyword(token_contents);
        if (type != TOKEN_ERR) {
          t = create_token(filename);
          t->linenum = *linenum;
          t->type = type;
          size_read += id_len;
        } else if (is_valid_identifier(token_contents)) {  // FIX ME
          t = create_token(filename);
          t->linenum = *linenum;
          t->type = TOKEN_IDENTIFIER;
          t -> data.identifier = (char *)malloc(sizeof(char) * strlen(token_contents) + 1);
          for (int i = 0; i < id_len; i++) {
            t->data.identifier[i] = token_contents[i];
          }
          size_read += id_len;
        } else {
          /* Errors */
          int total = generate_generic_error(&t, buffer, size_read, size,
                                             *linenum, filename);
          if (total == 0) {
            return size_read;
          } else {
            size_read += total;
          }
        }
      }
    }
    if (search) {
      return size_read;
    }
  }

  /* Append the Token to the end of the list. */
  node = create_list(t);
  if (*tokens != NULL) {
    (*tokens)->next = node;
  }
  *tokens = node;
  return size_read;
}

/*
  Takes in a filename and parses the file contents into a series of tokens.
  Returns the list of tokens in the file or NULL if there are no tokens
  in the file. Files can have unbounded total input size as well as unbounded
  line size (see the spec for grading breakdown if you do not handle unbounded
  input sizes). If a file cannot be opened or memory cannot be allocated the
  program should exit with a nonzero exit code. It is NEVER acceptable for the
  program to SEGFAULT.
*/
TokenList* TokenFile(char* filename) {
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: Unable to open file %s\n", filename);
    exit(1);
  }
  char* token_buffer = (char*)malloc(DEFAULT_BUFFERSIZE);
  if (token_buffer == NULL) {
    allocation_failed();
  }
  size_t buffer_size = 0;
  size_t buffer_offset = 0;
  size_t mem_size = DEFAULT_BUFFERSIZE;
  size_t linenum = 1;
  size_t read_size = 0;
  size_t tokens_read = 0;
  TokenList* tokens = NULL;
  TokenList* tokens_end = tokens;
  /*
     Read in input. We assume an unbounded line size so we cannot read in an
     entire line at a time.
  */
  while ((read_size = fread(token_buffer + buffer_size, sizeof(char),
                            mem_size - buffer_size, f)) != 0) {
    buffer_size += read_size;
    while ((tokens_read = SelectToken(token_buffer + buffer_offset,
                                      buffer_size - buffer_offset, &linenum,
                                      &tokens_end, filename)) != 0) {
      if (tokens == NULL) {
        tokens = tokens_end;
      }
      buffer_offset += tokens_read;
    }

    /*
       Move any remaining elements in the buffer to the front. Note that we
       cannot use strcpy because copying inside the same buffer leads to
       undefined behavior.
    */

    /* Set remaining size to to be that in the offset. */
    buffer_size = buffer_size - buffer_offset;

    if (buffer_offset != 0) {
      for (size_t i = 0; i < buffer_size; i++) {
        token_buffer[i] = token_buffer[i + buffer_offset];
      }
    }

    /*
       Make any necessary increases to the memory capacity of the buffer.
       This is only necessary when buffer_offset == 0, but we will do it
       earlier for efficiency.
    */
    if (buffer_offset < DEFAULT_BUFFERSIZE / 4) {
      mem_size *= 2;
      token_buffer = (char*)realloc(token_buffer, mem_size);
      if (token_buffer == NULL) {
        allocation_failed();
      }
    }

    /* Reset offset. */
    buffer_offset = 0;
  }

  /* There should never be any remaining tokens assuming every file ends in a
   * newline. */

  free(token_buffer);

  fclose(f);
  return tokens;
}

