"
lpp_funcs.lsp
  Contains the definitions for all built-in LPP functions.
  The definitions given here are used to generate the templated
  LPP source files from the templates in source/templates.

Copyright 2013 Aaron Hosford

Licensed under the Apache License, Version 2.0 (the 'License');
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an 'AS IS' BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"

(prints "\nLoading lpp_funcs.lsp\n")
(set 'lpp_funcs.lsp-loaded t)

(if (! lpp.lsp-loaded) '(load "lpp.lsp"))

(lpp-func "defclass" '("defclass")
  "Creates a new class."
  '("" ("CONS" "CONS" ()))
  '(cat "  " (lpp-func-protect-args)
        "  Lobj *ret = Lmake_UDclass(" (lpp-func-getarg 1) ", " (lpp-func-getarg 2) ");\n"
        "  " (lpp-func-unprotect-args)
        "  " (lpp-func-return "ret")))

(lpp-func "new" '("new")
  "Creates a new object of the given class."
  '("" ("UDCLASS" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_UDobject(" (lpp-func-getarg 1) ")"))))

(lpp-func "call_method" '("call-method" "::")
  "Calls the specified method of the given object on the remaining arguments."
  '("" ("UDOBJECT" "SYMBOL" "NIL"))
  '(cat "  " (lpp-func-protect-args)
        "  Lobj *ret = LUDobject_call_method("
                       (lpp-func-getarg 1)
                       ", LUDclass_select_method(LUDobject_class("
                       (lpp-func-getarg 1)
                       "), "
                       (lpp-func-getarg 2)
                       "), "
                       (lpp-func-args-after 2)
                       ");\n"
        "  " (lpp-func-unprotect-args)
        "  " (lpp-func-return "ret")))

(lpp-func "get_member" '("get-member")
  "Returns the value of the selected member of the given object. Exception if not allowed."
  '("" ("UDOBJECT" "SYMBOL"))
  '(cat "  " (lpp-func-return (cat "LUDobject_get_member("
                                   (lpp-func-getarg 1)
                                   ", LUDclass_select_member(LUDobject_class("
                                   (lpp-func-getarg 1)
                                   "), "
                                   (lpp-func-getarg 2)
                                   "))"))))

(lpp-func "set_member" '("set-member")
  "Sets the value of the selected member of the given object. Exception if not allowed."
  '("" ("UDOBJECT" "SYMBOL" "NIL"))
  '(cat "  " (lpp-func-return (cat "LUDobject_set_member("
                                   (lpp-func-getarg 1)
                                   ", LUDclass_select_member(LUDobject_class("
                                   (lpp-func-getarg 1)
                                   "), "
                                   (lpp-func-getarg 2)
                                   "), "
                                   (lpp-func-getarg 3)
                                   ")"))))

(lpp-func "help" '("help")
  "Prints a (hopefully) helpful blurb for new users."
  '("" (()))
  '(cat "  printf(\"\\n\\n\\tType \\\"(show-symbols)\\\" for a list of symbols and their values.\");\n"
        "  printf(\"\\n\tType \\\"(doc <symbol>)\\\" for an explanation of <symbol>.\\n\\n\");\n"
        "  " (lpp-func-return "Lnil")))

(lpp-func "show_symbols" '("show-symbols")
  "Prints the name and value of each symbol in the symbol table."
  '((cat " || !" (lpp-func-arg-is-nil-or 1 "STRING")
         " || !" (lpp-func-arg-is-nil-or 2 "STRING"))
    ("NIL" "NIL" ()))
  '(cat "  Lprinto(" (lpp-func-arglist) ");\n"
        "  const char *min = " (lpp-func-getarg-else-nil 1) " ? Lstring_cval(" (lpp-func-getarg 1) ") : 0;\n"
        "  const char *max = " (lpp-func-getarg-else-nil 2) " ? Lstring_cval(" (lpp-func-getarg 2) ") : 0;\n"
        "  Lprint_table(min, max);\n"
        "  " (lpp-func-return "Lnil")))

(lpp-func "version" '("version")
  "Returns this LPP executable's version number."
  '("" (()))
  '(cat "  " (lpp-func-return "Lmake_string(lpp_version)")))

(lpp-func "cons" '("cons")
  "Returns: A new cons object."
  '("" ("NIL" "CONS" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_cons("
                                   (lpp-func-getarg 1)
                                   ", "
                                   (lpp-func-getarg 2)
                                   ")"))))
        
