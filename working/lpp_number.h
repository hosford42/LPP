/*
   lpp_number.h
     The LPP number type.
   
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

#ifndef LPP_NUMBER_H
#define LPP_NUMBER_H

#include "lpp_obj.h"
#include "lpp_ops.h"

#define LTNUMBER_SIZE	(sizeof(Lobj) + sizeof(double))

#define Lnumber_cval(number) (*((double *) LOBJ_DATA(number)))

Lobj *Lmake_number(double numval);
#define Ldelete_number Ldelete_simple
Lobj *Lnumber_to_string(Lobj *num);
#define Lnumber_eval Lsimple_eval
Lobj *Lnumber_copy(Lobj *num);
Lobj *Lnumber_equals(Lobj *num1, Lobj *num2);

#endif /* LPP_NUMBER_H */
