/*
   lpp_func_defn.c 
     Definitions of the built-in LPP functions. 
   
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



#ifndef LPP_FUNCS_C
#define LPP_FUNCS_C


/* Creates a new class. */
Lobj *LFdefclass(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTCONS, LTCONS);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @defclass: "), args);
  }
  LMfree(errval);
  Lcapture(args);
  Lobj *ret = Lmake_UDclass(Lcons_car(args), Lcons_car(Lcons_cdr(args)));
  Lrelease_no_delete(args);
  return ret;

}

/* Creates a new object of the given class. */
Lobj *LFnew(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTUDCLASS);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @new: "), args);
  }
  LMfree(errval);
  return Lmake_UDobject(Lcons_car(args));

}

/* Calls the specified method of the given object on the remaining arguments. */
Lobj *LFcall_method(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, -1, LTUDOBJECT, LTSYMBOL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @call-method: "), args);
  }
  LMfree(errval);
  Lcapture(args);
  Lobj *ret = LUDobject_call_method(Lcons_car(args), LUDclass_select_method(LUDobject_class(Lcons_car(args)), Lcons_car(Lcons_cdr(args))), Lcons_cdr(Lcons_cdr(args)));
  Lrelease_no_delete(args);
  return ret;

}

/* Returns the value of the selected member of the given object. Exception if not allowed. */
Lobj *LFget_member(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, -1, LTUDOBJECT, LTSYMBOL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @get-member: "), args);
  }
  LMfree(errval);
  return LUDobject_get_member(Lcons_car(args), LUDclass_select_member(LUDobject_class(Lcons_car(args)), Lcons_car(Lcons_cdr(args))));

}

/* Sets the value of the selected member of the given object. Exception if not allowed. */
Lobj *LFset_member(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, -1, LTUDOBJECT, LTSYMBOL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set-member: "), args);
  }
  LMfree(errval);
  return LUDobject_set_member(Lcons_car(args), LUDclass_select_member(LUDobject_class(Lcons_car(args)), Lcons_car(Lcons_cdr(args))), Lcons_car(Lcons_cdr(Lcons_cdr(args))));

}

/* Prints a (hopefully) helpful blurb for new users. */
Lobj *LFhelp(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, 0);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @help: "), args);
  }
  LMfree(errval);
  printf("\n\n\tType \"(show-symbols)\" for a list of symbols and their values.");
  printf("\n	Type \"(doc <symbol>)\" for an explanation of <symbol>.\n\n");
  return Lnil;

}

/* Prints the name and value of each symbol in the symbol table. */
Lobj *LFshow_symbols(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTNIL, LTNIL);
  if(errval->level || !(!(args && Lcons_car(args)) || (Lcons_car(args)->Ltype == LTSTRING)) || !(!(Lcons_cdr(args) && Lcons_car(Lcons_cdr(args))) || (Lcons_car(Lcons_cdr(args))->Ltype == LTSTRING))) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @show-symbols: "), args);
  }
  LMfree(errval);
  Lprinto((args));
  const char *min = (args ? Lcons_car(args) : Lnil) ? Lstring_cval(Lcons_car(args)) : 0;
  const char *max = (Lcons_cdr(args) ? Lcons_car(Lcons_cdr(args)) : Lnil) ? Lstring_cval(Lcons_car(Lcons_cdr(args))) : 0;
  Lprint_table(min, max);
  return Lnil;

}

/* Returns this LPP executable's version number. */
Lobj *LFversion(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, 0);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @version: "), args);
  }
  LMfree(errval);
  return Lmake_string(lpp_version);

}

/* Returns: A new cons object. */
Lobj *LFcons(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTNIL, LTCONS);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @cons: "), args);
  }
  LMfree(errval);
  return Lmake_cons(Lcons_car(args), Lcons_car(Lcons_cdr(args)));

}

/* Returns: The car of the argument. */
Lobj *LFcar(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTCONS);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @car: "), args);
  }
  LMfree(errval);
  if(!Lcons_car(args))
    return Lnil;
  return Lcons_car(Lcons_car(args));

}

/* Returns: The cdr of the argument. */
Lobj *LFcdr(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTCONS);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @cdr: "), args);
  }
  LMfree(errval);
  if(!Lcons_car(args))
    return Lnil;
  return Lcons_cdr(Lcons_car(args));

}

