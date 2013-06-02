/*
   lpp_func_decl.h 
     Declarations of the built-in LPP functions. 
   
   Copyright 2013 Aaron Hosford

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */



#ifndef LPP_FUNCS_H
#define LPP_FUNCS_H


#include <stdio.h>

#ifdef __WINDOWS__
#include <windows.h>
#endif

#ifdef __STARNIX__
#include <sys/types.h>
#include <dirent.h>
#endif


/* Creates a new class. */
Lobj *LFdefclass(Lobj *args);

/* Creates a new object of the given class. */
Lobj *LFnew(Lobj *args);

/* Calls the specified method of the given object on the remaining arguments. */
Lobj *LFcall_method(Lobj *args);

/* Returns the value of the selected member of the given object. Exception if not allowed. */
Lobj *LFget_member(Lobj *args);

/* Sets the value of the selected member of the given object. Exception if not allowed. */
Lobj *LFset_member(Lobj *args);

/* Prints a (hopefully) helpful blurb for new users. */
Lobj *LFhelp(Lobj *args);

/* Prints the name and value of each symbol in the symbol table. */
Lobj *LFshow_symbols(Lobj *args);

/* Returns this LPP executable's version number. */
Lobj *LFversion(Lobj *args);

/* Returns: A new cons object. */
Lobj *LFcons(Lobj *args);

/* Returns: The car of the argument. */
Lobj *LFcar(Lobj *args);

/* Returns: The cdr of the argument. */
Lobj *LFcdr(Lobj *args);

/* Sets the car of argument 1 to argument 2. Returns: Argument 2. */
Lobj *LFsetcar(Lobj *args);

/* Sets the cdr of argument 1 to argument 2. Returns: Argument 2. */
Lobj *LFsetcdr(Lobj *args);

/* Returns: A new list of the arguments. */
Lobj *LFlist(Lobj *args);

/* Returns: The length of the argument. */
Lobj *LFlist_length(Lobj *args);

/* Returns: The sum of the arguments. */
Lobj *LFplus(Lobj *args);

/* Returns: The product of the arguments. */
Lobj *LFtimes(Lobj *args);

/* Returns: The negation of the argument. */
Lobj *LFnegative(Lobj *args);

/* Returns: The ratio of the arguments. */
Lobj *LFdivided_by(Lobj *args);

/* Returns: If each argument is greater than the previous, t, else nil. */
Lobj *LFgreater_than(Lobj *args);

/* Returns: If each argument is less than the previous, t, else nil. */
Lobj *LFless_than(Lobj *args);

/* Returns: The mantissa of a rational number. */
Lobj *LFmantissa(Lobj *args);

/* Returns: The square root of a non-negative rational number. */
Lobj *LFsquare_root(Lobj *args);

/* Returns: The absolute value of a rational number. */
Lobj *LFabsolute_value(Lobj *args);

/* Returns: The quote's value. */
Lobj *LFquote_value(Lobj *args);

/* Returns: A new quote object. */
Lobj *LFquote(Lobj *args);

/* Prints the concatenation of any number of strings to stdout. Returns: The concatenation of the arguments. */
Lobj *LFprints(Lobj *args);

/* Returns: The concatenation of the arguments. */
Lobj *LFcat(Lobj *args);

/* Returns: The substring of argument 1 defined by range [argument 2, argument 3). */
Lobj *LFsubstr(Lobj *args);

/* Returns: The length of the longest shared prefix of two strings. */
Lobj *LFstring_shared_prefix(Lobj *args);

/* Returns: Non-nil iff the second string is a prefix of the first. */
Lobj *LFstring_has_prefix(Lobj *args);

/* Returns: The location of the earliest occurrence of the second string in the first. */
Lobj *LFstring_find_substring(Lobj *args);

/* Returns: The length of the string. */
Lobj *LFstrlen(Lobj *args);

/* Returns: The name of the argument. */
Lobj *LFsymbol_name(Lobj *args);

/* Returns: The effective value of the argument. */
Lobj *LFsymbol_value(Lobj *args);

