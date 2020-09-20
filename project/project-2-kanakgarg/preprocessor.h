#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#define PREPROCESSOR_PRAGMA "#include"

/*
  Doubly Linked List for holding strings. Used by the preprocessor to hold the
  list of files that should be included.
*/
typedef struct StringDLL {
  char *name;
  struct StringDLL *prev;
  struct StringDLL *next;
} StringDLL;

/*
  Takes in a list of count files. For each file it parses the contents and extracts any files
  that are included with a #include PRAGMA. If the file is not already included for the
  particular file then it is searched. Collectively this produces a new list of files. These
  files are stored in the final_files pointer, which will be malloced inside ProcessIncludes.
  Includes are unique for each original file but not across mutliple files, so final_boundaries
  contains a list of count + 1 elements where elem i is the start index of files[i]'s group
  of files and i + 1 is the non inclusive end boundary. Final_boundaries is also malloced
  inside ProcessIncludes.
*/
int ProcessIncludes (char**files, size_t count, char***final_files, int** final_boundaries);
#endif
