/*
   lpp_charstr_util.h
     The LPP charstr struct, used to implement the LPP string type.
   
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

#ifndef LPP_CHARSTR_UTIL_H
#define LPP_CHARSTR_UTIL_H

#include <string.h>


struct charstr {
  unsigned int length;
  char *value;
};

typedef struct charstr charstr;


/* Returns the length of the shared prefix of s1 and s2. */
unsigned int Lcharstr_shared_prefix(charstr s1, charstr s2);

/* Returns non-zero iff sub is a prefix of super */
int Lcharstr_has_prefix(charstr super, charstr sub);

/* Finds the starting index of the first occurrence of the substring,
   or length of the superstring if no match is found. */
unsigned int Lcharstr_find_substring(charstr super, charstr sub);

#define Lchar_ptr_to_charstr(s) ((charstr) {strlen((s)), (s)})

#endif /* LPP_CHARSTR_UTIL_H */
