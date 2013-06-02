/*
   lpp_array.h
     The LPP array type.
   
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

#ifndef LPP_ARRAY_H
#define LPP_ARRAY_H

#include "lpp_obj.h"

/* Data for array objects */
struct Larray {
  unsigned int size;
  Lobj **vals;
};

#define LTARRAY_SIZE (sizeof(Lobj) + sizeof(struct Larray))
#define Larray_cval(array) (*((struct Larray *) LOBJ_DATA(array)))
#define Larray_size(array) (Larray_cval(array).size)
#define Larray_vals(array) (Larray_cval(array).vals)

Lobj *Lmake_array(unsigned int size);
Lobj *Lmake_array_no_copy(unsigned int size, Lobj **vals);
void Ldelete_array(Lobj *array);
Lobj *Larray_to_string(Lobj *array);
#define Larray_eval Lsimple_eval
Lobj *Larray_copy(Lobj *array);
Lobj *Larray_equals(Lobj *array1, Lobj *array2);

Lobj *Llist_to_array(Lobj *args);
Lobj *Larray_cat(Lobj *args);
Lobj *Larray_interval(Lobj *array, int start, int end);

#endif /* LPP_ARRAY_H */
