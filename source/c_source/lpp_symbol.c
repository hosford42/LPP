/*
   lpp_symbol.c
     The LPP symbol type.
   
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

#ifndef LPP_SYMBOL_C
#define LPP_SYMBOL_C

#include "lpp_symbol.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_cons.h"

Lobj *Lmake_symbol(Lobj *name, Lobj *doc, Lobj *init_val) {
  Lobj *new_symbol = 0;
  LMalloc(new_symbol, LTSYMBOL_SIZE);
    
  new_symbol->Ltype = LTSYMBOL;
  new_symbol->Lused = 0;
  LINIT(Lsymbol_name(new_symbol), name);
  LINIT(Lsymbol_doc(new_symbol), doc);
  LINIT(Lsymbol_vals(new_symbol), Lmake_cons(init_val, 0));
  
  return new_symbol;
}

void Ldelete_symbol(Lobj *symbol) {
  LSET(Lsymbol_name(symbol), Lnil);
  LSET(Lsymbol_doc(symbol), Lnil);
  LSET(Lsymbol_vals(symbol), Lnil);
  LMfree(symbol);
}

Lobj *Lsymbol_to_string(Lobj *symbol) {
  return Lsymbol_name(symbol);
}

Lobj *Lsymbol_eval(Lobj *symbol) {
  return Lcons_car(Lsymbol_vals(symbol));
}

void Lsymbol_push(Lobj *symbol, Lobj *val) {
  LSET(Lsymbol_vals(symbol), Lmake_cons(val, Lsymbol_vals(symbol)));
}

void Lsymbol_pop(Lobj *symbol) {
  LSET(Lsymbol_vals(symbol), Lcons_cdr(Lsymbol_vals(symbol)));
  if(!Lsymbol_vals(symbol))
    LSET(Lsymbol_vals(symbol), Lmake_cons(Lnil, Lnil));
}

#endif /* LPP_SYMBOL_C */
