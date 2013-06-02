/*
   lpp_parser.c
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

#ifndef LPP_PARSER_C
#define LPP_PARSER_C

#include "lpp_parser.h"
#include "lpp_lexer.h"

/*#define DEBUG_PARSER*/

Lobj *LPparse_lpp_file(Lstrm *input, Lstrm *output) {
  Lobj *dres = Lnil;
  while(!Leof(input)) {
    int licom = LPlex_ignore(input, output);
    if(licom == EOF)
      return Lnil;
      
    if((licom != '$') && (licom != '!') && (licom != ';') && (licom != '#')) 
      return Lmake_exception(Lmake_string("'$', '!', or ';' directive expected."), Lnil);
    
    if(licom != '#')
      LSET(dres, LPparse_directive(input));
    
    switch(licom) {
      case '!':
        LSET(dres, Lto_string(dres));
        Lputs(Lstring_cval(dres), output);
        break;
      case '$':
        if(!dres || (dres->Ltype != LTSTRING)) {
          if(!dres || (dres->Ltype != LTEXCEPTION))
            return Lmake_exception(Lmake_string("Error parsing \"#$\" expression."), dres);
          else
            return Lmake_exception(Lmake_string("While parsing \"#$\" expression."), dres);
        }
        Lputs(Lstring_cval(dres), output);
        break;
      case ';':
        /* Don't output anything */
        break;
      case '#':
        Lputc('#', output);
        break;
    };
  }
  LSET(dres, Lnil);
  return Lnil;
}

Lobj *LPparse_directive(Lstrm *input) {
  if(Lgetc(input) != '(')
    return Lmake_exception(Lmake_string("'(' expected"), Lnil);
  
  Lobj *ret = Lnil;
  while(!Leof(input)) {
    char c;
    while(isspace(c = Lgetc(input)));
    if(c == ')') {
      Lrelease_no_delete(ret);
      return ret;
    }
    if(c != EOF) {
      Lungetc(c, input);
      LSET(ret, LPparse_exp(input));
      LSET(ret, Leval(ret));
    }
  }
  
  return Lmake_exception(Lmake_string("')' expected"), Lnil);
}

Lobj *LPparse_multiple_exp(Lstrm *input, void (*prompt)(void), Lgeneric_1method rp) {
  Lobj *ret = Lnil;
  while(!Leof(input)) {
    if(prompt)
      prompt();
    char c;
    while(isspace(c = Lgetc(input)));
    if(c != EOF) {
      Lungetc(c, input);
      LSET(ret, LPparse_exp(input));
      LSET(ret, rp(ret));
      /*if(ret && (ret->Ltype == LTEXCEPTION)) {
        Lrelease_no_delete(ret);
        return ret;
      }*/       
    }
  }
  Lrelease_no_delete(ret);
  return ret;
}

#ifdef DEBUG_PARSER
static int depth = 0;
#endif

/* Generic parser for any lisp expression */
Lobj *LPparse_exp(Lstrm *input) {
  Lobj *value = Lnil;
  int token = LPlex(input, &value);
  
#ifdef DEBUG_PARSER
  printf("%c: ", token);
  Lprinto(value);
  printf("\n");
#endif

  switch(token) {
    case EOF:
      return Lmake_exception(Lmake_string("While parsing."), Lmake_string("Unexpected end of file."));
    
    case LPOBJ:
      if(value && (value->Ltype == LTEXCEPTION))
        return Lmake_exception(Lmake_string("While parsing."), value);
      return value;
      
    case '(':
#ifdef DEBUG_PARSER
      depth++;
      printf("[DEPTH = %d]\n", depth);
#endif
      return LPparse_list_tail(input);
   
    case '\'': 
      return Lmake_quote(LPparse_exp(input));
  
    case '#': { 
      int licom = Lgetc(input);
      if((licom != '!') && (licom != ';'))
        return Lmake_exception(Lmake_string("'!' or ';' directive expected"), Lnil);
      Lobj *ret = Lnil;
      LSET(ret, LPparse_directive(input));
      if(licom == ';')
        LSET(ret, LPparse_exp(input)); /* Ignore and persue what was expected */
      Lrelease_no_delete(ret);
      return ret;
    }
    
    default:
      return Lmake_exception(Lmake_string("Unexpected token while parsing object."), Lnil);
  }
}

