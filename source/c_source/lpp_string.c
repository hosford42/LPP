/*
   lpp_string.c
     The LPP string type.
   
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

#ifndef LPP_STRING_C
#define LPP_STRING_C

#include "lpp_string.h"
#include "lpp_types.h"

Lobj *Lmake_string(const char *strval) {
  char *s = 0;
  LMalloc(s, strlen(strval) + 1);
  strcpy(s, strval);
  
  return Lmake_string_no_copy(s);
}

Lobj *Lmake_string_no_copy(char *strval) {
  Lobj *new_string = 0;
  LMalloc(new_string, LTSTRING_SIZE);
    
  new_string->Ltype = LTSTRING;
  new_string->Lused = 0;
  Lstring_cval(new_string) = strval;
  
  return new_string;
}

Lobj *Lmake_string_from_char(char c) {
  char *s = 0;
  LMalloc(s, 2);
  s[0] = c;
  s[1] = 0;
  
  return Lmake_string_no_copy(s);
}

void Ldelete_string(Lobj *string) {
  LMfree(Lstring_cval(string));
  LMfree(string);
}

/* NOT DONE -- character escape sequences are still lacking. */
Lobj *Lstring_to_string(Lobj *string) {
  char *buff = 0;
  LMalloc(buff, strlen(Lstring_cval(string)) + 3);
  strcpy(buff, "\"");
  strcat(buff, Lstring_cval(string));
  strcat(buff, "\"");
  return Lmake_string_no_copy(buff);
}

Lobj *Lstring_copy(Lobj *string) {
  return Lmake_string(Lstring_cval(string));
}

Lobj *Lstring_equals(Lobj *string1, Lobj *string2) {
  return strcmp(Lstring_cval(string1), Lstring_cval(string2)) ? Lnil : Lt;
}

Lobj *Lstring_cat(Lobj *args) {
  int blength = strlen(Lstring_cval(Lcons_car(args)));
  int bsize = blength + 1;
  char *buff = 0;
  
  LMalloc(buff, bsize);
  strcpy(buff, Lstring_cval(Lcons_car(args)));
  
  args = Lcons_cdr(args);
  while(args) {
    int i = blength;
    blength+= strlen(Lstring_cval(Lcons_car(args)));
    if(blength >= bsize) {
      while(blength >= bsize)
        bsize*= 2;
      LMrealloc(buff, bsize);
    }
    strcpy(buff + i, Lstring_cval(Lcons_car(args)));
    args = Lcons_cdr(args);
  }
  
  return Lmake_string_no_copy(buff);
}

/* TODO: This is kind of a jacked up way to do indexing. Not sure what
   I was thinking at the time. It should work in a more standard/intuitive way. */
Lobj *Lstring_substring(Lobj *string, int start, int end) {
  int l = strlen(Lstring_cval(string));
  
  while(start < 0)
    start+= l;

  while(start > l)
    start-= l;
  
  while(end < 0)
    end+= l;

  while(end > l)
    end-= l;
      
  if(end < start) {
    int temp = end;
    end = start;
    start = temp;
  }
  
  int size = end - start;
  char *buff = 0;
  LMalloc(buff, size + 1);
  
  strncpy(buff, Lstring_cval(string) + start, size);
  buff[size] = '\0';
  
  return Lmake_string_no_copy(buff);
}

#endif /* LPP_STRING_C */
