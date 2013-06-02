/*
   lpp_ops.c
    General operations with no other obvious home.
   
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

#ifndef LPP_OPS_C
#define LPP_OPS_C

#include "lpp_ops.h"
#include "lpp_lstatics.h"
#include "lpp_cstatics.h"
#include "lpp_func.h"
                                      

void Ldelete_simple(Lobj *atom) {
  LMfree(atom);
}

Lobj *Lsimple_eval(Lobj *atom) {
  return atom;
}

Lobj *Lno_copy(Lobj *atom) {
  return atom;
}

Lobj *Lsimple_equals(Lobj *atom1, Lobj *atom2) {
  return (atom1 == atom2) ? Lt : Lnil;
}

Lobj *Lto_string(Lobj *obj) {
  return (!obj ?
           Lmake_string("()") :
           Ltypes[obj->Ltype]._1methods[LTO_STRING](obj));
}

void Lprinto(Lobj *obj) {
  Lobj *s = Lnil;
  LSET(s, Lto_string(obj));
  Lprints(s);
  LSET(s, Lnil);
}

void Lprints(Lobj *str) {
  if(!str) {
    printf("\nERROR: Null pointer passed to Lprints\n");
    exit(1);
  }
  if(str->Ltype != LTSTRING) {
    printf("\nERROR: Argument to Lprints is not a string\n");
    exit(1);
  }
  printf("%s", Lstring_cval(str));
}

Lobj *Lset(Lobj **ptr, Lobj *val) {
  Lobj *temp = *ptr;
  *ptr = val;
  Lcapture(val);
  Lrelease(temp);
  return val;
}

void Linput_prompt() {
  if(!Lquiet_mode && Lprompt_input)
    Lprints(Linput_prompt_string);
}

Lobj *Leval_and_print(Lobj *obj) {
  Lcapture(obj);
  if(!Lquiet_mode && Linput_report) {
    Lprints(Linput_report_prompt_string);
    Lprinto(obj);
  }
  Lobj *ret = Leval(obj);
  if((!Lquiet_mode && Loutput_report) || (ret && (ret->Ltype == LTEXCEPTION))) {
    Lprints(Loutput_report_prompt_string);
    Lprinto(ret);
  }
  Lrelease_no_delete(obj);
  return ret;
}

Lobj *Lsimple_eval_and_print(Lobj *obj) {
  Lcapture(obj);
  Lobj *ret = Leval(obj);
  Lrelease_no_delete(obj);
  if(!Lquiet_mode || (ret && (ret->Ltype == LTEXCEPTION))) {
    Lprinto(ret);
    printf("\n");
  }
  return ret;
}

Lobj *Leval_no_print(Lobj *obj) {
  Lobj *ret = Lnil;
  LSET(ret, obj);
  LSET(ret, Leval(ret));
  if(ret && (ret->Ltype == LTEXCEPTION)) {
    Lprints(Loutput_report_prompt_string);
    Lprinto(ret);
  }
  Lrelease_no_delete(ret);
  return ret;
}

Lobj *Lapply(Lobj *lorf, Lobj *arglist) {
  if(!lorf)
    return Lnil;
  
  if(lorf->Ltype == LTFUNC)
    return Lfunc_func(lorf)(arglist);
  
  if(lorf->Ltype == LTLAMBDA)
    return Llambda_apply(lorf, arglist);
  
  return Lmake_exception(Lmake_string("Bad function: "), lorf);
}

#endif /* LPP_OPS_C */
