/*
   lpp_func.c
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

#ifndef LPP_FUNC_C
#define LPP_FUNC_C

#include "lpp_func.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_string.h"

Lobj *Lmake_func(const char *name, const char *doc, Lfunction f) {
  Lobj *new_func = 0;
  LMalloc(new_func, LTFUNC_SIZE);
    
  new_func->Ltype = LTFUNC;
  new_func->Lused = -1;
  
  LINIT(Lfunc_name(new_func), Lmake_string(name));
  LINIT(Lfunc_doc(new_func), Lmake_string(doc));
  Lfunc_func(new_func) = f;
  
  return new_func;
}

void Ldelete_func(Lobj *func) {
  LSET(Lfunc_name(func), Lnil);
  LSET(Lfunc_doc(func), Lnil);
  LMfree(func);
}

Lobj *Lfunc_to_string(Lobj *func) {
  return Lfunc_name(func);
}

#endif /* LPP_FUNC_C */