/* Sets the car of argument 1 to argument 2. Returns: Argument 2. */
Lobj *LFsetcar(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTCONS, LTNIL);
  if(errval->level || !Lcons_car(args)) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set-car: "), args);
  }
  LMfree(errval);
  return LSET(Lcons_car(Lcons_car(args)), Lcons_car(Lcons_cdr(args)));

}

/* Sets the cdr of argument 1 to argument 2. Returns: Argument 2. */
Lobj *LFsetcdr(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTCONS, LTNIL);
  if(errval->level|| !Lcons_car(args)) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set-cdr: "), args);
  }
  LMfree(errval);
  return LSET(Lcons_cdr(Lcons_car(args)), Lcons_car(Lcons_cdr(args)));

}

/* Returns: A new list of the arguments. */
Lobj *LFlist(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @list: "), args);
  }
  LMfree(errval);
  if(!(!!args))
    return Lnil;
  return Llist_copy((args));

}

/* Returns: The length of the argument. */
Lobj *LFlist_length(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTCONS);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @list-length: "), args);
  }
  LMfree(errval);
  return Lmake_number(Llist_length(Lcons_car(args)));

}

/* Returns: The sum of the arguments. */
Lobj *LFplus(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @plus: "), args);
  }
  LMfree(errval);
  double sum = 0;
  while((!!args)) {
    sum+= Lnumber_cval(Lcons_car(args));
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  return Lmake_number(sum);

}

/* Returns: The product of the arguments. */
Lobj *LFtimes(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @times: "), args);
  }
  LMfree(errval);
  double product = 1;
  while((!!args)) {
    product*= Lnumber_cval(Lcons_car(args));
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  return Lmake_number(product);

}

/* Returns: The negation of the argument. */
Lobj *LFnegative(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @negative: "), args);
  }
  LMfree(errval);
  return Lmake_number(-Lnumber_cval(Lcons_car(args)));

}

/* Returns: The ratio of the arguments. */
Lobj *LFdivided_by(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTNUMBER, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @divided-by: "), args);
  }
  LMfree(errval);
  if(Lnumber_cval(Lcons_car(Lcons_cdr(args))))
    return Lmake_number(Lnumber_cval(Lcons_car(args)) / Lnumber_cval(Lcons_car(Lcons_cdr(args))));
  else
    return Lmake_exception(Lmake_string("Divide by zero. "), Lnil);

}

/* Returns: If each argument is greater than the previous, t, else nil. */
Lobj *LFgreater_than(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @greater-than: "), args);
  }
  LMfree(errval);
  double ceiling = Lnumber_cval(Lcons_car(args));
  (args = Lcons_cdr(args), Lcons_car(args));
  while((!!args)) {
    if(ceiling <= Lnumber_cval(Lcons_car(args)))
      return Lnil;
    ceiling = Lnumber_cval(Lcons_car(args));
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  return Lt;

}

/* Returns: If each argument is less than the previous, t, else nil. */
Lobj *LFless_than(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @less-than: "), args);
  }
  LMfree(errval);
  double floor = Lnumber_cval(Lcons_car(args));
  (args = Lcons_cdr(args), Lcons_car(args));
  while((!!args)) {
    if(floor >= Lnumber_cval(Lcons_car(args)))
      return Lnil;
    floor = Lnumber_cval(Lcons_car(args));
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  return Lt;

}

/* Returns: The mantissa of a rational number. */
Lobj *LFmantissa(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @mantissa: "), args);
  }
  LMfree(errval);
  return Lmake_number(Lnumber_cval(Lcons_car(args)) - ((long int) Lnumber_cval(Lcons_car(args))));

}

/* Returns: The square root of a non-negative rational number. */
Lobj *LFsquare_root(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @square-root: "), args);
  }
  LMfree(errval);
  if(Lnumber_cval(Lcons_car(args)) >= 0)
  return Lmake_number(sqrt(Lnumber_cval(Lcons_car(args))));
  else
    return Lmake_exception(Lmake_string("Square root of negative number is imaginary. "), Lnil);

}

