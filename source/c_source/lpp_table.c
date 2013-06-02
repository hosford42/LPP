/*
   lpp_table.c
     The LPP symbol table.
   
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

#ifndef LPP_TABLE_C
#define LPP_TABLE_C

#include "lpp_table.h"

Lobj *Llookup_symbol(const char *name) {
  struct table_member *member = table;
    
  if(!name) {
    printf("\nERROR: Null symbol name passed to Llookup_symbol\n");
    printf("\nReturning nil\n");
    return Lnil;
  }
  
  if(!member) {
    LMalloc(table, sizeof(struct table_member));
    LINIT(table->symbol, Lmake_symbol(Lmake_string(name), Lnil, Lnil));
    table->left = 0;
    table->right = 0;
    return table->symbol;
  }
  
  struct table_member *parent;
  int dir;
  do {
    parent = member;
    dir =  strcmp(Lstring_cval(Lsymbol_name(member->symbol)), name);
    if(dir == 0)
      return member->symbol;
    else if(dir < 0)
      member = member->right;
    else
      member = member->left;
  } while(member);
  
  LMalloc(member, sizeof(struct table_member));
  if(dir < 0)
    parent->right = member;
  else
    parent->left = member;
  LINIT(member->symbol, Lmake_symbol(Lmake_string(name), Lnil, Lnil));
  member->left = 0;
  member->right = 0;

  return member->symbol;
}

void Lprint_table(const char *min, const char *max) {
  Lprint_table_helper(table, min, max);
    
  printf("\n");
}

void Lprint_table_helper(struct table_member *member, const char *min, const char *max) {
  if(member) {
    int meets_min = !min || (strcmp(Lstring_cval(Lsymbol_name(member->symbol)), min) >= 0);
    int meets_max = !max || (strcmp(Lstring_cval(Lsymbol_name(member->symbol)), max) <= 0);
    
    if(meets_min)
      Lprint_table_helper(member->left, min, meets_max ? 0 : max);
    else if(member->left)
      Lprint_table_helper(member->left->right, min, meets_max ? 0 : max);
    
    if(meets_min && meets_max) {
      printf("\n\"");
      Lprints(Lsymbol_name(member->symbol));
      printf("\" ");
      Lprinto(Lsymbol_vals(member->symbol));
    }

    if(meets_max)
      Lprint_table_helper(member->right, meets_min ? 0 : min, max);
    else if(member->right)
      Lprint_table_helper(member->right->left, min, meets_max ? 0 : max);
  }
}

#endif /* LPP_TABLE_C */
