#include "tokenizer-errors.h"
#include <stdlib.h>
#include <string.h>
#include "string-helpers.h"
#include "utils.h"

/*
  Takes a pointer to a token t, a buffer, a starting
  location, a maximum buffer size,
  the line number of the buffer and the file of the buffer and produces an error
  token for a general error.
  Returns the number of characters consumed for the error, which is 0 if the
  error token could not be
  produced because a delimiter was not found in the given buffer size.
  For a general error characters are consumed until any whitespace is seen.
*/
int generate_generic_error(Token** t,
                           char* buffer,
                           size_t start,
                           size_t size,
                           int linenum,
                           char* filename) {
  char token_contents[size - start + 1];
  int err_len = 1;
  int search = 1;
  while (start + err_len < size && search) {
    if (!is_space(buffer[start + err_len])) {
      err_len++;
    } else {
      search = 0;
      /* Create an error token. */
      for (int j = 0; j < err_len; j++) {
        token_contents[j] = buffer[start + j];
      }
      token_contents[err_len] = '\0';
      *t = create_token(filename);
      (*t)->linenum = linenum;
      (*t)->data.error = (char*)malloc(sizeof(char) * strlen(token_contents) + 1);
      if ((*t)->data.error == NULL) {
        allocation_failed();
      }
      strcpy((*t)->data.error, token_contents);
      (*t)->type = TOKEN_ERR;
    }
  }
  if (!search) {
    return err_len;
  } else {
    return 0;
  }
}

/*
  Takes a pointer to a token t, a buffer, a starting
  location, a maximum buffer size,
  the line number of the buffer and the file of the buffer and produces an error
  token for a general error.
  Returns the number of characters consumed for the error, which is 0 if the
  error token could not be
  produced because a delimiter was not found in the given buffer size.
  For a character error, characters are consumed until a ' or \n is seen.
*/
int generate_character_error(Token** t,
                             char* buffer,
                             size_t start,
                             size_t size,
                             int linenum,
                             char* filename) {
  char token_contents[size - start + 1];
  int err_len = 1;
  int search = 1;
  while (start + err_len < size && search) {
    if (start + err_len + 1 < size &&
        replace_escape_in_character(buffer + start + err_len) != -1) {
      err_len += 2;
    } else if (buffer[start + err_len] != '\n' &&
               buffer[start + err_len] != '\'') {
      err_len++;
    } else {
      search = 0;
      if (buffer[start + err_len] != '\n') {
        err_len++;
      }
      /* Create an error token. */
      for (int j = 0; j < err_len; j++) {
        token_contents[j] = buffer[start + j];
      }
      token_contents[err_len] = '\0';
      *t = create_token(filename);
      (*t)->linenum = linenum;
      (*t)->data.error =
          (char*)malloc(sizeof(char) * strlen(token_contents) + 1);
      if ((*t)->data.error == NULL) {
        allocation_failed();
      }
      strcpy((*t)->data.error, token_contents);
      (*t)->type = TOKEN_ERR;
    }
  }
  if (!search) {
    return err_len;
  } else {
    return 0;
  }
}

/*
  Takes a pointer to a token t, a buffer, a starting
  location, a maximum buffer size,
  the line number of the buffer and the file of the buffer and produces an error
  token for a general error.
  Returns the number of characters consumed for the error, which is 0 if the
  error token could not be
  produced because a delimiter was not found in the given buffer size.
  For a string error, characters are consumed until a " or \n is seen.
*/
int generate_string_error(Token** t,
                          char* buffer,
                          size_t start,
                          size_t size,
                          int linenum,
                          char* filename) {
  char token_contents[size - start + 1];
  int err_len = 1;
  int search = 1;
  while (start + err_len < size && search) {
    if (start + err_len + 1 < size &&
        replace_escape_in_string(buffer + start + err_len) != -1) {
      err_len += 2;
    } else if (buffer[start + err_len] != '\n' &&
               buffer[start + err_len] != '"') {
      err_len++;
    } else {
      search = 0;
      if (buffer[start + err_len] != '\n') {
        err_len++;
      }
      /* create an error token. */
      for (int j = 0; j < err_len; j++) {
        token_contents[j] = buffer[start + j];
      }
      token_contents[err_len] = '\0';
      *t = create_token(filename);
      (*t)->linenum = linenum;
      (*t)->data.error =
          (char*)malloc(sizeof(char) * strlen(token_contents) + 1);
      if ((*t)->data.error == NULL) {
        allocation_failed();
      }
      strcpy((*t)->data.error, token_contents);
      (*t)->type = TOKEN_ERR;
    }
  }
  if (!search) {
    return err_len;
  } else {
    return 0;
  }
}

