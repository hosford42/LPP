/*
   lpp_lambda.c
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

#ifndef LPP_LAMBDA_C
#define LPP_LAMBDA_C

#include "lpp_lambda.h"
#include "lpp_cons.h"

Lobj *Lmake_lambda(Lobj *args, Lobj *doc, Lobj *body) {
  Lobj *new_lambda = 0;
  LMalloc(new_lambda, LTLAMBDA_SIZE);
    
  new_lambda->Ltype = LTLAMBDA;
  new_lambda->Lused = 0;
  
  LINIT(Llambda_args(new_lambda), args);
  LINIT(Llambda_doc(new_lambda), doc);
  LINIT(Llambda_body(new_lambda), body);
  
  return new_lambda;
}

void Ldelete_lambda(Lobj *lam) {
  LSET(Llambda_args(lam), Lnil);
  LSET(Llambda_doc(lam), Lnil);
  LSET(Llambda_body(lam), Lnil);
  LMfree(lam);
}

Lobj *Llambda_to_string(Lobj *lam) {
  char *buff = 0;

  Lobj *argsstr;
  Lobj *docstr;
  Lobj *bodystr;
  
  LINIT(argsstr, Lto_string(Llambda_args(lam)));
  LINIT(docstr, Lto_string(Llambda_doc(lam)));
  LINIT(bodystr, Lto_string(Llambda_body(lam)));
  
  LMalloc(buff, 13 + strlen(Lstring_cval(argsstr)) + strlen(Lstring_cval(docstr)) + strlen(Lstring_cval(bodystr)));
  strcpy(buff, "\\(lambda ");
  strcat(buff, Lstring_cval(argsstr));
  strcat(buff, " ");
  strcat(buff, Lstring_cval(docstr));
  strcat(buff, " ");
  strcat(buff, Lstring_cval(bodystr));
  strcat(buff, ")");
  
  Lrelease(argsstr);
  Lrelease(docstr);
  Lrelease(bodystr);
  
  return Lmake_string_no_copy(buff);
}

Lobj *Llambda_apply(Lobj *lam, Lobj *arglist) {
  Lobj *ret = Lnil;

  /* Bind the arguments */     
  Lerror *errval = Lbind_args(Llambda_args(lam), arglist);

  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    LSET(ret, Lmake_exception(Lmake_string("Unable to bind arguments to function: "),
                              Lmake_list(3,
                                         Lmake_list(2, Lmake_string("function"), lam),
                                         Lmake_list(2, Lmake_string("argument list"), arglist),
                                         Lmake_list(2, Lmake_string("error code"), Lmake_number(errval->level)))));
    Lrelease_no_delete(ret);
    return ret;
  }
  LMfree(errval);
  
  Lcapture(lam);
  Lcapture(arglist);
  
  /* Evaluate the body, saving the result */
  LSET(ret, Leval(Llambda_body(lam)));
  
  /* Unbind the arguments */
  errval = Lunbind_args(Llambda_args(lam));
  if(errval->level) {
    /* ERROR */
	LMfree(errval);
    LSET(ret, Lmake_exception(Lmake_string("Unable to unbind arguments to function: "),
                              Lmake_list(3,
                                         Lmake_list(2, Lmake_string("function"), lam),
                                         Lmake_list(2, Lmake_string("argument list"), arglist),
                                         Lmake_list(2, Lmake_string("error code"), Lmake_number(errval->level)))));
    Lrelease_no_delete(ret);
    return ret;
  }
  LMfree(errval);
  
  /* Return the result */
  Lrelease_no_delete(ret);
  Lrelease_no_delete(lam);
  Lrelease_no_delete(arglist);
  return ret;
}

Lerror *Lbind_args(Lobj *args, Lobj *arglist) {
  Lerror *errval = 0;
  int argnum = 0;
  Lobj *astore = args;
  
  LMalloc(errval, sizeof(Lerror));
  
  /* Default is no error */
  errval->level = 0;
  errval->domain = ELISP_CODE;
  
  /* If the function takes no arguments, just make sure it
     wasn't passed any. */
  if(!args) {
    if(arglist) {
      /* ERROR */
      errval->level = -1;
    }
    return errval;
  }
  
  /* Bind each argument var up until the next to last one. */
  while(Lcons_cdr(args)) {
    argnum++;
    
    /* Make sure there were enough arguments passed. */
    if(!arglist) {/* ERROR */
      /* Undo completed bindings */
      int anum = 0;
      while(anum < argnum) {
        anum++;
        if(Lcons_car(astore))
          Lsymbol_pop(Lcons_car(astore));
        astore = Lcons_cdr(astore);
      }
      
      errval->level = argnum;
      return errval;
    }
    
    /* A 'nil' value in the args list means the value is ignored. */
    if(Lcons_car(args))
      Lsymbol_push(Lcons_car(args), Lcons_car(arglist));
      
    args = Lcons_cdr(args);
    arglist = Lcons_cdr(arglist);
  }
  
  /* If the last value in the args list is nil, we don't accept any more
     arguments. */
  if(!Lcons_car(args)) {
    if(arglist) {/* ERROR */
      /* Undo completed bindings */
      int anum = 0;
      while(anum < argnum) {
        anum++;
        if(Lcons_car(astore))
          Lsymbol_pop(Lcons_car(astore));
        astore = Lcons_cdr(astore);
      }
      errval->level = argnum;
    }
    return errval;
  }
  
  /* Bind the last value in the args list to the remainder of the arglist */
  Lsymbol_push(Lcons_car(args), arglist);
  return errval;
}

Lerror *Lunbind_args(Lobj *args) {
  Lerror *errval = 0;
  
  LMalloc(errval, sizeof(Lerror));

  /* Just go through the args list and unbind each symbol in it */
  while(args) {
    if(Lcons_car(args))
      Lsymbol_pop(Lcons_car(args));
    args = Lcons_cdr(args);
  }
  
  /* There's no expected way for this to go wrong */
  errval->level = 0;
  errval->domain = EHARDWARE;
  return errval;
}


#endif /* LPP_LAMBDA_C */
