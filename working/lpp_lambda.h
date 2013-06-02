/*
   lpp_lambda.h
     The LPP lambda type, for interpreted functions.
   
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

#ifndef LPP_LAMBDA_H
#define LPP_LAMBDA_H

#include "lpp_obj.h"
#include "lpp_error.h"

/* Data for lambda objects */
struct Llambda_func {
  Lobj *args;
  Lobj *doc;
  Lobj *body;
};

#define LTLAMBDA_SIZE		(sizeof(Lobj) + sizeof(struct Llambda_func))
#define Llambda_cval(lam) (*((struct Llambda_func *) LOBJ_DATA(lam)))
#define Llambda_args(lam) (Llambda_cval(lam).args)
#define Llambda_doc(lam) (Llambda_cval(lam).doc)
#define Llambda_body(lam) (Llambda_cval(lam).body)

Lobj *Lmake_lambda(Lobj *args, Lobj *doc, Lobj *body);
void Ldelete_lambda(Lobj *lam);
Lobj *Llambda_to_string(Lobj *lam);
#define Llambda_eval Lsimple_eval
#define Llambda_copy Lno_copy
#define Llambda_equals Lsimple_equals
Lobj *Llambda_apply(Lobj *lam, Lobj *arglist);

Lerror *Lbind_args(Lobj *args, Lobj *arglist);
Lerror *Lunbind_args(Lobj *args);

#endif /* LPP_LAMBDA_H */
