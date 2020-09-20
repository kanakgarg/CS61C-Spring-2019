#ifndef CGEN_HELPERS_H
#define CGEN_HELPERS_H

#include "dast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct string_label_map {
  char* string;
  char* label;
  struct string_label_map* next;
} string_label_map;

extern string_label_map* s;

void freeStringList(string_label_map* lst);

/* Create new local label for use anywhere */
char* generateLocalLabel();

/* 
  Used to create a string used to refer to a label at the end of functions
  for return statements.
*/
char *generateFunctionEndLabel (char *func_name);

/* Create new string label for given string and add to string_label_map s */
char* generateStringLabel(char* str);

void generateString(Decl* curr);

void generateDataValue (int64_t value, size_t data_size);

void generateDataLabel (char *label, size_t data_size);

char* getStringLabel(string_label_map* temp, char* str);

  # endif
