#include "preprocessor.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string-helpers.h"
#include "utils.h"

/* Helper Functions */
char* ExtractPath(char* filename);
int ProcessFileIncludes(StringDLL** node_ptr,
                        StringDLL** front_ptr,
                        char* path);
size_t AppendInclude(StringDLL** node_ptr,
                     StringDLL** front_ptr,
                     char* path,
                     char* buffer,
                     size_t size);

/* String DLL Functions */
StringDLL* CreateNode(char* name, StringDLL* prev, StringDLL* next);
int StringDLLContainsFromFront(StringDLL* node, char* name);

// Used to ignore #include in comments
int is_comment = 0;

/*
  Takes in a list of count files. For each file it parses the contents and
  extracts any files
  that are included with a #include PRAGMA. If the file is not already included
  for the
  particular file then it is searched. Collectively this produces a new list of
  files. These
  files are stored in the final_files pointer, which will be malloced inside
  ProcessIncludes.
  Includes are unique for each original file but not across mutliple files, so
  final_boundaries
  contains a list of count + 1 elements where elem i is the start index of
  files[i]'s group
  of files and i + 1 is the non inclusive end boundary. Final_boundaries is also
  malloced
  inside ProcessIncludes.
*/
int ProcessIncludes(char** files,
                    size_t count,
                    char*** final_files,
                    int** final_boundaries) {
  int err = 0;
  *final_boundaries = (int*)malloc((count + 1) * sizeof(int));
  if (*final_boundaries == NULL) {
    allocation_failed();
  }
  (*final_boundaries)[0] = 0;
  size_t file_capacity = count + 1;
  size_t file_count = 0;
  *final_files = (char**)malloc(file_capacity * sizeof(char*));
  if (*final_files == NULL) {
    allocation_failed();
  }
  for (int i = 0; i < count; i++) {
    char* path = ExtractPath(files[i]);
    StringDLL* node = CreateNode(files[i], NULL, NULL);
    StringDLL* front = node;
    while (node != NULL) {
      err = ProcessFileIncludes(&node, &front, path) || err;
    }
    while (front != NULL) {
      if (file_count >= file_capacity) {
        file_capacity *= 2;
        *final_files = realloc(*final_files, file_capacity * sizeof(char*));
        if (*final_files == NULL) {
          allocation_failed();
        }
      }
      (*final_files)[file_count++] = front->name;
      node = front;
      front = front->next;
      free(node);
    }
    (*final_boundaries)[i + 1] = file_count;
    free(path);
  }
  return err;
}

/*
  Takes in a filename and returns the relative path from where the executable
  is being run to the file. This function mallocs memory for the new path
  which must be freed.
*/
char* ExtractPath(char* filename) {
  size_t len = strlen(filename);
  char* path = (char*)malloc(sizeof(char) * (len + 1));
  if (path == NULL) {
    allocation_failed();
  }
  path[0] = '\0';
  for (int i = len - 1; i >= 0; i--) {
    if (filename[i] == '/') {
      strncpy(path, filename, i + 1);
      path[i + 1] = '\0';
      break;
    }
  }
  return path;
}

/*
  Takes in two pointers to StringDLL pointers, node_ptr which holds the
  current element being processed and front_ptr which holds the element
  at the front of the list and a path. Reads in the file with name given
  by the node_ptr and finds any name specified by an #include PRAGMA. If
  the concatenation of the path with the filename included are not already
  in the list then a new node is appended to the front of the list.
  When the function terminates it changes the value of node_ptr to the
  proceeding node. Returns 0 if the file can be read and a nonzero value
  if the attempt to open the file does not succeed.
*/
int ProcessFileIncludes(StringDLL** node_ptr,
                        StringDLL** front_ptr,
                        char* path) {
  StringDLL* node = *node_ptr;
  FILE* f = fopen(node->name, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: Unable to open file %s\n", node->name);
    *node_ptr = node->prev;
    return 1;
  }
  size_t mem_size = DEFAULT_BUFFERSIZE;
  size_t buffer_size = 0;
  char* buffer = (char*)malloc(sizeof(char) * mem_size);
  if (buffer == NULL) {
    allocation_failed();
  }
  size_t read_size = 0;
  size_t chars_read = 0;
  size_t buffer_offset = 0;

  while ((read_size = fread(buffer + buffer_size, sizeof(char),
                            mem_size - buffer_size, f)) != 0) {
    buffer_size += read_size;
    while ((chars_read =
                AppendInclude(node_ptr, front_ptr, path, buffer + buffer_offset,
                              buffer_size - buffer_offset)) != 0) {
      buffer_offset += chars_read;
    }

    /*
       Move any remaining elements in the buffer to the front. Note that we
       cannot use strcpy because copying inside the same buffer leads to
       undefined
       behavior.
    */

    /* Set remaining size to to be that in the offset. */
    buffer_size = buffer_size - buffer_offset;

    if (buffer_offset != 0) {
      for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = buffer[i + buffer_offset];
      }
    }

    /*
       Make any necessary increases to the memory capacity of the buffer.
       This is only necessary when buffer_offset == 0, but we will do it
       earlier for efficiency.
    */
    if (buffer_offset < DEFAULT_BUFFERSIZE / 4) {
      mem_size *= 2;
      buffer = (char*)realloc(buffer, mem_size);
      if (buffer == NULL) {
        allocation_failed();
      }
    }

    /* Reset offset. */
    buffer_offset = 0;
  }
  fclose(f);
  *node_ptr = node->prev;
  return 0;
}

