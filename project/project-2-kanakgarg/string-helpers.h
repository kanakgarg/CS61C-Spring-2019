#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

/*
   Given a single character 'c', this function returns 1 if the character is 
   an English alphabet character (upper or lowercae). Otherwise, the 
   function returns 0. 
*/
int is_alpha (char c);

/*
   Given a single character 'c', the function returns 1 if the character is a
   (white)space character according to our grammar. If it is not a
   (white)space character, the function returns 0.
*/
int is_space (char c);


/*
   Given a single character 'c', the function returns 1 if the charcter is a
   digit according to our grammar. If it is not, the function returns 0.
*/
int is_digit (char c);

/* 
   Given a character pointer 'str', this function first checks to see if
   the first character is an escaped character. If it is not, it returns -1. 
   If it is an escaped character, it returns the character which is escaped.
*/
int replace_escape_in_character (char *str);

/*
   Given a character pointer 'str', this function first checks to see if
   the first character is an escaped character. If it is not, it returns -1.
   If it is an escaped character, it returns the character which is escaped.

   Note: The escape characters valid in strings and characters differ, hence
   the two separate functions. Please see the lexing grammar for details.
*/
int replace_escape_in_string (char *str);

/*
   Given a single character 'c', this function returns 1 if the character is
   a component of an identifier (letter, digit, or underscore). Otherwise,
   the function returns 0.
*/
int is_identifier_component (char c);

/*
   Given a character pointer 'str', this function returns 1 if the string
   is a valid identifier. Consider using the function above! If it is not
   a valid identifier, the function returns 0.
*/
int is_valid_identifier (char* str);

/* 
   Given two character pointers 'src' and 'dest', this function copies from 
   'src' to 'dest' and changes every uppercase character to  
   lowercase.  
   The resulting string in 'dest' is null terminated.  
*/
void lower_strcpy (char *dest, char *src);

/*
   Given a list of character pointers 'strings' and the length of the list
   'count', this function returns a newly allocated character pointer
   containing all characters in 'strings' concatenated together.
   The returned character pointer should be null terminated.
*/
char *str_concat (char **strings, size_t count);
#endif