(lpp-func "car" '("car" "first" "head")
  "Returns: The car of the argument."
  '("" ("CONS" ()))
  '(cat "  if(!"
        (lpp-func-getarg 1)
        ")\n"
        "    " (lpp-func-return "Lnil")
        "  " (lpp-func-return (cat "Lcons_car(" (lpp-func-getarg 1) ")"))))
        
(lpp-func "cdr" '("cdr" "rest" "tail")
  "Returns: The cdr of the argument."
  '("" ("CONS" ()))
  '(cat "  if(!"
        (lpp-func-getarg 1)
        ")\n"
        "    " (lpp-func-return "Lnil")
        "  " (lpp-func-return (cat "Lcons_cdr(" (lpp-func-getarg 1) ")"))))
        
(lpp-func "setcar" '("set-car")
  "Sets the car of argument 1 to argument 2. Returns: Argument 2."
  '((cat " || !" (lpp-func-getarg 1))
    ("CONS" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "LSET(Lcons_car("
                                   (lpp-func-getarg 1)
                                   "), "
                                   (lpp-func-getarg 2)
                                   ")"))))

(lpp-func "setcdr" '("set-cdr")
  "Sets the cdr of argument 1 to argument 2. Returns: Argument 2."
  '((cat "|| !" (lpp-func-getarg 1))
    ("CONS" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "LSET(Lcons_cdr("
                                   (lpp-func-getarg 1)
                                   "), "
                                   (lpp-func-getarg 2)
                                   ")"))))

(lpp-func "list" '("list")
  "Returns: A new list of the arguments."
  '("" ("NIL"))
  '(cat "  if(!" (lpp-func-more-args) ")\n"
        "    " (lpp-func-return "Lnil")
        "  " (lpp-func-return (cat "Llist_copy(" (lpp-func-arglist) ")"))))
    
(lpp-func "list_length" '("list-length")
  "Returns: The length of the argument."
  '("" ("CONS" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(Llist_length("
                                   (lpp-func-getarg 1)
                                   "))"))))
     
(lpp-func "plus" '("plus" "+")
  "Returns: The sum of the arguments."
  '("" ("NUMBER"))
  '(cat "  double sum = 0;\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    sum+= Lnumber_cval(" (lpp-func-getarg 1) ");\n"
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  " (lpp-func-return "Lmake_number(sum)")))
        
(lpp-func "times" '("times" "*")
  "Returns: The product of the arguments."
  '("" ("NUMBER"))
  '(cat "  double product = 1;\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    product*= Lnumber_cval(" (lpp-func-getarg 1) ");\n"
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  " (lpp-func-return "Lmake_number(product)")))

(lpp-func "negative" '("negative" "-")
  "Returns: The negation of the argument."
  '("" ("NUMBER" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(-Lnumber_cval("
                                   (lpp-func-getarg 1)
                                   "))"))))

(lpp-func "divided_by" '("divided-by" "/")
  "Returns: The ratio of the arguments."
  '("" ("NUMBER" "NUMBER" ()))
  '(cat "  if(Lnumber_cval(" (lpp-func-getarg 2) "))\n"
        "    " (lpp-func-return (cat "Lmake_number(Lnumber_cval("
                                     (lpp-func-getarg 1)
                                     ") / Lnumber_cval("
                                     (lpp-func-getarg 2)
                                     "))"))
        "  else\n"
        "    " (lpp-func-return "Lmake_exception(Lmake_string(\"Divide by zero. \"), Lnil)")))

