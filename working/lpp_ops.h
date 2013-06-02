/*
   lpp_ops.h
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

#ifndef LPP_OPS_H
#define LPP_OPS_H

#include "lpp_obj.h"
#include "lpp_types.h"
#include "lpp_string.h"

typedef void (*Ldelete_method)(Lobj *);
typedef Lobj *(*Lgeneric_1method)(Lobj *);
typedef Lobj *(*Lgeneric_2method)(Lobj *, Lobj *);

enum Lgeneric_1method_index {
  LTO_STRING,
  LEVAL,
  LCOPY,
  LNUMBER_OF_GENERIC_1METHODS
};

enum Lgeneric_2method_index {
  LEQUALS,
  LNUMBER_OF_GENERIC_2METHODS
};

struct Ltype_info {
  Ldelete_method do_delete;
  Lgeneric_1method _1methods[LNUMBER_OF_GENERIC_1METHODS];
  Lgeneric_2method _2methods[LNUMBER_OF_GENERIC_2METHODS];
  Lobj *token;
};

/* Array of type information objects */
struct Ltype_info Ltypes[LNUMBER_OF_TYPES];

/* Generic Ldelete_<type> function for simple atoms */
void Ldelete_simple(Lobj *atom);

/* Generic L<type>_eval function for simple atoms */
Lobj *Lsimple_eval(Lobj *atom);

/* Generic L<type>_copy function for uncopyable atoms */
Lobj *Lno_copy(Lobj *atom);

/* Generic L<type>_equals function for unique simple atoms */
Lobj *Lsimple_equals(Lobj *atom1, Lobj *atom2);

#define Ldelete(objptr) {\
  if((objptr))\
    Ltypes[(objptr)->Ltype].do_delete((objptr));\
}
/*#undef Ldelete
  #define Ldelete(objptr) {(objptr);}*/

#define Lcapture(objptr) {\
  if((objptr) && ((objptr)->Lused != -1))\
    (objptr)->Lused++;\
}

#define Lrelease(objptr) {\
  if((objptr) && ((objptr)->Lused != -1)) {\
    (objptr)->Lused--;\
    if(!(objptr)->Lused) {\
      Ldelete((objptr));\
    } else if((objptr)->Lused < -1) {\
      printf("\nERROR: Object has negative Lused counter: %d\n", (objptr)->Lused);\
      Lprinto((objptr));\
      exit(1);\
    }\
  }\
}
/*#undef Lrelease
  #define Lrelease(objptr) {(objptr);}*/

#define Lrelease_no_delete(objptr) {\
  if((objptr) && ((objptr)->Lused != -1))\
    if(--((objptr)->Lused) < -1) {\
      printf("\nERROR: Object has negative Lused counter: %d\n", (objptr)->Lused);\
      Lprinto((objptr));\
      exit(1);\
    }\
}
/*#undef Lrelease_no_delete
  #define Lrelease_no_delete(objptr) {(objptr);}*/

Lobj *Lto_string(Lobj *obj);

void Lprinto(Lobj *obj);
void Lprints(Lobj *str);

#define Leval(objptr) ((objptr) ? \
				    Ltypes[(objptr)->Ltype]._1methods[LEVAL]((objptr)) :\
				    Lnil)

#define Lcopy(objptr) ((objptr) ? \
            Ltypes[(objptr)->Ltype]._1methods[LCOPY]((objptr)) :\
            Lnil)

#define Lequals(objptr1, objptr2) (((objptr1) && (objptr2)) ? \
            (((objptr1)->Ltype == (objptr2)->Ltype) ?\
              Ltypes[(objptr1)->Ltype]._2methods[LEQUALS]((objptr1), (objptr2)) :\
              Lnil) :\
				    (((objptr1) == (objptr2)) ? Lt : Lnil))

/* Operations for object pointers */

/* Declare an object pointer named 'name' */
#define Lobjptr(name) Lobj *name = Lnil

/* Set object pointer 'ptr' to value 'val' */
Lobj *Lset(Lobj **ptr, Lobj *val);
#define LSET(ptr, val) Lset(&(ptr), (val))

/* Set unitialized object pointer 'ptr' to value 'val' */
#define LINIT(ptr, val) {\
  (ptr) = Lnil;\
  LSET((ptr), (val));\
}

void Linput_prompt();
Lobj *Leval_and_print(Lobj *obj);
Lobj *Lsimple_eval_and_print(Lobj *obj);
Lobj *Leval_no_print(Lobj *obj);

Lobj *Lapply(Lobj *lorf, Lobj *arglist);

#endif /* LPP_OPS_H */
