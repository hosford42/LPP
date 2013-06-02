/*
   lpp_string.h
     The LPP string type.
   
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

#ifndef LPP_STRING_H
#define LPP_STRING_H

#include "lpp_obj.h"

#define LTSTRING_SIZE (sizeof(Lobj) + sizeof(char *))

#define Lstring_cval(string) (*((char **) LOBJ_DATA(string)))

Lobj *Lmake_string(const char *strval);
Lobj *Lmake_string_no_copy(char *strval);
Lobj *Lmake_string_from_char(char c);
void Ldelete_string(Lobj *string);
Lobj *Lstring_to_string(Lobj *string);
#define Lstring_eval Lsimple_eval
Lobj *Lstring_copy(Lobj *string);
Lobj *Lstring_equals(Lobj *string1, Lobj *string2);

Lobj *Lstring_cat(Lobj *args);
Lobj *Lstring_substring(Lobj *string, int start, int end);

#endif /* LPP_STRING_H */

