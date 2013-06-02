/*
   lpp_ud_class.c
     The LPP user-defined (in contrast to built-in) class type.
   
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

#ifndef LPP_UD_CLASS_C
#define LPP_UD_CLASS_C

#include "lpp_ud_object.h"
#include "lpp_ud_class.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_exception.h"
#include "lpp_cons.h"
#include "lpp_string.h"
#include "lpp_number.h"
#include "lpp_error.h"

Lobj *Lmake_UDclass(Lobj *members, Lobj *methods) {
  Lerror *errval = Lcheck_args(members, 0, -1, LTNIL, LTNIL); /* Just make sure it's a well-formed list */
  if(errval->level) {
    /* ERROR */
    Lobj *result = Lmake_exception(Lmake_string("Ill-formed member list: "), Lmake_list(2, Lmake_list(2, Lmake_string("location"), Lmake_number(errval->level)), Lmake_list(2, Lmake_string("list"), members)));
	LMfree(errval);
	return result;
  }
  LMfree(errval);

  errval = Lcheck_args(methods, 0, -1, LTNIL, LTNIL); /* Just make sure it's a well-formed list */
  if(errval->level) {
    /* ERROR */
	Lobj *result = Lmake_exception(Lmake_string("Ill-formed method list: "), Lmake_list(2, Lmake_list(2, Lmake_string("location"), Lmake_number(errval->level)), Lmake_list(2, Lmake_string("list"), methods)));
	LMfree(errval);
	return result;
  }
  LMfree(errval);

  int num_members = Llist_length(members);
  Lobj **member_array = 0;
  Lobj **member_name_array = 0;
  
  if(num_members) {
    LMalloc(member_array, num_members * sizeof(Lobj *));
    LMalloc(member_name_array, num_members * sizeof(Lobj *));

    int current_member;   
    for(current_member = 0; current_member < num_members; current_member++) {
      Lobj *this_member = Lcons_car(members);
      errval = Lcheck_args(this_member, 2, 2, LTSYMBOL, LTNIL); /* Make sure it's a well-formed pair */
      if(errval->level) {
        /* ERROR */
        int m;
        for(m = 0; m < current_member; m++) {
          LSET(member_name_array[m], Lnil);
          LSET(member_array[m], Lnil);
        }
        LMfree(member_array);
        LMfree(member_name_array);
        
        Lobj *result = Lmake_exception(Lmake_string("Ill-formed member list: "), Lmake_list(2, Lmake_list(2, Lmake_string("location"), Lmake_number(errval->level)), Lmake_list(2, Lmake_string("list"), this_member)));
		LMfree(errval);
		return result;
      }
	  LMfree(errval);
      
      LINIT(member_name_array[current_member], Lcons_car(this_member));
      LINIT(member_array[current_member], Lcons_car(Lcons_cdr(this_member)));
      
      members = Lcons_cdr(members);
    }
  }

  int num_methods = Llist_length(methods);
  Lobj **method_array = 0;
  Lobj **method_name_array = 0;
  
  if(num_methods) {
    LMalloc(method_array, num_methods * sizeof(Lobj *));
    LMalloc(method_name_array, num_methods * sizeof(Lobj *));

    int current_method;   
    for(current_method = 0; current_method < num_methods; current_method++) {
      Lobj *this_method = Lcons_car(methods);
      errval = Lcheck_args(this_method, 2, 2, LTSYMBOL, LTNIL); /* Make sure it's a well-formed pair */
      if(errval->level) {
        /* ERROR */
        
        int m;
        for(m = 0; m < num_members; m++) {
          LSET(member_name_array[m], Lnil);
          LSET(member_array[m], Lnil);
        }
        LMfree(member_array);
        LMfree(member_name_array);

        for(m = 0; m < current_method; m++) {
          LSET(method_name_array[m], Lnil);
          LSET(method_array[m], Lnil);
        }
        LMfree(method_array);
        LMfree(method_name_array);
        
        Lobj *result = Lmake_exception(Lmake_string("Ill-formed method list: "), Lmake_list(2, Lmake_list(2, Lmake_string("location"), Lmake_number(errval->level)), Lmake_list(2, Lmake_string("list"), this_method)));
		LMfree(errval);
		return result;
      }
	  LMfree(errval);
      
      LINIT(method_name_array[current_method], Lcons_car(this_method));
      LINIT(method_array[current_method], Lcons_car(Lcons_cdr(this_method)));
      
      methods = Lcons_cdr(methods);
    }
  }

  return Lbuild_UDclass(num_members, member_array, member_name_array, num_methods, method_array, method_name_array);
}

