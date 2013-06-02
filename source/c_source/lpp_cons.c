/*
   lpp_cons.c
     The LPP cons type, for building lists and trees.
   
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

#ifndef LPP_CONS_C
#define LPP_CONS_C

#include "lpp_cons.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_string.h"
#include "lpp_lambda.h"
#include "lpp_exception.h"

#include <stdarg.h>

Lobj *Lmake_cons(Lobj *car, Lobj *cdr) {
  Lobj *new_cons = 0;
  LMalloc(new_cons, LTCONS_SIZE);
    
  new_cons->Ltype = LTCONS;
  new_cons->Lused = 0;
  
  LINIT(Lcons_car(new_cons), car);
  LINIT(Lcons_cdr(new_cons), cdr);
  
  return new_cons;
}

Lobj *Lmake_list(int length, ...) {
  va_list ap;
  Lobj *start;
  Lobj *current;
  
  va_start(ap, length);
  
  start = current = Lmake_cons(va_arg(ap, Lobj *), Lnil);
  length--;
  
  while(length != 0) {
    current = LSET(Lcons_cdr(current), Lmake_cons(va_arg(ap, Lobj *), Lnil));
    length--;
  }
  
  va_end(ap);
  
  return start;
}

void Ldelete_cons(Lobj *cons) {
  LSET(Lcons_car(cons), Lnil);
  LSET(Lcons_cdr(cons), Lnil);
  LMfree(cons);
}

Lobj *Lcons_to_string(Lobj *cons) {
  int i = 0; /* array index */
  int l = 10; /* length of array */
  char *buff = 0; /* array */
  LMalloc(buff, l + 10);
  
  buff[i++] = '(';
  buff[i++] = 0;
  while(cons && (cons->Ltype == LTCONS)) {
    Lobj *cars = Lnil; /* string rep. of (car cons) */
    LSET(cars, Lto_string(Lcons_car(cons)));
    
    i+= strlen(Lstring_cval(cars));
    if(i >= l) { /* Make sure there's enough room in buff for sizeof(cars) + a few chars */
      while(i >= l)
        l*= 2;
      if(!(buff = (char *) realloc(buff, l + 10))) {
        printf("\nUNRECOVERABLE ERROR: Insufficient memory\n");
        exit(1);
      }
    }
    strcat(buff, Lstring_cval(cars));
    
    Lrelease(cars);
    
    cons = Lcons_cdr(cons);
    if(cons)
      strcat(buff, " ");
    else
      strcat(buff, ")");
    i++;
  }
  
  if(cons) {
    Lobjptr(cdrs);
    strcat(buff, ". ");
    i+= 2;
    LSET(cdrs, Lto_string(cons));
    
    i+= strlen(Lstring_cval(cdrs));
    if(i >= l) {
      while(i >= l)
        l*= 2;
      if(!(buff = (char *) realloc(buff, l + 10))) {
        printf("\nUNRECOVERABLE ERROR: Insufficient memory\n");
        exit(1);
      }
    }
    strcat(buff, Lstring_cval(cdrs));
    
    Lrelease(cdrs);
    
    strcat(buff, ")");
  }
  
  return Lmake_string_no_copy(buff);
}

Lobj *Lcons_eval(Lobj *cons) {
  Lobj *evalled_list;

  Lerror *errval = Lcheck_args(cons, 1, -1, LTNIL, LTNIL); /* Just make sure it's a well-formed list */
  if(errval->level) {
    /* ERROR */
    Lobj *result = Lmake_exception(Lmake_string("Ill-formed list passed to Lcons_eval in argument "), Lmake_number(errval->level));
	LMfree(errval);
	return result;
  }
  LMfree(errval);
  
  LINIT(evalled_list, Llist_eval(cons));
  if(evalled_list->Ltype == LTEXCEPTION)
    return evalled_list;
      
  if(!Lcons_car(evalled_list)) {
    Lrelease_no_delete(evalled_list);
    return evalled_list;
  }
  if(Lcons_car(evalled_list)->Ltype != LTFUNC) {
    if(Lcons_car(evalled_list)->Ltype != LTLAMBDA) {
      /* ERROR */
      Lobj *ret = Lnil;
      LSET(ret, Lmake_exception(Lmake_string("Bad function: "), Lcons_car(evalled_list)));
      Lrelease(evalled_list);
      return ret;
    }
    
    LSET(evalled_list, Llambda_apply(Lcons_car(evalled_list), Lcons_cdr(evalled_list)));
    Lrelease_no_delete(evalled_list);
    
    return evalled_list;
  }
  
  LSET(evalled_list, Lfunc_func(Lcons_car(evalled_list))(Lcons_cdr(evalled_list)));
  Lrelease_no_delete(evalled_list);
  
/*  printf(") ");*/
  return evalled_list;
}

Lobj *Llist_eval(Lobj *list) {
  if(!list)
    return Lnil;
  Lcapture(list);
  Lobj *tcar = Lnil;
  Lobj *tcdr = Lnil;
  Lobj *ret = Lnil;
  LSET(tcar, Leval(Lcons_car(list)));
  if(tcar && (tcar->Ltype == LTEXCEPTION)) {
    Lrelease(list);
    Lrelease_no_delete(tcar);
    return tcar;
  }
  LSET(tcdr, Llist_eval(Lcons_cdr(list)));
  if(tcdr && (tcdr->Ltype == LTEXCEPTION)) {
    Lrelease(tcar);
    Lrelease(list);
    Lrelease_no_delete(tcdr);
    return tcdr;
  }
  LSET(ret, Lmake_cons(tcar, tcdr));
  Lrelease(tcar);
  Lrelease(tcdr);
  Lrelease(list);
  Lrelease_no_delete(ret);
  return ret;
}

Lobj *Lcons_copy(Lobj *cons) {
  return Lmake_cons(Lcons_car(cons), Lcons_cdr(cons));
}

Lobj *Llist_copy(Lobj *cons) {
  Lobj *root;
  Lobj *last;
  root = last = Lmake_cons(Lcons_car(cons), Lnil);
  while(cons = Lcons_cdr(cons))
    last = LSET(Lcons_cdr(last), Lmake_cons(Lcons_car(cons), Lnil));
  return root;
}

/* Count includes non-cons tail */
int Llist_length(Lobj *cons) {
  int length = 0;
  while(cons && (cons->Ltype == LTCONS)) {
    length++;
    cons = Lcons_cdr(cons);
  }
  return length + (cons ? 1 : 0);
}

Lobj *Lcons_equals(Lobj *cons1, Lobj *cons2) {
  return (Lequals(Lcons_car(cons1), Lcons_car(cons2)) && Lequals(Lcons_cdr(cons1), Lcons_cdr(cons2))) ? Lt : Lnil;
}


#endif /* LPP_CONS_C */
