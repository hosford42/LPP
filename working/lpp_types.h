/*
   lpp_types.h
     The LPP LTYPE enumeration of built-in type identifiers.
   
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

#ifndef LPP_TYPES_H
#define LPP_TYPES_H

/* Lisp types */
enum LTYPE {
  LTNIL,
  LTSYMBOL,
  LTNUMBER,
  LTSTRING,
  LTCONS,
  LTQUOTE,
  LTFUNC,
  LTLAMBDA,
  LTSTREAM,
  LTLCF,
  LTTYPE_TOKEN,
  LTEXCEPTION,
  LTDEACTIVATED_EXCEPTION,
  LTUDOBJECT,
  LTUDCLASS,
  LTARRAY,
  LNUMBER_OF_TYPES
};
typedef enum LTYPE LTYPE;

#endif /* LPP_TYPES_H */
