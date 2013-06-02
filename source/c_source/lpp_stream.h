/*
   lpp_stream.h
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

#ifndef LPP_STREAM_H
#define LPP_STREAM_H

#include "lpp_obj.h"

#define LTSTREAM_SIZE (sizeof(Lobj) + sizeof(Lstrm))

#define Lstream_cval(strm) ((Lstrm *) LOBJ_DATA(strm))

Lobj *Lmake_file_stream(const char *name, const char *mode);
Lobj *Lmake_string_stream(char *str, char mode);
void Ldelete_stream(Lobj *strm);
Lobj *Lstream_to_string(Lobj *strm);
#define Lstream_eval Lsimple_eval
#define Lstream_copy Lno_copy
#define Lstream_equals Lsimple_equals

#endif /* LPP_STREAM_H */

