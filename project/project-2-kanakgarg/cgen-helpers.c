#include "cgen-helpers.h"
#include "dast.h"
#include "utils.h"
#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int LABEL_COUNT = 0;
int STR_COUNT = 0;
int VAR_COUNT = 0;

void freeStringList(string_label_map* lst) {
  if (lst == NULL) {
    return;
  }
  free(lst->label);
  free(lst->string);
  freeStringList(lst->next);
  free(lst);
}

/* Create new local label for use anywhere */
char* generateLocalLabel() {
  char* label = (char*)malloc(sizeof("__label__") + 5);
  sprintf(label, "__label__%d", LABEL_COUNT);
  LABEL_COUNT++;
  return label;
}

/* 
  Used to create a string used to refer to a label at the end of functions
  for return statements.
*/
char *generateFunctionEndLabel (char *func_name) {
  char *string_header = "_";
  char *total_string = malloc (sizeof (char) * (strlen(string_header) + strlen (func_name) + 1));
  if (total_string == NULL) {
    allocation_failed ();
  }
  strcpy (total_string, string_header);
  strcpy (total_string + strlen (string_header), func_name); 
  return total_string;
}

/* Create new string label for given string and add to string_label_map s */
char* generateStringLabel(char* str) {
  if (s == NULL) {
    char *label = (char*)malloc(sizeof("__str__") + 4);
    sprintf(label, "__str__%d", STR_COUNT);
    STR_COUNT++;
    s = (string_label_map*)malloc(sizeof(string_label_map) * 1);
    s->label = label;
    s->string = str;
    s->next = NULL;
    return label;
  } else {
    string_label_map* front = s;
    string_label_map* temp = s;
    while (front != NULL) {
      if (strcmp (front->string, str) == 0) {
         return front->label;
        
      }
      temp = front;
      front = front->next;
    }
    char *label = (char*)malloc(sizeof("__str__") + 4);
    sprintf(label, "__str__%d", STR_COUNT);
    STR_COUNT++;
    temp->next = (string_label_map*)malloc(sizeof(string_label_map) * 1);
    temp->next->string = str;
    temp->next->label = label;
    temp->next->next = NULL;
    return label;
  }
}

void generateString(Decl* curr) {
  emitSTRING (curr->contents.str_info.str_literal, curr->data_size);
}

void generateDataValue (int64_t value, size_t data_size) {
  emitWORDVALUE (value);
  for (int i = 1; i < data_size / WORDSIZE; i++) {
    emitWORDVALUE (0);
  }
}

void generateDataLabel (char *label, size_t data_size) {
  emitWORDLABEL (label);
}

/* Create new var label for global vars */
char* generateVarLabel() {
  char* label = (char*)malloc(sizeof("__var__") + 5);
  sprintf(label, "__var__%d", VAR_COUNT);
  VAR_COUNT++;
  return label;
}

char* getStringLabel(string_label_map* temp, char* str) {
  while (temp != NULL) {
    if (!strcmp(str, temp->string)) {
      return temp->label;
    }
    temp = temp->next;
  }
  return NULL;
}
