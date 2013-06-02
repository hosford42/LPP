/*
   lpp_lexer.h
     The LPP lexer.
   
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

#ifndef LPP_LEXER_H
#define LPP_LEXER_H

#include "lpp_obj.h"
#include "stdio.h"

int Lis_identifier_char(int c);

/* Ignore everything up until the beginning of a pre-processor command */
int LPlex_ignore(Lstrm *input, Lstrm *output);

/* Lexical analysis */
int LPlex(Lstrm *input, Lobj **LPvalue);

Lobj *LPparse_identifier(Lstrm *input);

Lobj *LPparse_number(Lstrm *input);

Lobj *LPparse_string(Lstrm *input);

#endif /* LPP_LEXER_H */