/* Returns: The absolute value of a rational number. */
Lobj *LFabsolute_value(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @absolute-value: "), args);
  }
  LMfree(errval);
  if(Lnumber_cval(Lcons_car(args)) >= 0)
  return Lmake_number(Lnumber_cval(Lcons_car(args)));
  else
  return Lmake_number(-Lnumber_cval(Lcons_car(args)));

}

/* Returns: The quote's value. */
Lobj *LFquote_value(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTQUOTE);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @quote-value: "), args);
  }
  LMfree(errval);
  return Lquote_cval(Lcons_car(args));

}

/* Returns: A new quote object. */
Lobj *LFquote(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @quote: "), args);
  }
  LMfree(errval);
  return Lmake_quote(Lcons_car(args));

}

/* Prints the concatenation of any number of strings to stdout. Returns: The concatenation of the arguments. */
Lobj *LFprints(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, -1, LTSTRING, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @prints: "), args);
  }
  LMfree(errval);
  Lobj *ret = Lstring_cat((args));
  Lprints(ret);
  return ret;

}

/* Returns: The concatenation of the arguments. */
Lobj *LFcat(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, -1, LTSTRING, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @cat: "), args);
  }
  LMfree(errval);
  return Lstring_cat((args));

}

/* Returns: The substring of argument 1 defined by range [argument 2, argument 3). */
Lobj *LFsubstr(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 3, 3, LTSTRING, LTNUMBER, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @substr: "), args);
  }
  LMfree(errval);
  return Lstring_substring(Lcons_car(args), (int) Lnumber_cval(Lcons_car(Lcons_cdr(args))), (int) Lnumber_cval(Lcons_car(Lcons_cdr(Lcons_cdr(args)))));

}

/* Returns: The length of the longest shared prefix of two strings. */
Lobj *LFstring_shared_prefix(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSTRING, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @string-shared-prefix: "), args);
  }
  LMfree(errval);
  return Lmake_number(Lcharstr_shared_prefix(Lchar_ptr_to_charstr(Lstring_cval(Lcons_car(args))), Lchar_ptr_to_charstr(Lstring_cval(Lcons_car(Lcons_cdr(args))))));

}

/* Returns: Non-nil iff the second string is a prefix of the first. */
Lobj *LFstring_has_prefix(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSTRING, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @string-has-prefix: "), args);
  }
  LMfree(errval);
  return Lcharstr_has_prefix(Lchar_ptr_to_charstr(Lstring_cval(Lcons_car(args))), Lchar_ptr_to_charstr(Lstring_cval(Lcons_car(Lcons_cdr(args))))) ? Lt : Lnil;

}

/* Returns: The location of the earliest occurrence of the second string in the first. */
Lobj *LFstring_find_substring(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSTRING, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @find-substring: "), args);
  }
  LMfree(errval);
  return Lmake_number(Lcharstr_find_substring(Lchar_ptr_to_charstr(Lstring_cval(Lcons_car(args))), Lchar_ptr_to_charstr(Lstring_cval(Lcons_car(Lcons_cdr(args))))));

}

/* Returns: The length of the string. */
Lobj *LFstrlen(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @strlen: "), args);
  }
  LMfree(errval);
  return Lmake_number(strlen(Lstring_cval(Lcons_car(args))));

}

/* Returns: The name of the argument. */
Lobj *LFsymbol_name(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSYMBOL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @symbol-name: "), args);
  }
  LMfree(errval);
  return Lsymbol_name(Lcons_car(args));

}

/* Returns: The effective value of the argument. */
Lobj *LFsymbol_value(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSYMBOL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @value: "), args);
  }
  LMfree(errval);
  return Lcons_car(Lsymbol_vals(Lcons_car(args)));

}

/* Sets the value of argument 1 to argument 2. Returns: Argument 2. */
Lobj *LFsymbol_set(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSYMBOL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set: "), args);
  }
  LMfree(errval);
  return LSET(Lcons_car(Lsymbol_vals(Lcons_car(args))), Lcons_car(Lcons_cdr(args)));

}

/* Returns: A string representation of the argument. */
Lobj *LFto_string(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @to-string: "), args);
  }
  LMfree(errval);
  return Lto_string(Lcons_car(args));

}

/* Evaluates the argument. Returns: The result of the evaluation of the argument. */
Lobj *LFeval(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @eval: "), args);
  }
  LMfree(errval);
  return Leval(Lcons_car(args));

}