(lpp-func "greater_than" '("greater-than" ">")
  "Returns: If each argument is greater than the previous, t, else nil."
  '("" ("NUMBER"))
  '(cat "  double ceiling = Lnumber_cval("
        (lpp-func-getarg 1) ");\n"
        "  " (lpp-func-next-arg) ";\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    if(ceiling <= Lnumber_cval(" (lpp-func-getarg 1) "))\n"
        "      " (lpp-func-return "Lnil")
        "    ceiling = Lnumber_cval(" (lpp-func-getarg 1) ");\n"
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  " (lpp-func-return "Lt")))

(lpp-func "less_than" '("less-than" "<")
  "Returns: If each argument is less than the previous, t, else nil."
  '("" ("NUMBER"))
  '(cat "  double floor = Lnumber_cval("
        (lpp-func-getarg 1) ");\n"
        "  " (lpp-func-next-arg) ";\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    if(floor >= Lnumber_cval(" (lpp-func-getarg 1) "))\n"
        "      " (lpp-func-return "Lnil")
        "    floor = Lnumber_cval(" (lpp-func-getarg 1) ");\n"
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  " (lpp-func-return "Lt")))

(lpp-func "mantissa" '("mantissa")
  "Returns: The mantissa of a rational number."
  '("" ("NUMBER"))
  '(cat "  " (lpp-func-return (cat "Lmake_number(Lnumber_cval(" (lpp-func-getarg 1) ") - ((long int) Lnumber_cval(" (lpp-func-getarg 1) ")))"))))

(lpp-func "square_root" '("square-root")
  "Returns: The square root of a non-negative rational number."
  '("" ("NUMBER"))
  '(cat "  if(Lnumber_cval(" (lpp-func-getarg 1) ") >= 0)\n"
        "  " (lpp-func-return (cat "Lmake_number(sqrt(Lnumber_cval(" (lpp-func-getarg 1) ")))"))
        "  else\n"
        "    " (lpp-func-return "Lmake_exception(Lmake_string(\"Square root of negative number is imaginary. \"), Lnil)")))

(lpp-func "absolute_value" '("absolute-value" "abs")
  "Returns: The absolute value of a rational number."
  '("" ("NUMBER"))
  '(cat "  if(Lnumber_cval(" (lpp-func-getarg 1) ") >= 0)\n"
        "  " (lpp-func-return (cat "Lmake_number(Lnumber_cval(" (lpp-func-getarg 1) "))"))
        "  else\n"
        "  " (lpp-func-return (cat "Lmake_number(-Lnumber_cval(" (lpp-func-getarg 1) "))"))))

(lpp-func "quote_value" '("quote-value")
  "Returns: The quote's value."
  '("" ("QUOTE" ()))
  '(cat "  " (lpp-func-return (cat "Lquote_cval(" (lpp-func-getarg 1) ")"))))
  
(lpp-func "quote" '("quote")
  "Returns: A new quote object."
  '("" ("NIL" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_quote(" (lpp-func-getarg 1) ")"))))

(lpp-func "prints" '("prints")
  "Prints the concatenation of any number of strings to stdout. Returns: The concatenation of the arguments."
  '("" ("STRING" "STRING"))
  '(cat "  Lobj *ret = Lstring_cat(" (lpp-func-arglist) ");\n"
        "  Lprints(ret);\n"
        "  " (lpp-func-return "ret")))

(lpp-func "cat" '("cat")
  "Returns: The concatenation of the arguments."
  '("" ("STRING" "STRING"))
  (cat "  " (lpp-func-return (cat "Lstring_cat(" (lpp-func-arglist) ")"))))
  
(lpp-func "substr" '("substr")
  "Returns: The substring of argument 1 defined by range [argument 2, argument 3)."
  '("" ("STRING" "NUMBER" "NUMBER" ()))
  '(cat "  " (lpp-func-return (cat "Lstring_substring("
                                   (lpp-func-getarg 1)
                                   ", (int) Lnumber_cval("
                                   (lpp-func-getarg 2)
                                   "), (int) Lnumber_cval("
                                   (lpp-func-getarg 3)
                                   "))"))))

(lpp-func "string_shared_prefix" '("string-shared-prefix")
  "Returns: The length of the longest shared prefix of two strings."
  '("" ("STRING" "STRING" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(Lcharstr_shared_prefix(Lchar_ptr_to_charstr(Lstring_cval("
                                   (lpp-func-getarg 1)
                                   ")), Lchar_ptr_to_charstr(Lstring_cval("
                                   (lpp-func-getarg 2)
                                   "))))"))))

(lpp-func "string_has_prefix" '("string-has-prefix")
  "Returns: Non-nil iff the second string is a prefix of the first."
  '("" ("STRING" "STRING" ()))
  '(cat "  " (lpp-func-return (cat "Lcharstr_has_prefix(Lchar_ptr_to_charstr(Lstring_cval("
                                   (lpp-func-getarg 1)
                                   ")), Lchar_ptr_to_charstr(Lstring_cval("
                                   (lpp-func-getarg 2)
                                   "))) ? Lt : Lnil"))))

(lpp-func "string_find_substring" '("find-substring")
  "Returns: The location of the earliest occurrence of the second string in the first."
  '("" ("STRING" "STRING" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(Lcharstr_find_substring(Lchar_ptr_to_charstr(Lstring_cval("
                                   (lpp-func-getarg 1)
                                   ")), Lchar_ptr_to_charstr(Lstring_cval("
                                   (lpp-func-getarg 2)
                                   "))))"))))

(lpp-func "strlen" '("strlen")
  "Returns: The length of the string."
  '("" ("STRING" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(strlen(Lstring_cval(" (lpp-func-getarg 1) ")))"))))

(lpp-func "symbol_name" '("symbol-name")
  "Returns: The name of the argument."
  '("" ("SYMBOL" ()))
  '(cat "  " (lpp-func-return (cat "Lsymbol_name("
                                   (lpp-func-getarg 1)
                                   ")"))))

