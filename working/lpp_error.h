/*
   lpp_error.h
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

#ifndef LPP_ERROR_H
#define LPP_ERROR_H

#include "lpp_obj.h"

struct Lerror {
  int level;
  enum {
    ELISP_CODE,
    EC_CODE,
    EHARDWARE,
    EPASSING_ON
  } domain;
};
typedef struct Lerror Lerror;

Lerror *Lcheck_args(Lobj *args, int min_args, int max_args, ...);

#endif /* LPP_ERROR_H */
