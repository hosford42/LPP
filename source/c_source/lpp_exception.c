/*
   lpp_exception.c
     The LPP exception type.
   
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

#ifndef LPP_EXCEPTION_C
#define LPP_EXCEPTION_C

#include "lpp_exception.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_string.h"
#include "lpp_lambda.h"

#include <stdarg.h>

Lobj *Lmake_exception(Lobj *desc, Lobj *data) {
  Lobj *new_exc = 0;
  LMalloc(new_exc, LTEXCEPTION_SIZE);
    
  new_exc->Ltype = LTEXCEPTION;
  new_exc->Lused = 0;
  
  LINIT(Lexception_description(new_exc), desc);
  LINIT(Lexception_data(new_exc), data);
  
  return new_exc;
}

void Ldelete_exception(Lobj *exc) {
  LSET(Lexception_description(exc), Lnil);
  LSET(Lexception_data(exc), Lnil);
  LMfree(exc);
}

Lobj *Lexception_to_string(Lobj *exc) {
  int i = 0; /* array index */
  int l = 30; /* length of array */
  char *buff = 0; /* array */
  LMalloc(buff, l + 10);

  if(exc->Ltype == LTEXCEPTION) {  
    strcpy(buff, "\\(@exception ");
    i+= strlen("\\(@exception ");
  } else {
    strcpy(buff, "\\(@caught-exception ");
    i+= strlen("\\(@caught-exception ");
  }
  
  Lexception_to_string_helper(&l, &i, &buff, exc);
  
  strcat(buff, ")");
  
  return Lmake_string_no_copy(buff);
}

void Lexception_to_string_helper(int *buff_size, int *index, char **buff, Lobj *exc) {
  if(Lexception_data(exc) && (Lexception_data(exc)->Ltype == LTEXCEPTION)) {
    Lexception_to_string_helper(buff_size, index, buff, Lexception_data(exc));
    (*index)+= strlen("\n\t");
    strcat((*buff), "\n\t");
  }
  
  (*index)+= strlen(Lstring_cval(Lexception_description(exc)));
  if((*index) >= (*buff_size)) { /* Make sure there's enough room in buff for sizeof(cars) + a few chars */
    while((*index) >= (*buff_size))
      (*buff_size)*= 2;
    if(!((*buff) = (char *) realloc((*buff), (*buff_size) + 10))) {
      printf("\nUNRECOVERABLE ERROR: Insufficient memory\n");
      exit(1);
    }
  }
  strcat((*buff), Lstring_cval(Lexception_description(exc)));

  if(!Lexception_data(exc) || (Lexception_data(exc)->Ltype != LTEXCEPTION)) {
    Lobj *datas = Lnil; /* string rep. of data */
    LSET(datas, Lto_string(Lexception_data(exc)));
    
    (*index)+= strlen(Lstring_cval(datas));
    if((*index) >= (*buff_size)) { /* Make sure there's enough room in buff for sizeof(cars) + a few chars */
      while((*index) >= (*buff_size))
        (*buff_size)*= 2;
      if(!((*buff) = (char *) realloc((*buff), (*buff_size) + 10))) {
        printf("\nUNRECOVERABLE ERROR: Insufficient memory\n");
        exit(1);
      }
    }
    strcat((*buff), Lstring_cval(datas));
    
    Lrelease(datas);
  }
}

Lobj *Lsign_exception(Lobj *exc, const char *signature) {
  return Lmake_exception(Lmake_string(signature), exc);
}

#endif /* LPP_EXCEPTION_C */
