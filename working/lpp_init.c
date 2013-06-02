/*
   lpp_init.c 
     LPP interpreter initialization. 
   
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



#ifndef LPP_INIT_C
#define LPP_INIT_C

#include "lpp_init.h"

#define LTYPE_SETUP(tp, mprefix) {\
  Ltypes[tp].do_delete = Ldelete_##mprefix;\
  Ltypes[tp]._1methods[LTO_STRING] = L##mprefix##_to_string;\
  Ltypes[tp]._1methods[LEVAL] = L##mprefix##_eval;\
  Ltypes[tp]._1methods[LCOPY] = L##mprefix##_copy;\
  Ltypes[tp]._2methods[LEQUALS] = L##mprefix##_equals;\
  Ltypes[tp].token = mprefix##_token;\
}

void Linit_types() {
  Lobj *symbol_token = Lmake_type_token("@symbol");
  Lobj *number_token = Lmake_type_token("@number");
  Lobj *string_token = Lmake_type_token("@string");
  Lobj *cons_token = Lmake_type_token("@cons-cell");
  Lobj *quote_token = Lmake_type_token("@quote-cell");
  Lobj *func_token = Lmake_type_token("@function");
  Lobj *lambda_token = func_token;
  Lobj *stream_token = Lmake_type_token("@stream");
  Lobj *type_token_token = Lmake_type_token("@class");
  Lobj *exception_token = Lmake_type_token("@exception");
  Lobj *deactivated_exception_token = Lmake_type_token("@caught-exception");
  Lobj *UDclass_token = Lmake_type_token("@class");
  Lobj *UDobject_token = Lmake_type_token("@object");
  Lobj *array_token = Lmake_type_token("@array");

  LTYPE_SETUP(LTSYMBOL, symbol);
  LTYPE_SETUP(LTNUMBER, number);
  LTYPE_SETUP(LTSTRING, string);
  LTYPE_SETUP(LTCONS, cons);
  LTYPE_SETUP(LTQUOTE, quote);
  LTYPE_SETUP(LTFUNC, func);
  LTYPE_SETUP(LTLAMBDA, lambda);
  LTYPE_SETUP(LTSTREAM, stream);
  LTYPE_SETUP(LTTYPE_TOKEN, type_token);
  LTYPE_SETUP(LTEXCEPTION, exception);
  LTYPE_SETUP(LTDEACTIVATED_EXCEPTION, exception);
  LTYPE_SETUP(LTUDCLASS, UDclass);
  LTYPE_SETUP(LTUDOBJECT, UDobject);
  LTYPE_SETUP(LTARRAY, array);
  
  /* Special cases: */
  Ltypes[LTDEACTIVATED_EXCEPTION].token = deactivated_exception_token;
}

#undef LTYPE_SETUP


