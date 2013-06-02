/*
   lpp_type_token.h
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

#ifndef LPP_TYPE_TOKEN_H
#define LPP_TYPE_TOKEN_H

#include "lpp_obj.h"

/* Data for type-token objects */
struct Ltype_token {
  Lobj *name;
};

#define LTTYPE_TOKEN_SIZE (sizeof(Lobj) + sizeof(struct Ltype_token))
#define Ltype_token_cval(tt) (*((struct Ltype_token *) LOBJ_DATA(tt)))
#define Ltype_token_name(tt) (Ltype_token_cval(tt).name)

Lobj *Lmake_type_token(char *name);
void Ldelete_type_token(Lobj *tt);
Lobj *Ltype_token_to_string(Lobj *tt);
#define Ltype_token_eval Lsimple_eval
#define Ltype_token_copy Lno_copy
#define Ltype_token_equals Lsimple_equals

#endif /* LPP_TYPE_TOKEN_H */