(lpp-func "symbol_value" '("value")
  "Returns: The effective value of the argument."
  '("" ("SYMBOL" ()))
  '(cat "  " (lpp-func-return (cat "Lcons_car(Lsymbol_vals("
                                   (lpp-func-getarg 1)
                                   "))"))))

(lpp-func "symbol_set" '("set" ":=")
  "Sets the value of argument 1 to argument 2. Returns: Argument 2."
  '("" ("SYMBOL" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "LSET(Lcons_car(Lsymbol_vals("
                                   (lpp-func-getarg 1)
                                   ")), "
                                   (lpp-func-getarg 2)
                                   ")"))))

(lpp-func "to_string" '("to-string" "$")
  "Returns: A string representation of the argument."
  '("" ("NIL" ()))
  '(cat "  " (lpp-func-return (cat "Lto_string("
                                   (lpp-func-getarg 1)
                                   ")"))))

(lpp-func "eval" '("eval")
  "Evaluates the argument. Returns: The result of the evaluation of the argument."
  '("" ("NIL" ()))
  '(cat "  " (lpp-func-return (cat "Leval("
                                   (lpp-func-getarg 1)
                                   ")"))))
        
(lpp-func "type_of" '("type-of")
  "Returns: The type of the argument."
  '("" ("NIL" ()))
  '(cat "  " (lpp-func-return (cat (lpp-func-getarg 1)
                                   " ? Ltypes[("
                                   (lpp-func-getarg 1)
                                   ")->Ltype].token : Ltypes[LTCONS].token"))))
  
(lpp-func "copy" '("copy")
  "Returns: A copy of the argument if one can be made, otherwise the argument."
  '("" ("NIL" ()))
  '(cat "  " (lpp-func-return (cat "Lcopy(" (lpp-func-getarg 1) ")"))))

(lpp-func "equals" '("equals")
  "Returns: If the arguments are equal, t, else nil."
  '("" ("NIL" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "Lequals(" (lpp-func-getarg 1) ", " (lpp-func-getarg 2) ")"))))

(lpp-func "identical" '("identical" "=")
  "Returns: If the objects are identical, t, else nil."
  '("" ("NIL" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "("
                                   (lpp-func-getarg 1)
                                   " == "
                                   (lpp-func-getarg 2)
                                   ") ? Lt : Lnil"))))

(lpp-func "set_symbol_doc" '("set-symbol-doc")
  "Sets the value of a symbol's doc string."
  '((cat " || (" (lpp-func-getarg 2) " && (" (lpp-func-getarg 2) "->Ltype != LTSTRING))")
    ("SYMBOL" "NIL" ()))
  '(cat "  LSET(Lsymbol_doc(" (lpp-func-getarg 1) "), " (lpp-func-getarg 2) ");\n"
        "  " (lpp-func-return (lpp-func-getarg 2))))

(lpp-func "defun" '("defun")
  "Creates a new function object. Returns: The new function object."
  '((cat " || !" (lpp-func-getarg 1)
         " || (" (lpp-func-getarg 2) " && (" (lpp-func-getarg 2) "->Ltype != LTSTRING))")
    ("CONS" "NIL" "NIL" ()))
  '(cat "  Lobj *arglist = " (lpp-func-getarg 1) ";\n"
        "  errval = Lcheck_args(arglist, 1, -1, LTNIL, LTNIL);\n"
        "  if(errval->level) {\n"
        "    /* ERROR */\n"
        "    return " (cat "Lmake_exception(Lmake_string("
                           "\"Bad argument list in function definition: \""
                           "), "
                           (lpp-func-arglist)
                           ");\n")
        "  }\n"
        "  while(arglist) {\n"
        "    if(Lcons_car(arglist) && (Lcons_car(arglist)->Ltype != LTSYMBOL)) {\n"
        "      /* ERROR */\n"
        "      " (lpp-func-return (cat "Lmake_exception(Lmake_string("
                                       "\"Bad argument list in function definition: \""
                                       "), "
                                       (lpp-func-arglist)
                                       ")"))
        "    }\n"
        "    arglist = Lcons_cdr(arglist);\n"
        "  }\n"
        "  " (lpp-func-return (cat "Lmake_lambda("
                                   (lpp-func-getarg 1)
                                   ", "
                                   (lpp-func-getarg 2)
                                   ", "
                                   (lpp-func-getarg 3)
                                   ")"))))

  
