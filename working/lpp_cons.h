/*
   lpp_cons.h
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

#ifndef LPP_CONS_H
#define LPP_CONS_H

#include "lpp_obj.h"

/* Data for cons-cell objects */
struct Lcons_cell {
  Lobj *car;
  Lobj *cdr;
};

#define LTCONS_SIZE		(sizeof(Lobj) + sizeof(struct Lcons_cell))
#define Lcons_cval(cons) (*((struct Lcons_cell *) LOBJ_DATA(cons)))
#define Lcons_car(cons) (Lcons_cval(cons).car)
#define Lcons_cdr(cons) (Lcons_cval(cons).cdr)

Lobj *Lmake_cons(Lobj *car, Lobj *cdr);
Lobj *Lmake_list(int length, ...);
void Ldelete_cons(Lobj *cons);
int Llist_length(Lobj *cons);
Lobj *Lcons_to_string(Lobj *cons);
Lobj *Lcons_eval(Lobj *cons);
Lobj *Llist_eval(Lobj *list);
Lobj *Lcons_copy(Lobj *cons);
Lobj *Llist_copy(Lobj *cons);
Lobj *Lcons_equals(Lobj *cons1, Lobj *cons2);

#endif /* LPP_CONS_H */
