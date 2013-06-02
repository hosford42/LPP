/*
   lpp_lstatics.h
     Statically declared LPP objects.
   
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

#ifndef LPP_LSTATICS_H
#define LPP_LSTSTICS_H

#include "lpp_obj.h"

/* The 'true' object in lisp, called 't' within the environment */
static Lobj *Lt;

/* The prompt string printed before user input is accepted. */
static Lobj *Linput_prompt_string;

/* The prompt string printed before user input is reprinted. */
static Lobj *Linput_report_prompt_string;

/* The prompt string printed before output is printed. */
static Lobj *Loutput_report_prompt_string;

#endif /* LPP_LSTATICS_H */
