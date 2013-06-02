/*
   lpp_lexer.c
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

#ifndef LPP_LEXER_C
#define LPP_LEXER_C

#include "lpp_lexer.h"
#include "lpp_exception.h"

/* Lexical analysis */
int LPlex(Lstrm *input, Lobj **LPvalue) {
  int c;
  
  while((c = Lgetc(input)) != EOF) {
    if(c == ';') {
      do {
        c = Lgetc(input);
        if(c == '\n') {
          c = Lgetc(input);
          break;
        }
      } while(c != EOF);
    }
        
    if(c == '"') {
      Lungetc(c, input);
      Lset(LPvalue, LPparse_string(input));
      return LPOBJ;
    }
    
    if(isdigit(c)) {
      Lungetc(c, input);
      Lset(LPvalue, LPparse_number(input));
      return LPOBJ;
    }
    
    if(c == '.') {
      int c2 = Lgetc(input);
      
      if(isdigit(c2)) {
        Lungetc(c2, input);
        Lungetc(c, input);
        Lset(LPvalue, LPparse_number(input));
        return LPOBJ;
      }
      
      Lungetc(c2, input);
      Lungetc(c, input);
      Lset(LPvalue, LPparse_identifier(input));
      return LPOBJ;
    }
    
    if(c == '-') {
      int c2 = Lgetc(input);

      if(isdigit(c2)) {
        Lungetc(c2, input);
        Lungetc(c, input);
        Lset(LPvalue, LPparse_number(input));
        return LPOBJ;
      }
      
      if(c2 == '.') {
        int c3 = Lgetc(input);
        
        if(isdigit(c3)) {
          Lungetc(c3, input);
          Lungetc(c2, input);
          Lungetc(c, input);
          Lset(LPvalue, LPparse_number(input));
          return LPOBJ;
        }
        
        Lungetc(c3, input);
      }
      
      Lungetc(c2, input);
      Lungetc(c, input);
      Lset(LPvalue, LPparse_identifier(input));
      return LPOBJ;
    }
    
    if(Lis_identifier_char(c)) {
      Lungetc(c, input);
      Lset(LPvalue, LPparse_identifier(input));
      return LPOBJ;
    } else if(!isspace(c))
      /* It's a punctuation mark */
      return c;   
  }
  
  return EOF;
}

/* Ignore everything up until the beginning of a pre-processor command */
int LPlex_ignore(Lstrm *input, Lstrm *output) {
  int c;
  
  do {
    if((c = Lgetc(input)) == EOF)
      return EOF;
    
    while(c != '#') {
      Lputc(c, output);
      if((c = Lgetc(input)) == EOF)
        return EOF;
    }
    
    if((c = Lgetc(input)) == EOF) {
      Lputc('#', output);
      return EOF;
    }
      
    if(c == '#') {
      Lputc(c, output);
    }
        
  } while(c == '#');
      
  return c; /* Return the command type: #$, #!, or #;" */
}

Lobj *LPparse_identifier(Lstrm *input) {
  char *symbol_buffer = 0;
  int length = 40;
  int i = 0;

  LMalloc(symbol_buffer, length + 1);
  int c = Lgetc(input);
  do {
    if(i >= length) {
      length*= 2;
      LMrealloc(symbol_buffer, length + 1);
    }
    symbol_buffer[i++] = c;
    c = Lgetc(input);
  } while(Lis_identifier_char(c)); /* Make sure it's not a lisp or control char */
  Lungetc(c, input);
  symbol_buffer[i] = '\0';
  
  Lobj *ret = Llookup_symbol(symbol_buffer);
  LMfree(symbol_buffer);
  return ret;
}

Lobj *LPparse_number(Lstrm *input) {
  double numval;
  Lscan_number(input, numval);
  return Lmake_number(numval);
}

Lobj *LPparse_string(Lstrm *input) {
  char *buff = 0;
  int bsize = 10;
  int i = 0;
  int c;
  int delimiter = Lgetc(input); /* Remember what the delimiter char is */
  
  LMalloc(buff, bsize);

  while((c = Lgetc(input)) != EOF) {
    if(c == '\\') {
      if(i + 5 >= bsize) {
        bsize*= 2;
        LMrealloc(buff, bsize);
      }
      c = Lgetc(input);
      if(c == EOF) {
        buff[i] = '\0';
        return Lmake_exception(Lmake_string("Unexpected end of file in function LPparse_string."), Lmake_string_no_copy(buff));
      }
      if(c == 'n') {
        buff[i++] = '\n';
      } else if(c == 't') {
        buff[i++] = '\t';
      } else if(c == '0') {
        buff[i++] = '\0';
      } else if(c == 'b') {
        buff[i++] = '\b';
      } else if(c == 'r') {
        buff[i++] = '\r';
      } else if(c == 'f') {
        buff[i++] = '\f';
      } else {
        buff[i++] = c;
      }
    } else {
      if(c == delimiter) {
        buff[i] = '\0';
        return Lmake_string_no_copy(buff);
      }

      buff[i++] = c;
      if(i >= bsize) {
        bsize*= 2;
        LMrealloc(buff, bsize);
      }
    }
  }
  
  buff[i] = '\0';
  return Lmake_exception(Lmake_string("Unexpected end of file in function LPparse_string."), Lmake_string_no_copy(buff));
}

int Lis_identifier_char(int c) {
  return isalpha(c) ||
       ((c != EOF) &&
       (c != ' ') &&
       (c != '(') &&
       (c != ')') &&
       (c != '\n') &&
       (c != '\t') &&
       (c != '"') &&
       (c != '\\') &&
       (c != '#') &&
       (c >= 32) &&
       (c != '\b') &&
       (c != '\r') &&
       (c != '\f') &&
       (c != '\''));
}

#endif /* LPP_LEXER_C */
