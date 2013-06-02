/*
   lpp.c
     Gathers all the LPP code into a single file, in lieu of a decent make utility.
   
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

#ifndef LPP_C
#define LPP_C

#include "lpp.h"

#include "lpp_util.c"
#include "lpp_charstr_util.c"

#include "lpp_symbol.c"
#include "lpp_number.c"
#include "lpp_string.c"
#include "lpp_cons.c"
#include "lpp_quote.c"
#include "lpp_func.c"
#include "lpp_lambda.c"
#include "lpp_stream.c"
#include "lpp_type_token.c"
#include "lpp_exception.c"
#include "lpp_ud_object.c"
#include "lpp_ud_class.c"
#include "lpp_array.c"

#include "lpp_ops.c"
#include "lpp_func_defn.c"
#include "lpp_table.c"
#include "lpp_error.c"
#include "lpp_lexer.c"
#include "lpp_parser.c"
#include "lpp_init.c"
#include "lpp_top.c"

#include <stdlib.h>

/* MAIN */
int main(int argc, char *argv[]) {
  const char Lusage_str[] =
    "Options:\n\
\t'l':\tTreat input as lisp source, not lpp source\n\
\t'o':\tUse following file for output\n\
\t'i':\tUse following file for input\n\
\t'g':\tUse following file for logging\n\n";

  int as_lisp = -1; /* Should the input file be treated as lisp source or lpp source */
  char *inf = 0; /* Input file -- null means use stdin */
  char *outf = 0; /* Output file -- null means use stdout */
  char *stdoutf = 0; /* If not null, set stdout to this file */

  int a;
  for(a = 1; a < argc; a++) {
    if(argv[a][0] == '-') {
      switch(argv[a][1]) {
        case 'l':
          as_lisp = 1;
          break;
          
        case 'p':
          as_lisp = 0;
          break;
          
        case 'o':
          if(++a >= argc) {
            printf("%s", Lusage_str);
            exit(1);
          }
          outf = argv[a];
          break;
        
        case 'i':
          if(++a >= argc) {
            printf("%s", Lusage_str);
            exit(1);
          }
          inf = argv[a];
          break;
          
        case 'g':
          if(++a >= argc) {
            printf("%s", Lusage_str);
            exit(1);
          }
          stdoutf = argv[a];
          break;
          
        default:
          printf("%s", Lusage_str);
          exit(1);
      }
    } else {
      printf("%s", Lusage_str);
      exit(1);
    }
  }
  
  if(as_lisp == -1)
    as_lisp = inf ? 0 : 1; /* Default is lisp syntax for stdin, lpp syntax otherwise */

  if(stdoutf && !(freopen(stdoutf, "w", stdout))) {
    printf("\nERROR: Unable to open stdout file: %s\n", stdoutf);
    return 1;
  }

  Linit();
  Linit_strms();
  
  int interactive = inf ? 0 : 1;
  
  Lquiet_mode = 1;
  Linput_report = interactive; /* Default is input reporting only in user-interactive mode. */
  
  char temp[150];

  if(interactive) {
    printf("\nLisp PreProcessor v%s\nCopyright Aaron Hosford 2013\nApache License, Version 2.0\n\n", lpp_version);
    printf("Loading \"autoload.lsp\"...");
  }
  
  strcpy(temp, bin_dir);
  strcat(temp, "/autoload.lsp");
  int outcome = LTlisp(temp);
  
  if(!outcome) {
    printf("\n\nError while loading autoload.lsp!\n\n");
	return 1;
  } else if(interactive) {
    printf("done.\n");
    printf("Loading \"platform.lsp\"...");
  }
  
  strcpy(temp, bin_dir);
  strcat(temp, "/platform.lsp");
  outcome = LTlisp(temp);
  
  if(!outcome) {
    printf("\n\nError while loading platform.lsp!\n\n");
	return 1;
  } else if(interactive) {
    printf("done.\n");
    printf("Loading \"settings.lsp\"...");
  }
  
  strcpy(temp, bin_dir);
  strcat(temp, "/settings.lsp");
  outcome = LTlisp(temp);

  if(!outcome) {
    printf("\n\nError while loading settings.lsp!\n\n");
	return 1;
  } else if(interactive) {
    printf("done.\n");
  }
  
  Lquiet_mode = !Linput_report;
  
  return !(as_lisp ?
             LTlisp(inf) :
             LTlpp(inf, outf));
}

#endif /* LPP_C */