void Ldelete_UDclass(Lobj *udc) {
  int m;

  for(m = 0; m < LUDclass_num_members(udc); m++) {
    LSET(LUDclass_members(udc)[m], Lnil);
    LSET(LUDclass_member_names(udc)[m], Lnil);
  }
  LMfree(LUDclass_members(udc));
  LMfree(LUDclass_member_names(udc));
  
  for(m = 0; m < LUDclass_num_methods(udc); m++) {
    LSET(LUDclass_methods(udc)[m], Lnil);
    LSET(LUDclass_method_names(udc)[m], Lnil);
  }
  LMfree(LUDclass_methods(udc));
  LMfree(LUDclass_method_names(udc));
  
  LMfree(udc);
}

Lobj *LUDclass_to_string(Lobj *udo) {
  return Lmake_string("\\(class)");
}

Lobj *Lbuild_UDclass(int num_members, Lobj **members, Lobj **member_names, int num_methods, Lobj **methods, Lobj **method_names) {
  Lobj *new_udc = Lnil;
  LMalloc(new_udc, LTUDCLASS_SIZE);
  
  new_udc->Ltype = LTUDCLASS;
  new_udc->Lused = 0;

  LUDclass_num_members(new_udc) = num_members;
  LUDclass_members(new_udc) = members;
  LUDclass_member_names(new_udc) = member_names;
  
  LUDclass_num_methods(new_udc) = num_methods;
  LUDclass_methods(new_udc) = methods;
  LUDclass_method_names(new_udc) = method_names;  
  
  return new_udc;
}

void LUDclass_init_instance(Lobj *udc, Lobj *udo) {
  if(LUDclass_num_members(LUDobject_class(udo))) {
    LMalloc(LUDobject_members(udo), LUDclass_num_members(udc) * sizeof(Lobj *));
    int m;
    for(m = 0; m < LUDclass_num_members(udc); m++)
      LINIT(LUDobject_members(udo)[m], Lcopy(LUDclass_members(udc)[m]));
  }
}

void LUDclass_fin_instance(Lobj *udc, Lobj *udo) {
  if(LUDclass_num_members(udc)) {
    int m;
    for(m = 0; m < LUDclass_num_members(udc); m++)
      LSET(LUDobject_members(udo)[m], Lnil);
    LMfree(LUDobject_members(udo));
  }
}

int LUDclass_select_method(Lobj *udc, Lobj *sym) {
  int m;
  for(m = 0; m < LUDclass_num_methods(udc); m++)
    if(Lequals(Lsymbol_name(LUDclass_method_names(udc)[m]), Lsymbol_name(sym)))
      return m;
  
  return -1; /* Index that's always out of range. */
}

int LUDclass_select_member(Lobj *udc, Lobj *sym) {
  int m;
  for(m = 0; m < LUDclass_num_members(udc); m++)
    if(Lequals(Lsymbol_name(LUDclass_member_names(udc)[m]), Lsymbol_name(sym)))
      return m;
  
  return -1; /* Index that's always out of range. */
}

void LUDclass_enter_scope(Lobj *udc) {
  int m;
  
  for(m = 0; m < LUDclass_num_methods(udc); m++)
    Lsymbol_push(LUDclass_method_names(udc)[m], LUDclass_methods(udc)[m]);
  
  Lsymbol_push(Llookup_symbol("this-class"), udc);
}

void LUDclass_exit_scope(Lobj *udc) {
  Lsymbol_pop(Llookup_symbol("this-class"));

  int m;

  for(m = 0; m < LUDclass_num_methods(udc); m++)
    Lsymbol_pop(LUDclass_method_names(udc)[m]);
}

#endif /* LPP_UD_CLASS_C */
