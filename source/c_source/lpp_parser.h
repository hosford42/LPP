/*
   lpp_parser.h
     The LPP parser.
   
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

#ifndef LPP_PARSER_H
#define LPP_PARSER_H

#include <stdio.h>

/* Defined as alphabet char to conveniently avoid
   return value which would be used for punctuation marks by LPlex */
#define LPOBJ ((int) 'a')

Lobj *LPparse_lpp_file(Lstrm *input, Lstrm *output);
Lobj *LPparse_directive(Lstrm *input);
Lobj *LPparse_multiple_exp(Lstrm *input, void (*prompt)(void), Lgeneric_1method rp);
Lobj *LPparse_exp(Lstrm *input);
Lobj *LPparse_list_tail(Lstrm *input);

#endif /* LPP_PARSER_H */
