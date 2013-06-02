/*
   lpp_stream.c
     The LPP stream type, for file I/O.
   
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

#ifndef LPP_STREAM_C
#define LPP_STREAM_C

#include "lpp_stream.h"
#include "lpp_types.h"
#include "lpp_strm.h"
#include "lpp_exception.h"

Lobj *Lmake_file_stream(const char *name, const char *mode) {
  Lobj *new_strm = 0;
  LMalloc(new_strm, LTSTREAM_SIZE);
  
  new_strm->Ltype = LTSTREAM;
  new_strm->Lused = 0;
  if(!Lfopen(Lstream_cval(new_strm), name, mode)) {
    Lrelease(new_strm);
    return Lmake_exception(Lmake_string("Could not open file stream: "), Lmake_string(name));
  }
  
  return new_strm;
}

Lobj *Lmake_string_stream(char *str, char mode) {
  Lobj *new_strm = 0;
  LMalloc(new_strm, LTSTREAM_SIZE);
  
  new_strm->Ltype = LTSTREAM;
  new_strm->Lused = 0;
  if(!Lsopen(Lstream_cval(new_strm), &str, mode)) {
    Lrelease(new_strm);
    return Lmake_exception(Lmake_string("Could not open string stream: "), Lmake_string(str));
  }
  
  return new_strm;
}

void Ldelete_stream(Lobj *strm) {
  if(Lclose(Lstream_cval(strm))) {
    printf("\nERROR: Could not close stream\n");
  }
  LMfree(strm);
}

Lobj *Lstream_to_string(Lobj *stream) {
  return Lmake_string("\\(@stream)");
}

#endif /* LPP_STREAM_C */