/* Sets the value of argument 1 to argument 2. Returns: Argument 2. */
Lobj *LFsymbol_set(Lobj *args);

/* Returns: A string representation of the argument. */
Lobj *LFto_string(Lobj *args);

/* Evaluates the argument. Returns: The result of the evaluation of the argument. */
Lobj *LFeval(Lobj *args);

/* Returns: The type of the argument. */
Lobj *LFtype_of(Lobj *args);

/* Returns: A copy of the argument if one can be made, otherwise the argument. */
Lobj *LFcopy(Lobj *args);

/* Returns: If the arguments are equal, t, else nil. */
Lobj *LFequals(Lobj *args);

/* Returns: If the objects are identical, t, else nil. */
Lobj *LFidentical(Lobj *args);

/* Sets the value of a symbol's doc string. */
Lobj *LFset_symbol_doc(Lobj *args);

/* Creates a new function object. Returns: The new function object. */
Lobj *LFdefun(Lobj *args);

/* If the value of argument 1 is non-nil, evaluates argument 2, otherwise evaluates argument 3. Returns: The result of whichever argument gets evaluated. */
Lobj *LFif(Lobj *args);

/*  */
Lobj *LFdo(Lobj *args);

/*  */
Lobj *LFdo1(Lobj *args);

/*  */
Lobj *LFwhile(Lobj *args);

/*  */
Lobj *LFnot(Lobj *args);

/*  */
Lobj *LFand(Lobj *args);

/*  */
Lobj *LFor(Lobj *args);

/*  */
Lobj *LFfopen(Lobj *args);

/*  */
Lobj *LFis_open(Lobj *args);

/*  */
Lobj *LFclose(Lobj *args);

/*  */
Lobj *LFgetc(Lobj *args);

/*  */
Lobj *LFload(Lobj *args);

/*  */
Lobj *LFpreprocess(Lobj *args);

/*  */
Lobj *LFsystem(Lobj *args);

/* Ends execution immediately without unwinding the execution stack. Returns: DOES NOT RETURN. */
Lobj *LFabort(Lobj *args);

/* Returns: The documentation of a function. */
Lobj *LFdoc(Lobj *args);

/* Catches an exception while evaluating the first argument if it occurs, binds it to the symbol provided and executes the remaining argument. */
Lobj *LFcatch(Lobj *args);

/* Throws (raises) an exception. */
Lobj *LFthrow(Lobj *args);

/* Sets the prompt string for user-interaction mode. */
Lobj *LFset_prompt(Lobj *args);

/* Returns the prompt string for user-interaction mode. */
Lobj *LFget_prompt(Lobj *args);

/* Makes a symbol's value local during the evaluation of the expression. */
Lobj *LFlocal(Lobj *args);

/* Creates a new array of the requested size. */
Lobj *LFnew_array(Lobj *args);

/* Returns the value stored in the array at the given index. */
Lobj *LFget_array_index(Lobj *args);

/* Sets the value stored in the array at the given index to the given value and returns it. */
Lobj *LFset_array_index(Lobj *args);

/* Returns an array holding a subinterval of the original array's values. */
Lobj *LFarray_interval(Lobj *args);

/* Returns the concatenation of any number of arrays. */
Lobj *LFarray_cat(Lobj *args);

/* Returns the size of the array. */
Lobj *LFarray_size(Lobj *args);

/* Converts a list of values to an array. */
Lobj *LFlist_to_array(Lobj *args);

/* Converts a number to a string containing its corresponding ascii character. */
Lobj *LFnumber_to_char(Lobj *args);

/* Gets the value of an environment variable. */
Lobj *LFget_env(Lobj *args);

/* Gets a list of the contents of a directory. */
Lobj *LFlist_dir(Lobj *args);

/* Returns whether the path is a directory. */
Lobj *LFis_dir(Lobj *args);

/* Returns whether the path is a directory. */
Lobj *LFis_file(Lobj *args);


#endif /* LPP_FUNCS_H */
