/*
   lpp_error.c
     The LPP Lerror struct, for internal error representation.
   
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

#ifndef LPP_ERROR_C
#define LPP_ERROR_C

#include "lpp_error.h"
#include <stdarg.h>

Lerror *Lcheck_args(Lobj *args, int min_args, int max_args, ...) {
  va_list ap;
  Lerror *errval = 0;
  int arg_count;

  LMalloc(errval, sizeof(Lerror));
  
  errval->level = 0;
  errval->domain = ELISP_CODE;
  
  if(min_args < 0) {
    /* C ERROR */
    printf("CODE ERROR: Negative argument passed to min_args\nIn function Lcheck_args\n");
    errval->level = min_args;
    errval->domain = EC_CODE;
    return errval;
  }
  
  if(max_args < min_args) {
    if(max_args == -1) {
      int arg_count = 1;
      va_start(ap, max_args);
      LTYPE t = LTNIL;
      while(args && (args->Ltype == LTCONS)) {
        /* argument (min_args + 1) is the generic type for the remainder of the arguments */
        if(arg_count <= min_args + 1)
          t = va_arg(ap, LTYPE);
        if((t != LTNIL) && (!Lcons_car(args) || (t != Lcons_car(args)->Ltype))) {
          if(Lcons_car(args) || (t != LTCONS)) {
            errval->level = arg_count;
            va_end(ap);
            return errval;
          }
        }
        arg_count++;
        args = Lcons_cdr(args);
      }
      
      if(args && (args->Ltype != LTCONS))
        errval->level = -arg_count;
      else if(arg_count < min_args)
        errval->level = -arg_count;
      
      va_end(ap);
      return errval;
    } else {
      /* C ERROR */
      printf("CODE ERROR: Argument passed to max_args is less than min_args\nIn function Lcheck_args\n");
      errval->level = min_args - max_args;
      return errval;
    }
  }
  
  arg_count = 1;
  va_start(ap, max_args);
  while(args && (args->Ltype == LTCONS) && (arg_count <= max_args)) {
    LTYPE t;
    t = va_arg(ap, LTYPE);
    if((t != LTNIL) && (!Lcons_car(args) || (t != Lcons_car(args)->Ltype))) {
      if(Lcons_car(args) || (t != LTCONS)) {
        errval->level = arg_count;
        va_end(ap);
        return errval;
      }
    }
    arg_count++;
    args = Lcons_cdr(args);
  }
  
  if(args)
    errval->level = -arg_count;
  else if(arg_count <= min_args)
    errval->level = -arg_count;
  
  va_end(ap);
  return errval;
}

#endif /* LPP_ERROR_H */