/* Called when a list has just been opened */
Lobj *LPparse_list_tail(Lstrm *input) {
  Lobj *value = Lnil;
  int token = LPlex(input, &value);

#ifdef DEBUG_PARSER  
  printf("%c: ", token);
  Lprinto(value);
  printf("\n");
#endif
  
  if(token == EOF) 
    return Lmake_exception(Lmake_string("Unexpected end of file."), Lnil);
  
  if((token == LPOBJ) && value && (value->Ltype == LTEXCEPTION))
    return Lmake_exception(Lmake_string("While parsing list tail."), value);
  
  if(token == ')') {
#ifdef DEBUG_PARSER
    depth--;
    printf("[DEPTH = %d]\n", depth);
#endif

    return Lnil;
  }

  if(token == '.') {
    Lobj *ret = LPparse_exp(input);
    if((LPlex(input, &value) != ')') && !(value && (value->Ltype == LTEXCEPTION))) 
      return Lmake_exception(Lmake_string("')' expected after expression"), ret);
    return ret;
  }
  
  if(token == '(') {
#ifdef DEBUG_PARSER  
    depth++;
    printf("[DEPTH = %d]\n", depth);
#endif

    Lobj *head = Lnil;
    LSET(head, LPparse_list_tail(input));
    if(head && (head->Ltype == LTEXCEPTION)) {
      Lrelease_no_delete(head);
      return head;
    }
      
    Lobj *tail = Lnil;
    LSET(tail, LPparse_list_tail(input));
    if(tail && (tail->Ltype == LTEXCEPTION)) {
      Lrelease(head);
      Lrelease_no_delete(tail);
      return tail;
    }
    
    Lrelease_no_delete(head);
    Lrelease_no_delete(tail);
    return Lmake_cons(head, tail);
  }
  
  if(token == '\'') {
    Lobj *head = Lnil;
    LSET(head, Lmake_quote(LPparse_exp(input)));
    if(head && (head->Ltype == LTEXCEPTION)) {
      Lrelease_no_delete(head);
      return head;
    }
      
    Lobj *tail = Lnil;
    LSET(tail, LPparse_list_tail(input));
    if(tail && (tail->Ltype == LTEXCEPTION)) {
      Lrelease(head);
      Lrelease_no_delete(tail);
      return tail;
    }
    
    Lrelease_no_delete(head);
    Lrelease_no_delete(tail);
    return Lmake_cons(head, tail);
  }
  
  if(token == '#') {
    int licom = Lgetc(input);
    if((licom != '!') && (licom != ';'))
      return Lmake_exception(Lmake_string("'!' or ';' directive expected"), Lnil);
    Lobj *ret = Lnil;
    LSET(ret, LPparse_directive(input));
    switch(licom) {
      case '!': {
        Lobj *tail = Lnil;
        LSET(tail, LPparse_list_tail(input));
        if(tail && (tail->Ltype == LTEXCEPTION)) {
          Lrelease(ret);
          Lrelease_no_delete(tail);
          return Lmake_exception(Lmake_string("While parsing '!' directive."), tail);
        }
        Lrelease_no_delete(ret);
        Lrelease_no_delete(tail);
        return Lmake_cons(ret, tail);
      }
      case ';':
        LSET(ret, Lnil);
        return LPparse_list_tail(input);
    }
  }  
    
  if(token == LPOBJ) {
    Lcapture(value);
    Lobj *tail = Lnil;
    LSET(tail, LPparse_list_tail(input));
    if(tail && (tail->Ltype == LTEXCEPTION)) {
      Lrelease(value);
      Lrelease_no_delete(tail);
      return tail;
    }
    Lrelease_no_delete(value);
    Lrelease_no_delete(tail);
    return Lmake_cons(value, tail);
  }
  
  return Lmake_exception(Lmake_string("Unexpected token while parsing list."), Lnil);
}

#endif /* LPP_PARSER_C */

