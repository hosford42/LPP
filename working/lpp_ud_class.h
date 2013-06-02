/*
   lpp_ud_class.h
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

#ifndef LPP_UD_CLASS_H
#define LPP_UD_CLASS_H

#include "lpp_obj.h"

/* Data for user-defined classes */
struct LUDclass {
  int num_members;
  Lobj **members;
  Lobj **member_names;
  
  int num_methods;
  Lobj **methods;
  Lobj **method_names;
};

#define LTUDCLASS_SIZE (sizeof(Lobj) + sizeof(struct LUDclass))
#define LUDclass_cval(udc) (*((struct LUDclass *) LOBJ_DATA(udc)))

#define LUDclass_num_members(udc) (LUDclass_cval(udc).num_members)
#define LUDclass_members(udc) (LUDclass_cval(udc).members)
#define LUDclass_member_names(udc) (LUDclass_cval(udc).member_names)

#define LUDclass_num_methods(udc) (LUDclass_cval(udc).num_methods)
#define LUDclass_methods(udc) (LUDclass_cval(udc).methods)
#define LUDclass_method_names(udc) (LUDclass_cval(udc).method_names)

#define LUDclass_instance_to_string(udc) (LUDclass_methods(udc)[LTO_STRING])
#define LUDclass_instance_eval(udc) (LUDclass_methods(udc)[LEVAL])
#define LUDclass_instance_equals(udc) (LUDclass_methods(udc)[LNUMBER_OF_GENERIC_1METHODS + LEQUALS])


Lobj *Lmake_UDclass(Lobj *members, Lobj *methods);
void Ldelete_UDclass(Lobj *udc);
Lobj *LUDclass_to_string(Lobj *udc);
#define LUDclass_eval Lsimple_eval
#define LUDclass_copy Lno_copy
#define LUDclass_equals Lsimple_equals

Lobj *Lbuild_UDclass(int num_members, Lobj **members, Lobj **member_names, int num_methods, Lobj **methods, Lobj **method_names);

void LUDclass_init_instance(Lobj *udc, Lobj *udo);
void LUDclass_fin_instance(Lobj *udc, Lobj *udo);

int LUDclass_select_method(Lobj *udc, Lobj *sym);
int LUDclass_select_member(Lobj *udc, Lobj *sym);

void LUDclass_enter_scope(Lobj *udc);
void LUDclass_exit_scope(Lobj *udc);

#endif /* LPP_UD_CLASS_H */
