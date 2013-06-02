/*
   lpp_util.h
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

#ifndef LPP_UTIL_H
#define LPP_UTIL_H

/* Error if *ptr not null */
void LMalloc_safe(void **ptr, unsigned int size);
#define LMalloc(ptr, size) LMalloc_safe((void **) &(ptr), (size))

/* Error if *ptr null */
void LMrealloc_safe(void **ptr, unsigned int size);
#define LMrealloc(ptr, size) LMrealloc_safe((void **) &(ptr), (size))

/* Error if *ptr null */
void LMfree_safe(void **ptr);
#define LMfree(ptr) LMfree_safe((void **) &(ptr))

/* Expands array *a1 if there's insufficient room for a2 */
char *Lstrcat_safe(char **a1, char *a2);
#define Lstrcat(a1, a2) Lstrcat_safe(&(a1), (a2))

#endif /* LPP_UTIL_H */