(lpp-func "if" '("if" "?")
  (cat "If the value of argument 1 is non-nil, evaluates argument 2, otherwise evaluates argument 3. "
       "Returns: The result of whichever argument gets evaluated.")
  '("" ("NIL" "NIL" () "NIL" ()))
  '(cat "  " (lpp-func-protect-args)
        "  Lobj *ret;\n"
        "  if(" (lpp-func-getarg 1) ")\n"
        "    ret = Leval(" (lpp-func-getarg 2) ");\n"
        "  else\n"
        "    ret = " (lpp-func-is-arg 3) " ?\n"
        "            Leval(" (lpp-func-getarg 3) ") :\n"
        "            Lnil;\n"
        "  " (lpp-func-unprotect-args)
        "  " (lpp-func-return "ret")))

(lpp-func "do" '("do")
  ""
  '("" ("NIL"))
  '(cat "  Lobj *ret = Lnil;\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    " (lpp-func-protect-args)
        "    LSET(ret, Leval(" (lpp-func-getarg 1) "));\n"
        "    " (lpp-func-unprotect-args)
		"    if(ret && ret->Ltype == LTEXCEPTION) {\n"
        "  	   Lrelease_no_delete(ret);\n"
		"      " (lpp-func-return "ret")
		"    }\n"
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  Lrelease_no_delete(ret);\n"
        "  " (lpp-func-return "ret")))

(lpp-func "do1" '("do1")
  ""
  '("" ("NIL"))
  '(cat "  if(!" (lpp-func-more-args) ")\n"
        "    " (lpp-func-return "Lnil")
        "  Lobj *trash = Lnil;\n"
        "  Lobj *ret = Lnil;\n"
        "  " (lpp-func-protect-args)
        "  LSET(ret, Leval(" (lpp-func-getarg 1) "));\n"
        "  " (lpp-func-unprotect-args)
        "  " (lpp-func-next-arg) ";\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    " (lpp-func-protect-args)
        "    LSET(trash, Leval(" (lpp-func-getarg 1) "));\n"
        "    " (lpp-func-unprotect-args)
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  LSET(trash, Lnil);\n"
        "  Lrelease_no_delete(ret);\n"
        "  " (lpp-func-return "ret")))

(lpp-func "while" '("while")
  ""
  '("" ("NIL" "NIL" ()))
  '(cat "  " (lpp-func-protect-args)
        "  Lobj *test = Lnil;\n"
        "  Lobj *ret = Lnil;\n"
        "  unsigned long int count = 0;\n"
        "  while(LSET(test, Leval(" (lpp-func-getarg 1) "))) {\n"
        "    LSET(ret, Leval(" (lpp-func-getarg 2) "));\n"
        "    if(++count == 0) {\n"
        "      " (lpp-func-return (cat "Lmake_exception(Lmake_string(\"Infinite @while loop: \"), "
                                       (lpp-func-arglist)
                                       ")"))
        "    }\n"
        "  }\n"
        "  Lrelease(test);\n"
        "  " (lpp-func-unprotect-args)
        "  Lrelease_no_delete(ret);\n"
        "  " (lpp-func-return "ret")))

(lpp-func "not" '("not" "!")
  ""
  '("" ("NIL" ()))
  '(cat "  if(" (lpp-func-getarg 1) ")\n"
        "    " (lpp-func-return "Lnil")
        "  else\n"
        "    " (lpp-func-return "Lt")))

(lpp-func "and" '("and" "&&")
  ""
  '("" ("NIL"))
  '(cat "  Lobj *ret = Lnil;\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    " (lpp-func-protect-args)
        "    if(!LSET(ret, Leval(" (lpp-func-getarg 1) "))) {\n"
        "      " (lpp-func-unprotect-args)
        "      " (lpp-func-return "Lnil")
        "    }\n"
        "    " (lpp-func-unprotect-args)
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  Lrelease_no_delete(ret);\n"
        "  " (lpp-func-return "ret")))

(lpp-func "or" '("or" "||")
  ""
  '("" ("NIL"))
  '(cat "  Lobj *ret = Lnil;\n"
        "  while(" (lpp-func-more-args) ") {\n"
        "    " (lpp-func-protect-args)
        "    if(LSET(ret, Leval(" (lpp-func-getarg 1) "))) {\n"
        "      Lrelease_no_delete(ret);\n"
        "      " (lpp-func-unprotect-args)
        "      " (lpp-func-return "ret")
        "    }\n"
        "    " (lpp-func-unprotect-args)
        "    " (lpp-func-next-arg) ";\n"
        "  }\n"
        "  " (lpp-func-return "Lnil")))

