/*
   lpp_exception.h
     The LPP exception type.
   
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

#ifndef LPP_EXCEPTION_H
#define LPP_EXCEPTION_H

#include "lpp_obj.h"

/* Data for exception objects */
struct Lexception {
  Lobj *description; /* Expects LTSTRING */
  Lobj *data;
};

#define LTEXCEPTION_SIZE		(sizeof(Lobj) + sizeof(struct Lexception))
#define Lexception_cval(exc) (*((struct Lexception *) LOBJ_DATA(exc)))
#define Lexception_description(exc) (Lexception_cval(exc).description)
#define Lexception_data(exc) (Lexception_cval(exc).data)

Lobj *Lmake_exception(Lobj *desc, Lobj *data);
void Ldelete_exception(Lobj *exc);
Lobj *Lexception_to_string(Lobj *exc);
#define Lexception_eval Lsimple_eval
#define Lexception_copy Lno_copy
#define Lexception_equals Lsimple_equals

Lobj *Lsign_exception(Lobj *exc, const char *signature);
void Lexception_to_string_helper(int *buff_size, int *index, char **buff, Lobj *exc);

#define Lpass_exc(exp, signature) \
  (Lobj *temp = (exp),\
   if(temp && (temp->Ltype == LTEXCEPTION))\
     return Lsign_exception(temp, "In " #signature),\
   temp)
  

#endif /* LPP_EXCEPTION_H */