/* Returns: The type of the argument. */
Lobj *LFtype_of(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @type-of: "), args);
  }
  LMfree(errval);
  return Lcons_car(args) ? Ltypes[(Lcons_car(args))->Ltype].token : Ltypes[LTCONS].token;

}

/* Returns: A copy of the argument if one can be made, otherwise the argument. */
Lobj *LFcopy(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @copy: "), args);
  }
  LMfree(errval);
  return Lcopy(Lcons_car(args));

}

/* Returns: If the arguments are equal, t, else nil. */
Lobj *LFequals(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTNIL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @equals: "), args);
  }
  LMfree(errval);
  return Lequals(Lcons_car(args), Lcons_car(Lcons_cdr(args)));

}

/* Returns: If the objects are identical, t, else nil. */
Lobj *LFidentical(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTNIL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @identical: "), args);
  }
  LMfree(errval);
  return (Lcons_car(args) == Lcons_car(Lcons_cdr(args))) ? Lt : Lnil;

}

/* Sets the value of a symbol's doc string. */
Lobj *LFset_symbol_doc(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSYMBOL, LTNIL);
  if(errval->level || (Lcons_car(Lcons_cdr(args)) && (Lcons_car(Lcons_cdr(args))->Ltype != LTSTRING))) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set-symbol-doc: "), args);
  }
  LMfree(errval);
  LSET(Lsymbol_doc(Lcons_car(args)), Lcons_car(Lcons_cdr(args)));
  return Lcons_car(Lcons_cdr(args));

}

/* Creates a new function object. Returns: The new function object. */
Lobj *LFdefun(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 3, 3, LTCONS, LTNIL, LTNIL);
  if(errval->level || !Lcons_car(args) || (Lcons_car(Lcons_cdr(args)) && (Lcons_car(Lcons_cdr(args))->Ltype != LTSTRING))) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @defun: "), args);
  }
  LMfree(errval);
  Lobj *arglist = Lcons_car(args);
  errval = Lcheck_args(arglist, 1, -1, LTNIL, LTNIL);
  if(errval->level) {
    /* ERROR */
    return Lmake_exception(Lmake_string("Bad argument list in function definition: "), (args));
  }
  while(arglist) {
    if(Lcons_car(arglist) && (Lcons_car(arglist)->Ltype != LTSYMBOL)) {
      /* ERROR */
      return Lmake_exception(Lmake_string("Bad argument list in function definition: "), (args));
    }
    arglist = Lcons_cdr(arglist);
  }
  return Lmake_lambda(Lcons_car(args), Lcons_car(Lcons_cdr(args)), Lcons_car(Lcons_cdr(Lcons_cdr(args))));

}

/* If the value of argument 1 is non-nil, evaluates argument 2, otherwise evaluates argument 3. Returns: The result of whichever argument gets evaluated. */
Lobj *LFif(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 3, LTNIL, LTNIL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @if: "), args);
  }
  LMfree(errval);
  Lcapture(args);
  Lobj *ret;
  if(Lcons_car(args))
    ret = Leval(Lcons_car(Lcons_cdr(args)));
  else
    ret = Lcons_cdr(Lcons_cdr(args)) ?
            Leval(Lcons_car(Lcons_cdr(Lcons_cdr(args)))) :
            Lnil;
  Lrelease_no_delete(args);
  return ret;

}

/*  */
Lobj *LFdo(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @do: "), args);
  }
  LMfree(errval);
  Lobj *ret = Lnil;
  while((!!args)) {
    Lcapture(args);
    LSET(ret, Leval(Lcons_car(args)));
    Lrelease_no_delete(args);
    if(ret && ret->Ltype == LTEXCEPTION) {
  	   Lrelease_no_delete(ret);
      return ret;
    }
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  Lrelease_no_delete(ret);
  return ret;

}

/*  */
Lobj *LFdo1(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @do1: "), args);
  }
  LMfree(errval);
  if(!(!!args))
    return Lnil;
  Lobj *trash = Lnil;
  Lobj *ret = Lnil;
  Lcapture(args);
  LSET(ret, Leval(Lcons_car(args)));
  Lrelease_no_delete(args);
  (args = Lcons_cdr(args), Lcons_car(args));
  while((!!args)) {
    Lcapture(args);
    LSET(trash, Leval(Lcons_car(args)));
    Lrelease_no_delete(args);
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  LSET(trash, Lnil);
  Lrelease_no_delete(ret);
  return ret;

}