#define LFUNC_SETUP(fc, lname, doc) \
LSET(Lcons_car(Lsymbol_vals(Llookup_symbol(lname + 1))), Lmake_func(lname, doc, LF##fc))

#define LFUNC_ALSO(symval, lname) \
LSET(Lcons_car(Lsymbol_vals(Llookup_symbol(lname))), symval)

void Linit_funcs() {
  
  LFUNC_SETUP(defclass, "@defclass", "Creates a new class.");
  LFUNC_SETUP(new, "@new", "Creates a new object of the given class.");
  LFUNC_ALSO(LFUNC_SETUP(call_method, "@call-method", "Calls the specified method of the given object on the remaining arguments."), "::");
  LFUNC_SETUP(get_member, "@get-member", "Returns the value of the selected member of the given object. Exception if not allowed.");
  LFUNC_SETUP(set_member, "@set-member", "Sets the value of the selected member of the given object. Exception if not allowed.");
  LFUNC_SETUP(help, "@help", "Prints a (hopefully) helpful blurb for new users.");
  LFUNC_SETUP(show_symbols, "@show-symbols", "Prints the name and value of each symbol in the symbol table.");
  LFUNC_SETUP(version, "@version", "Returns this LPP executable's version number.");
  LFUNC_SETUP(cons, "@cons", "Returns: A new cons object.");
  LFUNC_ALSO(LFUNC_ALSO(LFUNC_SETUP(car, "@car", "Returns: The car of the argument."), "first"), "head");
  LFUNC_ALSO(LFUNC_ALSO(LFUNC_SETUP(cdr, "@cdr", "Returns: The cdr of the argument."), "rest"), "tail");
  LFUNC_SETUP(setcar, "@set-car", "Sets the car of argument 1 to argument 2. Returns: Argument 2.");
  LFUNC_SETUP(setcdr, "@set-cdr", "Sets the cdr of argument 1 to argument 2. Returns: Argument 2.");
  LFUNC_SETUP(list, "@list", "Returns: A new list of the arguments.");
  LFUNC_SETUP(list_length, "@list-length", "Returns: The length of the argument.");
  LFUNC_ALSO(LFUNC_SETUP(plus, "@plus", "Returns: The sum of the arguments."), "+");
  LFUNC_ALSO(LFUNC_SETUP(times, "@times", "Returns: The product of the arguments."), "*");
  LFUNC_ALSO(LFUNC_SETUP(negative, "@negative", "Returns: The negation of the argument."), "-");
  LFUNC_ALSO(LFUNC_SETUP(divided_by, "@divided-by", "Returns: The ratio of the arguments."), "/");
  LFUNC_ALSO(LFUNC_SETUP(greater_than, "@greater-than", "Returns: If each argument is greater than the previous, t, else nil."), ">");
  LFUNC_ALSO(LFUNC_SETUP(less_than, "@less-than", "Returns: If each argument is less than the previous, t, else nil."), "<");
  LFUNC_SETUP(mantissa, "@mantissa", "Returns: The mantissa of a rational number.");
  LFUNC_SETUP(square_root, "@square-root", "Returns: The square root of a non-negative rational number.");
  LFUNC_ALSO(LFUNC_SETUP(absolute_value, "@absolute-value", "Returns: The absolute value of a rational number."), "abs");
  LFUNC_SETUP(quote_value, "@quote-value", "Returns: The quote's value.");
  LFUNC_SETUP(quote, "@quote", "Returns: A new quote object.");
  LFUNC_SETUP(prints, "@prints", "Prints the concatenation of any number of strings to stdout. Returns: The concatenation of the arguments.");
  LFUNC_SETUP(cat, "@cat", "Returns: The concatenation of the arguments.");
  LFUNC_SETUP(substr, "@substr", "Returns: The substring of argument 1 defined by range [argument 2, argument 3).");
  LFUNC_SETUP(string_shared_prefix, "@string-shared-prefix", "Returns: The length of the longest shared prefix of two strings.");
  LFUNC_SETUP(string_has_prefix, "@string-has-prefix", "Returns: Non-nil iff the second string is a prefix of the first.");
  LFUNC_SETUP(string_find_substring, "@find-substring", "Returns: The location of the earliest occurrence of the second string in the first.");
  LFUNC_SETUP(strlen, "@strlen", "Returns: The length of the string.");
  LFUNC_SETUP(symbol_name, "@symbol-name", "Returns: The name of the argument.");
  LFUNC_SETUP(symbol_value, "@value", "Returns: The effective value of the argument.");
  LFUNC_ALSO(LFUNC_SETUP(symbol_set, "@set", "Sets the value of argument 1 to argument 2. Returns: Argument 2."), ":=");
  LFUNC_ALSO(LFUNC_SETUP(to_string, "@to-string", "Returns: A string representation of the argument."), "$");
  LFUNC_SETUP(eval, "@eval", "Evaluates the argument. Returns: The result of the evaluation of the argument.");
  LFUNC_SETUP(type_of, "@type-of", "Returns: The type of the argument.");
  LFUNC_SETUP(copy, "@copy", "Returns: A copy of the argument if one can be made, otherwise the argument.");
  LFUNC_SETUP(equals, "@equals", "Returns: If the arguments are equal, t, else nil.");
  LFUNC_ALSO(LFUNC_SETUP(identical, "@identical", "Returns: If the objects are identical, t, else nil."), "=");
  LFUNC_SETUP(set_symbol_doc, "@set-symbol-doc", "Sets the value of a symbol's doc string.");
  LFUNC_SETUP(defun, "@defun", "Creates a new function object. Returns: The new function object.");
  LFUNC_ALSO(LFUNC_SETUP(if, "@if", "If the value of argument 1 is non-nil, evaluates argument 2, otherwise evaluates argument 3. Returns: The result of whichever argument gets evaluated."), "?");
  LFUNC_SETUP(do, "@do", "");
  LFUNC_SETUP(do1, "@do1", "");
  LFUNC_SETUP(while, "@while", "");
  LFUNC_ALSO(LFUNC_SETUP(not, "@not", ""), "!");
  LFUNC_ALSO(LFUNC_SETUP(and, "@and", ""), "&&");
  LFUNC_ALSO(LFUNC_SETUP(or, "@or", ""), "||");
  LFUNC_SETUP(fopen, "@fopen", "");
  LFUNC_SETUP(is_open, "@is-open", "");
  LFUNC_SETUP(close, "@close", "");
  LFUNC_SETUP(getc, "@getc", "");
  LFUNC_SETUP(load, "@load-file", "");
  LFUNC_SETUP(preprocess, "@preprocess", "");
  LFUNC_SETUP(system, "@system", "");
  LFUNC_ALSO(LFUNC_ALSO(LFUNC_ALSO(LFUNC_SETUP(abort, "@abort", "Ends execution immediately without unwinding the execution stack. Returns: DOES NOT RETURN."), "quit"), "exit"), "bye");
  LFUNC_SETUP(doc, "@doc", "Returns: The documentation of a function.");
  LFUNC_SETUP(catch, "@catch", "Catches an exception while evaluating the first argument if it occurs, binds it to the symbol provided and executes the remaining argument.");
  LFUNC_SETUP(throw, "@throw", "Throws (raises) an exception.");
  LFUNC_SETUP(set_prompt, "@set-prompt", "Sets the prompt string for user-interaction mode.");
  LFUNC_SETUP(get_prompt, "@get-prompt", "Returns the prompt string for user-interaction mode.");
  LFUNC_SETUP(local, "@local", "Makes a symbol's value local during the evaluation of the expression.");
  LFUNC_SETUP(new_array, "@new-array", "Creates a new array of the requested size.");
  LFUNC_SETUP(get_array_index, "@get-array-index", "Returns the value stored in the array at the given index.");
  LFUNC_SETUP(set_array_index, "@set-array-index", "Sets the value stored in the array at the given index to the given value and returns it.");
  LFUNC_SETUP(array_interval, "@array-interval", "Returns an array holding a subinterval of the original array's values.");
  LFUNC_SETUP(array_cat, "@array-cat", "Returns the concatenation of any number of arrays.");
  LFUNC_SETUP(array_size, "@array-size", "Returns the size of the array.");
  LFUNC_SETUP(list_to_array, "@list-to-array", "Converts a list of values to an array.");
  LFUNC_SETUP(number_to_char, "@number-to-ascii", "Converts a number to a string containing its corresponding ascii character.");
  LFUNC_SETUP(get_env, "@get-env", "Gets the value of an environment variable.");
  LFUNC_SETUP(list_dir, "@list-dir", "Gets a list of the contents of a directory.");
  LFUNC_SETUP(is_dir, "@is-dir", "Returns whether the path is a directory.");
  LFUNC_SETUP(is_file, "@is-file", "Returns whether the path is a directory.");
}

#undef LFUNC_ALSO
#undef LFUNC_SETUP

void Linit() {
  Linit_types();
  Linit_funcs();
  
  Lquiet_mode = 0;
  
  Lprompt_input = 1;
  Linput_report = 1;
  Loutput_report = 1;
  
  LINIT(Linput_prompt_string, Lmake_string("\n\n: "));
  LINIT(Linput_report_prompt_string, Lmake_string("\n< "));
  LINIT(Loutput_report_prompt_string, Lmake_string("\n> "));
  
  LINIT(Lt, LSET(Lcons_car(Lsymbol_vals(Llookup_symbol("t"))), Llookup_symbol("t")));
  Llookup_symbol("nil");
  
  char *temp = (char *) getenv("LPPPATH");
  if(temp) {
    bin_dir = temp;
  } else {
    bin_dir = ".";
  }
  LSET(Lcons_car(Lsymbol_vals(Llookup_symbol("lpp-path"))), Lmake_string(bin_dir));
  
  lpp_version = "0.92";
}

#endif /* LPP_INIT_C */
