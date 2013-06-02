/*
   lpp_strm.h
     The Lstrm stream struct, used to implement the LPP stream type.
   
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

#ifndef LPP_STRM_H
#define LPP_STRM_H

#include <stdio.h>
#include "lpp_str.h"

enum Lstrm_type {
  LSTRING_STRM,
  LFILE_STRM
};
typedef enum Lstrm_type Lstrm_type;

struct Lstrm {
  Lstrm_type type;
  union {
    FILE *as_file;
    Lstr *as_string;
  } data;
};
typedef struct Lstrm Lstrm;

static Lstrm *Lstdin;
static Lstrm *Lstdout;
static Lstrm *Lstderr;

void Linit_strms() {
  Lstdin = 0;
  LMalloc(Lstdin, sizeof(Lstrm));
  Lstdin->type = LFILE_STRM;
  Lstdin->data.as_file = stdin;
  
  Lstdout = 0;
  LMalloc(Lstdout, sizeof(Lstrm));
  Lstdout->type = LFILE_STRM;
  Lstdout->data.as_file = stdout;
  
  Lstderr = 0;
  LMalloc(Lstderr, sizeof(Lstrm));
  Lstderr->type = LFILE_STRM;
  Lstderr->data.as_file = stderr;
}

#define Lgetc(strm) (\
  Lis_open(strm) ?\
    (((strm)->type == LSTRING_STRM) ?\
      Lstr_getc((strm)->data.as_string) :\
      getc((strm)->data.as_file)) :\
    EOF\
)

#define Lputc(c, strm) do {\
  if(Lis_open((strm))) {\
    if((strm)->type == LSTRING_STRM)\
      Lstr_putc((c), (strm)->data.as_string);\
    else\
      putc((c), (strm)->data.as_file);\
  }\
} while(0)

#define Lungetc(c, strm) do {\
  if(Lis_open((strm))) {\
    if((strm)->type == LSTRING_STRM)\
      Lstr_ungetc((c), (strm)->data.as_string);\
    else\
      ungetc((c), (strm)->data.as_file);\
  }\
} while(0)

#define Lscan_number(strm, arg) (\
  Lis_open((strm)) ?\
    (((strm)->type == LSTRING_STRM) ?\
      Lstr_scan_double((strm)->data.as_string, &(arg)) :\
      fscanf((strm)->data.as_file, "%lf", &(arg))) :\
    EOF\
)

#define Lputs(arg, strm) (\
  Lis_open((strm)) ?\
    (((strm)->type == LSTRING_STRM) ?\
      Lstr_puts((arg), (strm)->data.as_string) :\
      fputs((arg), (strm)->data.as_file)) :\
    EOF\
)

Lstrm *Lfopen(Lstrm *strm, const char *name, const char *type) {
  FILE *f = fopen(name, type);

  if(!strm) {
    if(!f)
      return 0;
    LMalloc(strm, sizeof(Lstrm));
  }
  strm->type = LFILE_STRM;
  strm->data.as_file = f;
  
  return f ? strm : 0;
}

Lstrm *Lsopen(Lstrm *strm, char **str, char type) {
  Lstr *s = Lstr_open(str, type);

  if(!strm) {
    if(!s)
      return 0;
    LMalloc(strm, sizeof(Lstrm));
  }
  strm->type = LSTRING_STRM;
  strm->data.as_string = s;
  
  return s ? strm : 0;
}

#define Lis_open(strm) (\
  ((strm)->type == LSTRING_STRM) ?\
    !!(strm)->data.as_string :\
    !!(strm)->data.as_file\
)

int Lclose(Lstrm *s) {
  if(!Lis_open(s))
    return 1;
  int result;
  if(s->type == LSTRING_STRM) {
    result = Lstr_close(s->data.as_string);
    s->data.as_string = 0;
  } else {
    result = fclose(s->data.as_file);
    s->data.as_file = 0;
  }
  return result;
}

#define Leof(strm) (\
  !Lis_open((strm)) ||\
  (((strm)->type == LSTRING_STRM) ?\
    Lstr_eof((strm)->data.as_string) :\
    feof((strm)->data.as_file))\
)


#endif /* LPP_STRM_H */
