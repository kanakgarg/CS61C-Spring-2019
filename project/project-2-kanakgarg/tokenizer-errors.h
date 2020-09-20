#ifndef TOKENIZER_ERRORS_H
#define TOKENIZER_ERRORS_H

#include "tokens.h"

/*
  Takes a pointer to an already allocated token t, a buffer, a starting location, a maximum buffer size,
  the line number of the buffer and the file of the buffer and produces an error token for a general error.
  Returns the number of characters consumed for the error, which is 0 if the error token could not be
  produced because a delimiter was not found in the given buffer size.
  For a general error characters are consumed until any whitespace is seen.
*/
int generate_generic_error (Token **t, char *buffer, size_t start, size_t size, int line_num, char *filename);

/*
  Takes a pointer to an already allocated token t, a buffer, a starting location, a maximum buffer size,
  the line number of the buffer and the file of the buffer and produces an error token for a general error.
  Returns the number of characters consumed for the error, which is 0 if the error token could not be
  produced because a delimiter was not found in the given buffer size.
  For a character error, characters are consumed until a ' or \n is seen.
*/
int generate_character_error (Token **t, char *buffer, size_t start, size_t size, int line_num, char *filename);

/*
  Takes a pointer to an already allocated token t, a buffer, a starting location, a maximum buffer size,
  the line number of the buffer and the file of the buffer and produces an error token for a general error.
  Returns the number of characters consumed for the error, which is 0 if the error token could not be
  produced because a delimiter was not found in the given buffer size.
  For a string error, characters are consumed until a " or \n is seen.
*/
int generate_string_error (Token **t, char *buffer, size_t start, size_t size, int line_num, char *filename);

#endif