/*
   Takes in two pointers to StringDLL pointer, a path and a buffer of file
   contents of a given size. Scans the buffer for the presence of the
   #include PRAGMA. If found it attempts to append that name concatenated
   with the path to the front of the list (assuming it is not already included).
   Returns the number of characters that were searched to find the PRAGMA
   (and thus do not need to be searched again).
*/
size_t AppendInclude(StringDLL** node_ptr,
                     StringDLL** front_ptr,
                     char* path,
                     char* buffer,
                     size_t size) {
  size_t init_start = 0;
  while (is_comment) {
    if (init_start >= size) {
      return size;
    } else if (buffer[init_start++] == '\n') {
      is_comment = 0;
      return init_start;
    }
  }
  while (init_start < size) {
    if (buffer[init_start] == '/') {
      if (init_start + 1 >= size) {
        return init_start;
      } else if (buffer[init_start + 1] == '/') {
        is_comment = 1;
        return init_start + 2;
      } else {
        init_start += 1;
      }
    } else if (buffer[init_start] == '\'') {
      init_start++;
      int str_len = 0;
      int search = 1;
      while (init_start + str_len < size && search) {
        if (buffer[init_start + str_len] == '\\') {
          if (init_start + str_len + 1 < size &&
              replace_escape_in_character(buffer + init_start + str_len) !=
                  -1) {
            str_len += 2;
          } else {
            str_len += 1;
          }
        } else if (buffer[init_start + str_len] == '\'' ||
                   buffer[init_start + str_len] == '\n') {
          search = 0;
          init_start += str_len;
        } else {
          str_len += 1;
        }
      }
      if (search) {
        return init_start;
      }
    } else if (buffer[init_start] == '"') {
      init_start++;
      int str_len = 0;
      int search = 1;
      while (init_start + str_len < size && search) {
        if (buffer[init_start + str_len] == '\\') {
          if (init_start + str_len + 1 < size &&
              replace_escape_in_string(buffer + init_start + str_len) != -1) {
            str_len += 2;
          } else {
            str_len += 1;
          }
        } else if (buffer[init_start + str_len] == '"' ||
                   buffer[init_start + str_len] == '\n') {
          search = 0;
          init_start += str_len;
        } else {
          str_len += 1;
        }
      }
      if (search) {
        return init_start;
      }
    } else if (buffer[init_start] == '#') {
      if (strlen(PREPROCESSOR_PRAGMA) + init_start >= size) {
        return init_start;
      }
      int match = 1;
      int i;
      for (i = 0; i < strlen(PREPROCESSOR_PRAGMA); i++) {
        if (buffer[init_start + i] != PREPROCESSOR_PRAGMA[i]) {
          match = 0;
          break;
        }
      }
      if (match) {
        int start = init_start;
        while (start < size && buffer[start] == ' ') {
          start++;
        }
        if (start >= size) {
          return init_start;
        } else if (buffer[start] == '"') {
          start++;
          char* dest = NULL;
          size_t str_len = 0;
          int search = 1;
          while (start + str_len < size && search) {
            if (buffer[start + str_len] == '\\') {
              if (start + str_len + 1 < size &&
                  replace_escape_in_string(buffer + start + str_len) != -1) {
                str_len += 2;
              } else {
                str_len += 1;
              }
            } else if (buffer[start + str_len] == '"') {
              search = 0;
              dest = (char*)malloc(sizeof(char) * (str_len + 1));
              if (dest == NULL) {
                allocation_failed();
              }
              char temp_buffer[str_len + 1];
              /* Create a string token. */
              for (int j = 0; j < str_len; j++) {
                temp_buffer[j] = buffer[start + j];
              }
              temp_buffer[str_len] = '\0';
              start += str_len + 1;
              int i = 0;
              int j = 0;
              while (i < strlen(temp_buffer)) {
                int escaped = replace_escape_in_string(temp_buffer + i);
                if (escaped == -1) {
                  dest[j++] = temp_buffer[i++];
                } else {
                  dest[j++] = escaped;
                  i += 2;
                }
              }
              dest[j] = '\0';
            } else if (!isprint(buffer[start + str_len])) {
              return start + str_len;
            } else {
              str_len += 1;
            }
          }
          char* strings[2];
          strings[0] = path;
          strings[1] = dest;
          char* final_path = str_concat(strings, 2);
          if (!StringDLLContainsFromFront(*front_ptr, final_path)) {
            StringDLL* node = CreateNode(final_path, NULL, *front_ptr);
            (*front_ptr)->prev = node;
            *front_ptr = node;
          }
          free(final_path);
          free(dest);
          return start;
        } else {
          return start;
        }
      } else {
        init_start += i;
      }
    } else {
      init_start++;
    }
  }
  return init_start;
}

/*
  Takes in a name, a point to a previous, a pointer to next and creates
  a new StringDLL node properly initialized. It is assumed that name
  may not be valid when the function terminates to memory must be allocated
  for name.
*/
StringDLL* CreateNode(char* name, StringDLL* prev, StringDLL* next) {
  StringDLL* node = (StringDLL*)malloc(sizeof(StringDLL));
  if (node == NULL) {
    allocation_failed();
  }
  node->prev = prev;
  node->next = next;
  node->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(node->name, name);
  return node;
}

/*
  Takes in a node and a name and checks if the name is stored in that
  node or any node that follows it.
*/
int StringDLLContainsFromFront(StringDLL* node, char* name) {
  if (node == NULL) {
    return 0;
  }
  if (strcmp(node->name, name) == 0) {
    return 1;
  }
  return StringDLLContainsFromFront(node->next, name);
}
