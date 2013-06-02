/*
   lpp_symbol.h
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

#ifndef LPP_SYMBOL_H
#define LPP_SYMBOL_H

#include "lpp_obj.h"

/* Data for symbol objects */
struct Lsymbol {
  Lobj *name; /* expects LTSTRING */
  Lobj *doc; /* expects LTSTRING */
  Lobj *vals; /* expects LTCONS */
};

#define LTSYMBOL_SIZE		(sizeof(Lobj) + sizeof(struct Lsymbol))
#define Lsymbol_cval(symbol)	(*((struct Lsymbol *) LOBJ_DATA(symbol)))
#define Lsymbol_name(symbol)	(Lsymbol_cval(symbol).name)
#define Lsymbol_doc(symbol) (Lsymbol_cval(symbol).doc)
#define Lsymbol_vals(symbol)	(Lsymbol_cval(symbol).vals)

Lobj *Lmake_symbol(Lobj *name, Lobj *doc, Lobj *init_val);
void Ldelete_symbol(Lobj *symbol);
Lobj *Lsymbol_to_string(Lobj *symbol);
Lobj *Lsymbol_eval(Lobj *symbol);
#define Lsymbol_copy Lno_copy
#define Lsymbol_equals Lsimple_equals

void Lsymbol_push(Lobj *symbol, Lobj *val);
void Lsymbol_pop(Lobj *symbol);

#endif /* LPP_SYMBOL_H */
