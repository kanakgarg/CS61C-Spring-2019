// Global Variable for if we should exit with an error
extern size_t error_code;

/*
  Function used to set the error_code and output an error message
  for when a variable is declared with an unknown type.
*/
void unknown_type_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message
  when a function returns a struct.
*/
void struct_return_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message
  when a function takes a struct as a parameter.
*/
void struct_param_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message
  for when a variable is declared multiple times in the same scope.
*/
void redeclaration_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message
  when a variable uses the same name as a previously-declared function.
*/
void function_conflict_error (char *id, int linenum, char *filename);


/*
  Function used to exit and panic if an AST is malformed. This indicates
  an error in the previous parts of the project.
*/
void malformed_ast ();

/*
  Function used to exit and panic if bool, char, or int is missing from the
  type list. This indicates an error in initializing the type list.
*/
void missing_built_in_type (char *str);

/*
  Function used to set the error code and output an error message if a 
  struct is recursively defined (contains itself as a field).
*/
void recursive_struct_error (char *field, char *struct_name, int linenum, char *filename);

/*
  Function used to set the error code and output an error message if a 
  function is defined twice.
*/
void function_redefinition_error (char *id, int orig_linenum, char *orig_file, int new_linenum, char *new_file);

/*
  Function used to set the error code and output an error message if a function
  is declared twice with conflicting parameters.
*/                  
void declaration_conflict_error (char *id, int orig_linenum, char *orig_file, int new_linenum, char *new_file);

/*
  Function used to set the error code and output an error message if an 
  identifier is used without being previously declared.
*/
void undeclared_variable_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message if an
  identifier is used in a context before it is declared.
*/
void variable_used_before_declaration_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message if an
  identifier which corresponds to a function is used in an expression
  that is not a call expression.
*/
void improper_function_use_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  a return statement has a type which does not match the expected return
  type for the function.
*/
void incorrect_return_type_error (char *id, char *expected_front, char *expected_id, size_t expected_ptr_count, char *actual_front, char *actual_id, size_t actual_ptr_count, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  a call expression has the incorrect number of arguments.
*/
void incorrect_argument_count_error (char *id, int linenum, char *filename, size_t expected_arg_count, size_t actual_arg_count);

/*
  Function used to set the error code and output an error message when a
  a function is needed for a call expression.
*/
void missing_function_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  an expression attempts to assign to a const after declaration.
*/
void const_reassign_error (char *id, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  a . or -> reference a field that does not exist for the given struct.
*/
void missing_field_error (char *field, char *struct_name, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  a function argument has a type that does not match the parameter.
*/
void incorrect_argument_type_error (char *id, int linenum, char *filename, char *expected_front, char *expected_id, size_t expected_ptr_count, char *actual_front, char *actual_id, size_t actual_ptr_count, size_t param_number);

/*
  Function used to set the error code and output an error message when
  a expression which is not a valid LHS is assigned to.
*/
void incorrect_LHS_error (int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  a global variable is initialized to a value not statically deducible
  at the time of generating assembly.
*/
void incorrect_global_RHS_error (int linenum, char *filename);

/*
  Function used to set the error code and output an error message when
  an expression being assigned to has a different type than the RHS.
*/
void assignment_type_error (char *LHS_start, char *LHS_type, char *RHS_start, char *RHS_type, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when a type
  is used with an operator on which it cannot be applied.
*/
void illegal_operator_type_error (char *op, char *type_front, char *type_id, size_t ptr_count, int linenum, char *filename);

/*
  Function used to set the error code and output an error message when a pair
  of types are used with an operator which cannot be used together (although
  both could be used with a different partner type).
*/
void illegal_operator_pair_error (char *op, char *t1_front, char *t1, size_t ptr1, char *t2_front, char *t2, size_t ptr2, int linenum, char *filename);
