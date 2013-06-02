/*
   lpp_number.c
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

#ifndef LPP_NUMBER_C
#define LPP_NUMBER_C

#include "lpp_number.h"
#include "lpp_types.h"
#include "lpp_string.h"

Lobj *Lmake_number(double numval) {
  Lobj *new_number = 0;
  LMalloc(new_number, LTNUMBER_SIZE);
    
  new_number->Ltype = LTNUMBER;
  new_number->Lused = 0;
  Lnumber_cval(new_number) = numval;
  
  return new_number;
}

Lobj *Lnumber_to_string(Lobj *num) {
  char buff[20];
  sprintf(buff, "%.10g", Lnumber_cval(num));
  return Lmake_string(buff);
}

Lobj *Lnumber_copy(Lobj *num) {
  return Lmake_number(Lnumber_cval(num));
}

Lobj *Lnumber_equals(Lobj *num1, Lobj *num2) {
  return (Lnumber_cval(num1) == Lnumber_cval(num2)) ? Lt : Lnil;
}

#endif /* LPP_NUMBER_C */
