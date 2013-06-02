/*
   lpp_quote.h
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

#ifndef LPP_QUOTE_H
#define LPP_QUOTE_H

#include "lpp_obj.h"

#define LTQUOTE_SIZE		(sizeof(Lobj) + sizeof(Lobj *))
#define Lquote_cval(quote)	(*((Lobj **) LOBJ_DATA(quote)))

Lobj *Lmake_quote(Lobj *qval);
void Ldelete_quote(Lobj *quote);
Lobj *Lquote_to_string(Lobj *quote);
Lobj *Lquote_eval(Lobj *quote);
Lobj *Lquote_copy(Lobj *quote);
Lobj *Lquote_equals(Lobj *quote1, Lobj *quote2);

#endif /* LPP_QUOTE_H */
