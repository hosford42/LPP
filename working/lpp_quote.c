/*
   lpp_quote.c
     The LPP quote type, used for mentioning data rather than evaluating it.
   
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

#ifndef LPP_QUOTE_C
#define LPP_QUOTE_C

#include "lpp_quote.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_string.h"

Lobj *Lmake_quote(Lobj *qval) {
  Lobj *new_quote = 0;
  LMalloc(new_quote, LTQUOTE_SIZE);
    
  new_quote->Ltype = LTQUOTE;
  new_quote->Lused = 0;
  LINIT(Lquote_cval(new_quote), qval);
  
  return new_quote;
}

void Ldelete_quote(Lobj *quote) {
  LSET(Lquote_cval(quote), Lnil);
  LMfree(quote);
}

Lobj *Lquote_to_string(Lobj *quote) {
  Lobj *s = Lnil;
  char *buff = 0;
  
  LSET(s, Lto_string(Lquote_cval(quote)));
  LMalloc(buff, strlen(Lstring_cval(s)) + 2);
  strcpy(buff, "'");
  strcat(buff, Lstring_cval(s));
  Lrelease(s);
  
  return Lmake_string_no_copy(buff);
}

Lobj *Lquote_eval(Lobj *quote) {
  return Lquote_cval(quote);
}

Lobj *Lquote_copy(Lobj *quote) {
  return Lmake_quote(Lquote_cval(quote));
}

Lobj *Lquote_equals(Lobj *quote1, Lobj *quote2) {
  return Lequals(Lquote_cval(quote1), Lquote_cval(quote2));
}

#endif /* LPP_QUOTE_C */