/*  */
Lobj *LFwhile(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTNIL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @while: "), args);
  }
  LMfree(errval);
  Lcapture(args);
  Lobj *test = Lnil;
  Lobj *ret = Lnil;
  unsigned long int count = 0;
  while(LSET(test, Leval(Lcons_car(args)))) {
    LSET(ret, Leval(Lcons_car(Lcons_cdr(args))));
    if(++count == 0) {
      return Lmake_exception(Lmake_string("Infinite @while loop: "), (args));
    }
  }
  Lrelease(test);
  Lrelease_no_delete(args);
  Lrelease_no_delete(ret);
  return ret;

}

/*  */
Lobj *LFnot(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @not: "), args);
  }
  LMfree(errval);
  if(Lcons_car(args))
    return Lnil;
  else
    return Lt;

}

/*  */
Lobj *LFand(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @and: "), args);
  }
  LMfree(errval);
  Lobj *ret = Lnil;
  while((!!args)) {
    Lcapture(args);
    if(!LSET(ret, Leval(Lcons_car(args)))) {
      Lrelease_no_delete(args);
      return Lnil;
    }
    Lrelease_no_delete(args);
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  Lrelease_no_delete(ret);
  return ret;

}

/*  */
Lobj *LFor(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @or: "), args);
  }
  LMfree(errval);
  Lobj *ret = Lnil;
  while((!!args)) {
    Lcapture(args);
    if(LSET(ret, Leval(Lcons_car(args)))) {
      Lrelease_no_delete(ret);
      Lrelease_no_delete(args);
      return ret;
    }
    Lrelease_no_delete(args);
    (args = Lcons_cdr(args), Lcons_car(args));
  }
  return Lnil;

}

/*  */
Lobj *LFfopen(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSTRING, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @fopen: "), args);
  }
  LMfree(errval);
  return Lmake_file_stream(Lstring_cval(Lcons_car(args)), Lstring_cval(Lcons_car(Lcons_cdr(args))));

}

/*  */
Lobj *LFis_open(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTREAM);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @is-open: "), args);
  }
  LMfree(errval);
  return (Lstream_cval(Lcons_car(args)) && Lis_open(Lstream_cval(Lcons_car(args)))) ? Lt : Lnil;

}

/*  */
Lobj *LFclose(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTREAM);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @close: "), args);
  }
  LMfree(errval);
  if(Lstream_cval(Lcons_car(args)) && !Lclose(Lstream_cval(Lcons_car(args))))
    return Lt;
  else
    return Lnil;

}

/*  */
Lobj *LFgetc(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTREAM);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @getc: "), args);
  }
  LMfree(errval);
  if(Lstream_cval(Lcons_car(args))) {
    char c[2] = " ";
    int i = Lgetc(Lstream_cval(Lcons_car(args)));
    if(i == EOF)
      return Lnil;
    c[0] = i;
    return Lmake_string(c);
  } else
    return Lnil;

}

/*  */
Lobj *LFload(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 3, LTSTREAM, LTNIL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @load-file: "), args);
  }
  LMfree(errval);
  if(!Lis_open(Lstream_cval(Lcons_car(args))))
    return Lnil;
  Lobj *ret = Lnil;
  int print = (Lcons_cdr(args) && Lcons_car(Lcons_cdr(args)));
  int prompt = (Lcons_cdr(Lcons_cdr(args)) && Lcons_car(Lcons_cdr(Lcons_cdr(args))));
  LSET(ret, LPparse_multiple_exp(Lstream_cval(Lcons_car(args)), (prompt ? Linput_prompt : 0), (print ? Leval_and_print : Leval_no_print)));
  if(ret && (ret->Ltype == LTEXCEPTION)) {
    LSET(ret, Lnil);
    return Lnil;
  } else {
    LSET(ret, Lnil);
    return Lt;
  }

}

