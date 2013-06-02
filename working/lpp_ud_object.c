/*
   lpp_ud_object.c
     The LPP user-defined (in contrast to built-in) object type.
   
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

#ifndef LPP_UD_OBJECT_C
#define LPP_UD_OBJECT_C

#include "lpp_ud_object.h"
#include "lpp_ud_class.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_exception.h"

Lobj *Lmake_UDobject(Lobj *udc) {
  Lobj *new_udo = Lnil;
  LMalloc(new_udo, LTUDOBJECT_SIZE);
    
  new_udo->Ltype = LTUDOBJECT;
  new_udo->Lused = 0;
  
  LINIT(LUDobject_class(new_udo), udc);
  LUDobject_members(new_udo) = 0;
  
  LUDclass_init_instance(udc, new_udo);
  
  return new_udo;
}

void Ldelete_UDobject(Lobj *udo) {
  LUDclass_fin_instance(LUDobject_class(udo), udo);
  LSET(LUDobject_class(udo), Lnil);
  LMfree(udo);
}

Lobj *LUDobject_to_string(Lobj *udo) {
  return Lmake_string("\\(object)");
}

Lobj *LUDobject_equals(Lobj *udo1, Lobj *udo2) {
  /* BEGIN HACK */
  return (udo1 == udo2) ? Lt : Lnil;
  /* END HACK */

  Lobj *args;
  LINIT(args, Lmake_list(2, udo1, udo2));
  Lobj *ret;
  LINIT(ret, Lapply(LUDclass_instance_equals(LUDobject_class(udo1)), args));
  Lrelease(args);
  Lrelease_no_delete(ret);
  return ret;
}

Lobj *LUDobject_call_method(Lobj *udo, int m, Lobj *args) {
  if((m >= LUDclass_num_methods(LUDobject_class(udo))) || (m < 0))
    return Lmake_exception(Lmake_string("Method index out of range: "), Lmake_number(m));
  
  Lcapture(udo);
  Lcapture(args);
  LUDobject_enter_scope(udo);
  
  Lobj *ret;
  LINIT(ret, Lapply(LUDclass_methods(LUDobject_class(udo))[m], args));
  
  LUDobject_exit_scope(udo);
  Lrelease(args);
  Lrelease(udo);
  
  Lrelease_no_delete(ret);
  return ret;
}

Lobj *LUDobject_get_member(Lobj *udo, int m) {
  if(!Lequals(Lsymbol_eval(Llookup_symbol("this-class")), LUDobject_class(udo)))
    return Lmake_exception(Lmake_string("Cannot access private members of object: "), udo);

  if((m >= LUDclass_num_members(LUDobject_class(udo))) || (m < 0))
    return Lmake_exception(Lmake_string("Member index out of range: "), Lmake_number(m));
  
  return LUDobject_members(udo)[m];
}

Lobj *LUDobject_set_member(Lobj *udo, int m, Lobj *val) {
  if(!Lequals(Lsymbol_eval(Llookup_symbol("this-class")), LUDobject_class(udo)))
    return Lmake_exception(Lmake_string("Cannot access private members of object: "), udo);

  if((m >= LUDclass_num_members(LUDobject_class(udo))) || (m < 0))
    return Lmake_exception(Lmake_string("Member index out of range: "), Lmake_number(m));
  
  LSET(LUDobject_members(udo)[m], val);
  return val;
}

void LUDobject_enter_scope(Lobj *udo) {
  LUDclass_enter_scope(LUDobject_class(udo));

  int m;
  
  for(m = 0; m < LUDobject_num_members(udo); m++)
    Lsymbol_push(LUDclass_member_names(LUDobject_class(udo))[m], LUDobject_members(udo)[m]);

  Lsymbol_push(Llookup_symbol("this-object"), udo);
}

void LUDobject_exit_scope(Lobj *udo) {
  Lsymbol_pop(Llookup_symbol("this-object"));

  int m;
  
  for(m = 0; m < LUDobject_num_members(udo); m++) {
    LSET(LUDobject_members(udo)[m], Lsymbol_eval(LUDclass_member_names(LUDobject_class(udo))[m]));
    Lsymbol_pop(LUDclass_member_names(LUDobject_class(udo))[m]);
  }

  LUDclass_exit_scope(LUDobject_class(udo));
}

#endif /* LPP_UD_OBJECT_C */
