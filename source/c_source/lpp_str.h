/*
   lpp_str.h
     The Lstr string struct, used by LPP to hold the data for the LPP string type.
   
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

#ifndef LPP_STR_H
#define LPP_STR_H

#include <stdio.h>

struct Lstr {
  int rw; /* Read (0) or write (1) */
  int length; /* Length of string or buffer */
  int index; /* Index of operation */
  char **buffer; /* String buffer */
};
typedef struct Lstr Lstr;

int Lstr_getc(Lstr *str) {
  if(str->index >= str->length - 1)
    return EOF;
  return *(str->buffer)[str->index++];
}

int Lstr_putc(int c, Lstr *str) {
  if(str->index >= str->length - 1) {
    while(str->index >= str->length - 1)
      str->length *= 2;
    LMrealloc(*(str->buffer), str->length);
  }
  *(str->buffer)[str->index++] = c;
  *(str->buffer)[str->index] = 0;
  
  return 0;
}

void Lstr_ungetc(int c, Lstr *str) {
  if(str->index > 0)
    *(str->buffer)[--str->index] = c;
  else {
    printf("\nERROR: Called 'Lstr_unget' with insufficient room in buffer\n");
    exit(1);
  }
}

int Lstr_scan_double(Lstr *s, double *arg) {
  int result = 0;
  char *end;
  *arg = strtod(*(s->buffer) + s->index, &end);

  printf("\n");
  int i;
  for(i = 0; i < end - *(s->buffer); i++)
    printf("%c", *(s->buffer)[i]);
  printf("\n");

  if(end == *(s->buffer) + s->index)
    result = EOF;
  s->index = end - *(s->buffer);
  return result;
}

int Lstr_puts(const char *arg, Lstr *s) {
  int size = strlen(arg);
  if(s->index + size >= s->length - 1) {
    while(s->index + size >= s->length - 1)
      s->length*= 2;
    LMrealloc(*(s->buffer), s->length);
  }
  strncpy(*(s->buffer) + s->index, arg, size);
  s->index+= size;
  *(s->buffer)[s->index] = 0;
  return 0;
}

Lstr *Lstr_open(char **s, char type) {
  Lstr *result = 0;
  LMalloc(result, sizeof(Lstr));

  result->buffer = s;
  switch(type) {
    case 'r':
      result->index = 0;
      result->rw = 0;
      if(!*s) {
        LMalloc(*s, 1);
        *s[0] = 0;
      }
      result->length = strlen(*s) + 1;
      break;
      
    case 'w':
      result->index = 0;
      result->rw = 1;
      if(!*s)
        LMalloc(*s, 1);
      *s[0] = 0;
      result->length = 1;
      break;
      
    case 'a':
      if(!*s) {
        LMalloc(*s, 1);
        *s[0] = 0;
      }
      result->index = strlen(*s);
      result->rw = 1;
      result->length = result->index + 1;
      break;
      
    default:
      result = 0;
      /*printf("\nERROR: Could not open string stream; unexpected stream type: %c\n", type);
      exit(1);*/
  }

  return result;    
}

#define Lstr_close(s) ((LMfree((s)), (s) = 0, 0))

#define Lstr_eof(s) (!*((s)->buffer)[(s)->index])

#endif /* LPP_STR_H */