/*  */
Lobj *LFpreprocess(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 2, LTSTREAM, LTSTREAM);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @preprocess: "), args);
  }
  LMfree(errval);
  if(!Lis_open(Lstream_cval(Lcons_car(args))))
    return Lnil;
  if(!Lcons_cdr(args)) {
    LPparse_lpp_file(Lstream_cval(Lcons_car(args)), Lstdout);
    return Lt;
  }
  if(!Lis_open(Lstream_cval(Lcons_car(Lcons_cdr(args)))))
    return Lnil;
  LPparse_lpp_file(Lstream_cval(Lcons_car(args)), Lstream_cval(Lcons_car(Lcons_cdr(args))));
  return Lt;

}

/*  */
Lobj *LFsystem(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @system: "), args);
  }
  LMfree(errval);
  return Lmake_number(system(Lstring_cval(Lcons_car(args))));

}

/* Ends execution immediately without unwinding the execution stack. Returns: DOES NOT RETURN. */
Lobj *LFabort(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, 0);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @abort: "), args);
  }
  LMfree(errval);
  exit(1);
  return Lnil;

}

/* Returns: The documentation of a function. */
Lobj *LFdoc(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @doc: "), args);
  }
  LMfree(errval);
  if((Lcons_car(args))->Ltype == LTFUNC) {
    return Lfunc_doc(Lcons_car(args));
  } else if((Lcons_car(args))->Ltype == LTLAMBDA) {
    return Llambda_doc(Lcons_car(args));
  } else if((Lcons_car(args))->Ltype == LTSYMBOL) {
    return Lsymbol_doc(Lcons_car(args));
  } else {
    return Lnil;
  }
}

/* Catches an exception while evaluating the first argument if it occurs, binds it to the symbol provided and executes the remaining argument. */
Lobj *LFcatch(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 3, 3, LTNIL, LTSYMBOL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @catch: "), args);
  }
  LMfree(errval);
  Lcapture(args);
  Lobj *ret;
  ret = Leval(Lcons_car(args));
  if(ret && (ret->Ltype == LTEXCEPTION)) {
    Lcapture(ret);
    ret->Ltype = LTDEACTIVATED_EXCEPTION;
    Lsymbol_push(Lcons_car(Lcons_cdr(args)), ret);
    Lobj *ctchres;
    ctchres = Leval(Lcons_car(Lcons_cdr(Lcons_cdr(args))));
    Lcapture(ctchres);
    Lsymbol_pop(Lcons_car(Lcons_cdr(args)));
    Lrelease(ret);
    Lrelease_no_delete(args);
    Lrelease_no_delete(ctchres);
    return ctchres;
  } else
  Lrelease_no_delete(args);
  return ret;

}

/* Throws (raises) an exception. */
Lobj *LFthrow(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSTRING, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @throw: "), args);
  }
  LMfree(errval);
  return Lmake_exception(Lcons_car(args), Lcons_car(Lcons_cdr(args)));

}

/* Sets the prompt string for user-interaction mode. */
Lobj *LFset_prompt(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set-prompt: "), args);
  }
  LMfree(errval);
  return LSET(Linput_prompt_string, Lcons_car(args));

}

/* Returns the prompt string for user-interaction mode. */
Lobj *LFget_prompt(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, 0);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @get-prompt: "), args);
  }
  LMfree(errval);
  return Linput_prompt_string;

}

/* Makes a symbol's value local during the evaluation of the expression. */
Lobj *LFlocal(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTSYMBOL, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @local: "), args);
  }
  LMfree(errval);
  Lcapture(args);
  Lsymbol_push(Lcons_car(args), Lnil);
  Lobj *ret;
  LINIT(ret, Leval(Lcons_car(Lcons_cdr(args))));
  Lsymbol_pop(Lcons_car(args));
  Lrelease_no_delete(ret);
  Lrelease_no_delete(args);
  return ret;

}

/* Creates a new array of the requested size. */
Lobj *LFnew_array(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @new-array: "), args);
  }
  LMfree(errval);
  return Lmake_array((unsigned int) Lnumber_cval(Lcons_car(args)));

}

/* Returns the value stored in the array at the given index. */
Lobj *LFget_array_index(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 2, 2, LTARRAY, LTNUMBER);
  if(errval->level || (Larray_size(Lcons_car(args)) <= ((unsigned int) Lnumber_cval(Lcons_car(Lcons_cdr(args)))))) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @get-array-index: "), args);
  }
  LMfree(errval);
  return Larray_vals(Lcons_car(args))[(unsigned int) Lnumber_cval(Lcons_car(Lcons_cdr(args)))];

}

