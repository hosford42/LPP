/*
   lpp_array.c
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

#ifndef LPP_ARRAY_C
#define LPP_ARRAY_C

#include "lpp_array.h"
#include "lpp_types.h"
#include "lpp_ops.h"
#include "lpp_string.h"

Lobj *Lmake_array(unsigned int size) {
  Lobj **vals = 0;

  if(size) {
    LMalloc(vals, size * sizeof(Lobj *));

    int index;
    for(index = 0; index < size; index++)
      vals[index] = Lnil;
  }

  return Lmake_array_no_copy(size, vals);
}

Lobj *Lmake_array_no_copy(unsigned int size, Lobj **vals) {
  Lobj *new_array = 0;
  LMalloc(new_array, LTARRAY_SIZE);

  new_array->Ltype = LTARRAY;
  new_array->Lused = 0;

  Larray_size(new_array) = size;
  Larray_vals(new_array) = vals;

  return new_array;
}

void Ldelete_array(Lobj *array) {
  if(Larray_size(array)) {
    unsigned int index;
    for(index = 0; index < Larray_size(array); index++)
      LSET(Larray_vals(array)[index], Lnil);

    LMfree(Larray_vals(array));
    Larray_vals(array) = 0;
    Larray_size(array) = 0;
  }


  LMfree(array);
}

Lobj *Larray_to_string(Lobj *array) {
  int i = 0; /* array index */
  int l = 10; /* length of array */
  char *buff = 0; /* array */
  LMalloc(buff, l + 10);
  
  buff[i++] = '[';
  buff[i++] = 0;

  unsigned int index = 0;
  while(index < Larray_size(array)) {
    Lobj *s = Lnil; /* string rep. of Larray_vals(array)[index] */
    LSET(s, Lto_string(Larray_vals(array)[index]));
    
    i+= strlen(Lstring_cval(s));
    if(i >= l) { /* Make sure there's enough room in buff for sizeof(cars) + a few chars */
      while(i >= l)
        l*= 2;
      if(!(buff = (char *) realloc(buff, l + 10))) {
        printf("\nUNRECOVERABLE ERROR: Insufficient memory\n");
        exit(1);
      }
    }
    strcat(buff, Lstring_cval(s));
    
    Lrelease(s);
    
    index++;

    if(index < Larray_size(array)) {
      strcat(buff, " ");
      i++;
    }
  }

  strcat(buff, "]");
  
  return Lmake_string_no_copy(buff);
}

Lobj *Larray_copy(Lobj *array) {
  Lobj **vals = 0;

  if(Larray_size(array)) {
    LMalloc(vals, Larray_size(array) * sizeof(Lobj *));
    
    unsigned int index;
    for(index = 0; index < Larray_size(array); index++)
      LINIT(Larray_vals(array)[index], Lnil);
  }

  return Lmake_array_no_copy(Larray_size(Larray_size(array)), vals);
}

Lobj *Larray_equals(Lobj *array1, Lobj *array2) {
  if(Larray_size(array1) != Larray_size(array2))
    return Lnil;
  
  unsigned int index;
  for(index = 0; index < Larray_size(array1); index++)
    if(!Lequals(Larray_vals(array1)[index], Larray_vals(array2)[index]))
      return Lnil;

  return Lt;
}

Lobj *Llist_to_array(Lobj *args) {
  unsigned int size = Llist_length(args);

  Lobj **vals = 0;

  if(size) {
    LMalloc(vals, size * sizeof(Lobj *));
  
    unsigned int index = 0;
    while(args) {
      if(args->Ltype == LTCONS) {
        LINIT(vals[index], Lcons_car(args));
        index++;
        args = Lcons_cdr(args);
      } else {
        LINIT(vals[index], args);
        index++;
        args = Lnil;
      }
    }
  }

  return Lmake_array_no_copy(size, vals);
}


Lobj *Larray_cat(Lobj *args) {
  unsigned int size = 0;
  
  Lobj *iter = args;
  while(iter) {
    size+= Larray_size(Lcons_car(iter));
    iter = Lcons_cdr(iter);
  }

  if(!size)
    return Lmake_array(0);

  Lobj **vals = 0;
  LMalloc(vals, size * sizeof(Lobj *));

  unsigned int running_index = 0;
  while(args) {
    unsigned int index;
    for(index = 0; index < Larray_size(Lcons_car(args)); index++, running_index++)
      LINIT(vals[running_index], Larray_vals(Lcons_car(args))[index]);

    args = Lcons_cdr(args);
  }

  return Lmake_array_no_copy(size, vals);
}

Lobj *Larray_interval(Lobj *array, int start, int end) {
  if(Larray_size(array) == 0)
    return Lmake_array(0);

  while(start < 0)
    start+= Larray_size(array);

  while(start >= Larray_size(array))
    start-= Larray_size(array);
  
  while(end < 0)
    end+= Larray_size(array);

  while(end >= Larray_size(array))
    end-= Larray_size(array);
      
  if(end < start) {
    int temp = end;
    end = start;
    start = temp;
  }
  
  unsigned int size = end - start;
  Lobj **buff = 0;
  LMalloc(buff, size * sizeof(Lobj *));
  
  int index;
  for(index = 0; index < size; index++)
    LINIT(buff[index], Larray_vals(array)[index + start]);

  return Lmake_array_no_copy(size, buff);
}

#endif /* LPP_ARRAY_C */