(lpp-func "fopen" '("fopen")
  ""
  '("" ("STRING" "STRING" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_file_stream(Lstring_cval("
                                   (lpp-func-getarg 1)
                                   "), Lstring_cval("
                                   (lpp-func-getarg 2)
                                   "))"))))

(lpp-func "is_open" '("is-open")
  ""
  '("" ("STREAM" ()))
  '(cat "  " (lpp-func-return (cat "(Lstream_cval("
                                   (lpp-func-getarg 1)
                                   ") && Lis_open(Lstream_cval("
                                   (lpp-func-getarg 1)
                                   "))) ? Lt : Lnil"))))

(lpp-func "close" '("close")
  ""
  '("" ("STREAM" ()))
  '(cat "  if(Lstream_cval(" (lpp-func-getarg 1)
        ") && !Lclose(Lstream_cval(" (lpp-func-getarg 1) ")))\n"
        "    " (lpp-func-return "Lt")
        "  else\n"
        "    " (lpp-func-return "Lnil")))

(lpp-func "getc" '("getc")
  ""
  '("" ("STREAM" ()))
  '(cat "  if(Lstream_cval(" (lpp-func-getarg 1) ")) {\n"
        "    char c[2] = \" \";\n"
        "    int i = Lgetc(Lstream_cval(" (lpp-func-getarg 1) "));\n"
        "    if(i == EOF)\n"
        "      " (lpp-func-return "Lnil")
        "    c[0] = i;\n"
        "    " (lpp-func-return "Lmake_string(c)")
        "  } else\n"
        "    " (lpp-func-return "Lnil")))


(lpp-func "load" '("load-file")
  ""
  '("" ("STREAM" () "NIL" "NIL" ()))
  '(cat "  if(!Lis_open(Lstream_cval(" (lpp-func-getarg 1) ")))\n"
        "    " (lpp-func-return "Lnil")
        "  Lobj *ret = Lnil;\n"
        "  int print = (" (lpp-func-is-arg 2) " && " (lpp-func-getarg 2) ");\n"
        "  int prompt = (" (lpp-func-is-arg 3) " && " (lpp-func-getarg 3) ");\n"
        "  LSET(ret, LPparse_multiple_exp(Lstream_cval(" (lpp-func-getarg 1) "), (prompt ? Linput_prompt : 0), (print ? Leval_and_print : Leval_no_print)));\n"
        "  if(ret && (ret->Ltype == LTEXCEPTION)) {\n"
        "    LSET(ret, Lnil);\n"
        "    " (lpp-func-return "Lnil")
        "  } else {\n"
        "    LSET(ret, Lnil);\n"
        "    " (lpp-func-return "Lt")
        "  }\n"))

(lpp-func "preprocess" '("preprocess")
  ""
  '("" ("STREAM" () "STREAM" ()))
  '(cat "  if(!Lis_open(Lstream_cval(" (lpp-func-getarg 1) ")))\n"
        "    " (lpp-func-return "Lnil")
        "  if(!" (lpp-func-is-arg 2) ") {\n"
        "    LPparse_lpp_file(Lstream_cval(" (lpp-func-getarg 1) "), Lstdout);\n"
        "    " (lpp-func-return "Lt")
        "  }\n"
        "  if(!Lis_open(Lstream_cval(" (lpp-func-getarg 2) ")))\n"
        "    " (lpp-func-return "Lnil")
        "  LPparse_lpp_file(Lstream_cval(" (lpp-func-getarg 1) "), Lstream_cval(" (lpp-func-getarg 2) "));\n"
        "  " (lpp-func-return "Lt")))
        
(lpp-func "system" '("system")
  ""
  '("" ("STRING" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(system(Lstring_cval(" (lpp-func-getarg 1) ")))"))))

(lpp-func "abort" '("abort" "quit" "exit" "bye")
  "Ends execution immediately without unwinding the execution stack. Returns: DOES NOT RETURN."
  '("" (()))
  '(cat "  exit(1);\n"
        "  " (lpp-func-return "Lnil")))

(lpp-func "doc" '("doc")
  "Returns: The documentation of a function."
  '("" ("NIL" ()))
  '(cat "  if((" (lpp-func-getarg 1) ")->Ltype == LTFUNC) {\n"
        "    " (lpp-func-return (cat "Lfunc_doc(" (lpp-func-getarg 1) ")"))
        "  } else if((" (lpp-func-getarg 1) ")->Ltype == LTLAMBDA) {\n"
        "    " (lpp-func-return (cat "Llambda_doc(" (lpp-func-getarg 1) ")"))
        "  } else if((" (lpp-func-getarg 1) ")->Ltype == LTSYMBOL) {\n"
        "    " (lpp-func-return (cat "Lsymbol_doc(" (lpp-func-getarg 1) ")"))
        "  } else {\n"
        "    " (lpp-func-return "Lnil")
        "  }"))

