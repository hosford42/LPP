/*
   lpp_obj.h
     The Lobject structure, used to implement all LPP types.
   
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

#ifndef LPP_OBJ_H
#define LPP_OBJ_H

struct Lobject {
  unsigned int Ltype; /* The type of the object */
  int Lused; /* The number of references to the object */
};

typedef struct Lobject Lobj;

/* Data for each object will be stored in additional space allocated immediately after
	the Lobj struct in memory. */
#define LOBJ_DATA(objptr) ((void *) ((objptr) + 1))

#endif /* LPP_OBJ_H */
