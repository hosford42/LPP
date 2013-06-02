/*
   lpp_table.h
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

#ifndef LPP_TABLE_H
#define LPP_TABLE_H

struct table_member {
  Lobj *symbol;
  struct table_member *left;
  struct table_member *right;
};

struct table_member *table = 0;

Lobj *Llookup_symbol(const char *name);

void Lprint_table();

void Lprint_table_helper(struct table_member *member, const char *min, const char *max);

#endif /* LPP_TABLE_H */