(lpp-func "catch" '("catch")
  (cat "Catches an exception while evaluating the first argument if it occurs, "
       "binds it to the symbol provided and executes the remaining argument.")
  '("" ("NIL" "SYMBOL" "NIL" ()))
  '(cat "  " (lpp-func-protect-args)
        "  Lobj *ret;\n"
        "  ret = Leval(" (lpp-func-getarg 1) ");\n"
        "  if(ret && (ret->Ltype == LTEXCEPTION)) {\n"
        "    Lcapture(ret);\n"
        "    ret->Ltype = LTDEACTIVATED_EXCEPTION;\n"
        "    Lsymbol_push(" (lpp-func-getarg 2) ", ret);\n"
        "    Lobj *ctchres;\n"
        "    ctchres = Leval(" (lpp-func-getarg 3) ");\n"
        "    Lcapture(ctchres);\n"
        "    Lsymbol_pop(" (lpp-func-getarg 2) ");\n"
        "    Lrelease(ret);\n"
        "    " (lpp-func-unprotect-args)
        "    Lrelease_no_delete(ctchres);\n"
        "    " (lpp-func-return "ctchres")
        "  } else\n"
        "  " (lpp-func-unprotect-args)
        "  " (lpp-func-return "ret")))

(lpp-func "throw" '("throw")
  "Throws (raises) an exception."
  '("" ("STRING" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_exception(" (lpp-func-getarg 1) ", " (lpp-func-getarg 2) ")"))))

(lpp-func "set_prompt" '("set-prompt")
  "Sets the prompt string for user-interaction mode."
  '("" ("STRING" ()))
  '(cat "  " (lpp-func-return (cat "LSET(Linput_prompt_string, " (lpp-func-getarg 1) ")"))))

(lpp-func "get_prompt" '("get-prompt")
  "Returns the prompt string for user-interaction mode."
  '("" (()))
  '(cat "  " (lpp-func-return "Linput_prompt_string")))

(lpp-func "local" '("local")
  "Makes a symbol's value local during the evaluation of the expression."
  '("" ("SYMBOL" "NIL" ()))
  '(cat "  " (lpp-func-protect-args)
        "  Lsymbol_push(" (lpp-func-getarg 1) ", Lnil);\n"
        "  Lobj *ret;\n"
        "  LINIT(ret, Leval(" (lpp-func-getarg 2) "));\n"
        "  Lsymbol_pop(" (lpp-func-getarg 1) ");\n"
        "  Lrelease_no_delete(ret);\n"
        "  " (lpp-func-unprotect-args)
        "  " (lpp-func-return "ret")))

(lpp-func "new_array" '("new-array")
  "Creates a new array of the requested size."
  '("" ("NUMBER" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_array((unsigned int) Lnumber_cval(" (lpp-func-getarg 1) "))"))))

(lpp-func "get_array_index" '("get-array-index")
  "Returns the value stored in the array at the given index."
  '((cat " || (Larray_size(" (lpp-func-getarg 1) ") <= ((unsigned int) Lnumber_cval(" (lpp-func-getarg 2) ")))")
    ("ARRAY" "NUMBER" ()))
  '(cat "  " (lpp-func-return (cat "Larray_vals(" (lpp-func-getarg 1) ")[(unsigned int) Lnumber_cval(" (lpp-func-getarg 2) ")]"))))

(lpp-func "set_array_index" '("set-array-index")
  "Sets the value stored in the array at the given index to the given value and returns it."
  '((cat " || (Larray_size(" (lpp-func-getarg 1) ") <= ((unsigned int) Lnumber_cval(" (lpp-func-getarg 2) ")))")
    ("ARRAY" "NUMBER" "NIL" ()))
  '(cat "  " (lpp-func-return (cat "LSET(Larray_vals(" (lpp-func-getarg 1) ")[(unsigned int) Lnumber_cval(" (lpp-func-getarg 2) ")], " (lpp-func-getarg 3) ")"))))

(lpp-func "array_interval" '("array-interval")
  "Returns an array holding a subinterval of the original array's values."
  '("" ("ARRAY" "NUMBER" "NUMBER" ()))
  '(cat "  " (lpp-func-return (cat "Larray_interval(" (lpp-func-getarg 1) ", (int) Lnumber_cval(" (lpp-func-getarg 2) "), (int) Lnumber_cval(" (lpp-func-getarg 3) "))"))))

(lpp-func "array_cat" '("array-cat")
  "Returns the concatenation of any number of arrays."
  '("" ("ARRAY"))
  '(cat "  " (lpp-func-return (cat "Larray_cat(" (lpp-func-arglist) ")"))))

(lpp-func "array_size" '("array-size")
  "Returns the size of the array."
  '("" ("ARRAY" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_number(Larray_size(" (lpp-func-getarg 1) "))"))))

(lpp-func "list_to_array" '("list-to-array")
  "Converts a list of values to an array."
  '("" ("NIL" ()))
  '(cat "  " (lpp-func-return (cat "Llist_to_array(" (lpp-func-getarg 1) ")"))))

(lpp-func "number_to_char" '("number-to-ascii")
  "Converts a number to a string containing its corresponding ascii character."
  '("" ("NUMBER" ()))
  '(cat "  " (lpp-func-return (cat "Lmake_string_from_char((char) Lnumber_cval(" (lpp-func-getarg 1) "))"))))

(lpp-func "get_env" '("get-env")
  "Gets the value of an environment variable."
  '("" ("STRING" ()))
  '(cat "  char *temp = (char *) getenv(Lstring_cval(" (lpp-func-getarg 1) "));\n"
        "  if(!temp)\n"
		"    " (lpp-func-return "Lnil")
        "  " (lpp-func-return (cat "Lmake_string(temp)"))))

(lpp-func "list_dir" '("list-dir")
  "Gets a list of the contents of a directory."
  '("" ("STRING" ()))
  (? '(equals OSFLAG "WINDOWS")
    '(cat "  WIN32_FIND_DATA FindFileData;\n"
          "  HANDLE hFind = FindFirstFile(Lstring_cval(" (lpp-func-getarg 1) "), &FindFileData);\n"
		  "  if (hFind == INVALID_HANDLE_VALUE)\n" 
		  "    " (lpp-func-return "Lnil") ; TODO: Should this return an error instead?
          "  Lobj *current = Lnil;\n"
          "  Lobj *top = Lnil;\n"
          "  while(1) {\n"
          "    if(current == Lnil)\n"
          "      LSET(top, current = Lmake_cons(Lmake_string(FindFileData.cFileName), Lnil));\n"
          "    else {\n"
          "      LSET(Lcons_cdr(current), Lmake_cons(Lmake_string(FindFileData.cFileName), Lnil));\n"
          "      current = Lcons_cdr(current);\n"
          "    }\n"
		  "    if(!FindNextFile(hFind, &FindFileData))\n"
		  "      break;\n"
          "  }\n"
		  "  FindClose(hFind);\n"
          "  Lrelease_no_delete(top);\n"
          "  " (lpp-func-return "top"))
    '(cat "  DIR *directory = opendir(Lstring_cval(" (lpp-func-getarg 1) "));\n"
          "  struct dirent *entry;\n"
          "  if(!directory)\n"
          "    " (lpp-func-return "Lnil") ;TODO: Should this return an error instead?
          "  Lobj *current = Lnil;\n"
          "  Lobj *top = Lnil;\n"
          "  while(entry = readdir(directory)) {\n"
          "    if(current == Lnil)\n"
          "      LSET(top, current = Lmake_cons(Lmake_string(entry->d_name), Lnil));\n"
          "    else {\n"
          "      LSET(Lcons_cdr(current), Lmake_string(entry->d_name));\n"
          "      current = Lcons_cdr(current);\n"
          "    }\n"
          "  }\n"
          "  Lrelease_no_delete(top);\n"
          "  " (lpp-func-return "top"))))

(lpp-func "is_dir" '("is-dir")
  "Returns whether the path is a directory."
  '("" ("STRING" ()))
  '(cat "  DWORD dwAttrs = GetFileAttributes(Lstring_cval(" (lpp-func-getarg 1) "));\n"
        "  if(dwAttrs != INVALID_FILE_ATTRIBUTES && (dwAttrs & FILE_ATTRIBUTE_DIRECTORY))\n"
		"    " (lpp-func-return "Lt")
		"  " (lpp-func-return "Lnil")))

(lpp-func "is_file" '("is-file")
  "Returns whether the path is a directory."
  '("" ("STRING" ()))
  '(cat "  DWORD dwAttrs = GetFileAttributes(Lstring_cval(" (lpp-func-getarg 1) "));\n"
        "  if(dwAttrs != INVALID_FILE_ATTRIBUTES && !(dwAttrs & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)))\n"
		"    " (lpp-func-return "Lt")
		"  " (lpp-func-return "Lnil")))


(prints "\nSuccessfully loaded lpp_funcs.lsp\n")        




