/*
   lpp_ud_object.h
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

#ifndef LPP_UD_OBJECT_H
#define LPP_UD_OBJECT_H

#include "lpp_obj.h"

/* Data for user-defined objects */
struct LUDobject {
  Lobj *my_class; /* Expects LUDclass */
  Lobj **members;
};

#define LTUDOBJECT_SIZE		(sizeof(Lobj) + sizeof(struct LUDobject))
#define LUDobject_cval(udo) (*((struct LUDobject *) LOBJ_DATA(udo)))
#define LUDobject_class(udo) (LUDobject_cval(udo).my_class)
#define LUDobject_members(udo) (LUDobject_cval(udo).members)

#define LUDobject_num_members(udo) (LUDclass_num_members(LUDobject_class(udo)))


Lobj *Lmake_UDobject(Lobj *udc);
void Ldelete_UDobject(Lobj *udo);
Lobj *LUDobject_to_string(Lobj *udo);
#define LUDobject_eval Lsimple_eval
/* This should be changed once copy constructors are implemented */
#define LUDobject_copy Lno_copy
Lobj *LUDobject_equals(Lobj *udo1, Lobj *udo2);

Lobj *LUDobject_call_method(Lobj *udo, int m, Lobj *args);
Lobj *LUDobject_get_member(Lobj *udo, int m);
Lobj *LUDobject_set_member(Lobj *udo, int m, Lobj *val);

void LUDobject_enter_scope(Lobj *udo);
void LUDobject_exit_scope(Lobj *udo);

#endif /* LPP_UD_OBJECT_H */
