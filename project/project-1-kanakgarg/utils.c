#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

/*
  Function to exit the program and report the error for when
  memory allocation fails.
*/
void allocation_failed() {
  fprintf(stderr, "Ran out of memory. Program exiting.\n");
  exit(1);
}
