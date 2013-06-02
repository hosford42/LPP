/*
   lpp_type_token.c
     The LPP type meta-type, for LPP built-in type reflection.
   
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

#ifndef LPP_TYPE_TOKEN_C
#define LPP_TYPE_TOKEN_C

#include "lpp_type_token.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_table.h"

Lobj *Lmake_type_token(char *name) {
  Lobj *new_tt = 0;
  LMalloc(new_tt, LTTYPE_TOKEN_SIZE);
    
  new_tt->Ltype = LTTYPE_TOKEN;
  new_tt->Lused = -1;/* Never delete */
  
  LINIT(Ltype_token_name(new_tt), Lmake_string(name));
  
  /* Automatically register the type token in the symbol table */
  Lobj *symbol = Llookup_symbol(name + 1);
  Lsymbol_push(symbol, new_tt);
  
  return new_tt;
}

void Ldelete_type_token(Lobj *tt) {
  LSET(Ltype_token_name(tt), Lnil);
  LMfree(tt);
}

Lobj *Ltype_token_to_string(Lobj *tt) {
  return Ltype_token_name(tt);
}

#endif /* LPP_TYPE_TOKEN_C */