/* Sets the value stored in the array at the given index to the given value and returns it. */
Lobj *LFset_array_index(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 3, 3, LTARRAY, LTNUMBER, LTNIL);
  if(errval->level || (Larray_size(Lcons_car(args)) <= ((unsigned int) Lnumber_cval(Lcons_car(Lcons_cdr(args)))))) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @set-array-index: "), args);
  }
  LMfree(errval);
  return LSET(Larray_vals(Lcons_car(args))[(unsigned int) Lnumber_cval(Lcons_car(Lcons_cdr(args)))], Lcons_car(Lcons_cdr(Lcons_cdr(args))));

}

/* Returns an array holding a subinterval of the original array's values. */
Lobj *LFarray_interval(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 3, 3, LTARRAY, LTNUMBER, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @array-interval: "), args);
  }
  LMfree(errval);
  return Larray_interval(Lcons_car(args), (int) Lnumber_cval(Lcons_car(Lcons_cdr(args))), (int) Lnumber_cval(Lcons_car(Lcons_cdr(Lcons_cdr(args)))));

}

/* Returns the concatenation of any number of arrays. */
Lobj *LFarray_cat(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 0, -1, LTARRAY);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @array-cat: "), args);
  }
  LMfree(errval);
  return Larray_cat((args));

}

/* Returns the size of the array. */
Lobj *LFarray_size(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTARRAY);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @array-size: "), args);
  }
  LMfree(errval);
  return Lmake_number(Larray_size(Lcons_car(args)));

}

/* Converts a list of values to an array. */
Lobj *LFlist_to_array(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNIL);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @list-to-array: "), args);
  }
  LMfree(errval);
  return Llist_to_array(Lcons_car(args));

}

/* Converts a number to a string containing its corresponding ascii character. */
Lobj *LFnumber_to_char(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTNUMBER);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @number-to-ascii: "), args);
  }
  LMfree(errval);
  return Lmake_string_from_char((char) Lnumber_cval(Lcons_car(args)));

}

/* Gets the value of an environment variable. */
Lobj *LFget_env(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @get-env: "), args);
  }
  LMfree(errval);
  char *temp = (char *) getenv(Lstring_cval(Lcons_car(args)));
  if(!temp)
    return Lnil;
  return Lmake_string(temp);

}

/* Gets a list of the contents of a directory. */
Lobj *LFlist_dir(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @list-dir: "), args);
  }
  LMfree(errval);
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind = FindFirstFile(Lstring_cval(Lcons_car(args)), &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
    return Lnil;
  Lobj *current = Lnil;
  Lobj *top = Lnil;
  while(1) {
    if(current == Lnil)
      LSET(top, current = Lmake_cons(Lmake_string(FindFileData.cFileName), Lnil));
    else {
      LSET(Lcons_cdr(current), Lmake_cons(Lmake_string(FindFileData.cFileName), Lnil));
      current = Lcons_cdr(current);
    }
    if(!FindNextFile(hFind, &FindFileData))
      break;
  }
  FindClose(hFind);
  Lrelease_no_delete(top);
  return top;

}

/* Returns whether the path is a directory. */
Lobj *LFis_dir(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @is-dir: "), args);
  }
  LMfree(errval);
  DWORD dwAttrs = GetFileAttributes(Lstring_cval(Lcons_car(args)));
  if(dwAttrs != INVALID_FILE_ATTRIBUTES && (dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
    return Lt;
  return Lnil;

}

/* Returns whether the path is a directory. */
Lobj *LFis_file(Lobj *args) {
  Lerror *errval = Lcheck_args(args, 1, 1, LTSTRING);
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string("Bad argument list passed to @is-file: "), args);
  }
  LMfree(errval);
  DWORD dwAttrs = GetFileAttributes(Lstring_cval(Lcons_car(args)));
  if(dwAttrs != INVALID_FILE_ATTRIBUTES && !(dwAttrs & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)))
    return Lt;
  return Lnil;

}


#endif /* LPP_FUNCS_C */
