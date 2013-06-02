/*
   lpp_func.h
     The LPP function type, for built-in functions.
   
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

#ifndef LPP_FUNC_H
#define LPP_FUNC_H

#include "lpp_obj.h"
#include "lpp_ops.h"

typedef Lobj *(*Lfunction)(Lobj *);/* Expects LTCONS or NIL argument */

struct Lfunc {
  Lobj *name; /* Expects LTSTRING */
  Lobj *doc; /* Expects LTSTRING */
  Lfunction func;
};

#define LTFUNC_SIZE (sizeof(Lobj) + sizeof(struct Lfunc))

#define Lfunc_cval(fn) (*((struct Lfunc *) LOBJ_DATA(fn)))
#define Lfunc_name(fn) (Lfunc_cval(fn).name)
#define Lfunc_doc(fn) (Lfunc_cval(fn).doc)
#define Lfunc_func(fn) (Lfunc_cval(fn).func)

Lobj *Lmake_func(const char *name, const char *doc, Lfunction f);
void Ldelete_func(Lobj *func);
Lobj *Lfunc_to_string(Lobj *func);
#define Lfunc_eval Lsimple_eval
#define Lfunc_copy Lno_copy
#define Lfunc_equals Lsimple_equals

#endif /* LPP_FUNC_H */

