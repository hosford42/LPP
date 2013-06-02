/*
   lpp_util.c
     Utilities commonly used throughout LPP.
   
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

#ifndef LPP_UTIL_C
#define LPP_UTIL_C

void LMalloc_safe(void **ptr, unsigned int size) {
  if(*ptr != 0) {
    printf("\n\nERROR: Non-null pointer passed to LMalloc_safe\n\n");
    *ptr = 0;
  }
  if(!size) {
    printf("\n\nERROR: Zero-size allocation in LMalloc_safe\n\n");
  } else {
    *ptr = (void *) malloc(size);
    if(!*ptr) {
      printf("\n\nUNRECOVERABLE ERROR: Insufficient memory\n\n");
      exit(1);
    }
  }
}

void LMrealloc_safe(void **ptr, unsigned int size) {
  if(!*ptr) {
    printf("\n\nERROR: Null pointer passed to LMrealloc_safe\n\n");
    LMalloc_safe(ptr, size);
  } else {
    if(!size) {
      printf("\n\nERROR: Zero-size allocation in LMrealloc_safe\n\n");
      LMfree_safe(ptr);
    } else {
      *ptr = (void *) realloc(*ptr, size);
      if(!*ptr) {
        printf("\n\nUNRECOVERABLE ERROR: Insufficient memory\n\n");
        exit(1);
      }
    }
  }
}

void LMfree_safe(void **ptr) {
  if(!*ptr) {
    printf("\n\nERROR: Null pointer passed to LMfree_safe\n\n");
  } else {
    free(*ptr);
    *ptr = 0;
  }
}

char *Lstrcat_safe(char **a1, char *a2) {
  if(!a2) {
    printf("\n\nERROR: Null pointer passed to Lstrcat_safe\n\n");
    return *a1;
  } else {
    LMrealloc(*a1, strlen(*a1) + strlen(a2) + 1);
    return (char *) strcat(*a1, a2);
  }
}

#endif /* LPP_UTIL_C */
