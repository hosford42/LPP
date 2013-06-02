/*
   lpp_top.c
     The top-level LPP interpreter procedures.
   
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

#ifndef LPP_TOP_C
#define LPP_TOP_C

int LTlisp(const char *inf) {
  Lstrm *input;
  
  if(inf) {
    if(!(input = Lfopen(0, inf, "r"))) {
      printf("\nERROR: Unable to open input file: %s\n", inf);
      return 0;
    }
  } else
    input = Lstdin;
  
  Lobj *ret = Lnil;
  LSET(ret, LPparse_multiple_exp(input, Linput_prompt, Leval_and_print));
  LSET(ret, Lnil);
  
  if(inf) {
    if(Lclose(input)) {
      printf("\nERROR: Unable to close input file: %s\n", inf);
      return 0;
    }
  }
  
  return 1;
}

int LTlpp(const char *inf, const char *outf) {
  Lstrm *input;
  Lstrm *output;
  
  if(inf) {
    if(!(input = Lfopen(0, inf, "r"))) {
      printf("\nERROR: Unable to open input file: %s\n", inf);
      return 0;
    }
  } else
    input = Lstdin;
    
  if(outf) {
    if(!(output = Lfopen(0, outf, "w"))) {
      printf("\nERROR: Unable to open output file: %s\n", outf);
      if(inf && Lclose(input))
        printf("ERROR: Unable to close input file: %s\n", inf);
      return 0;
    }
  } else
    output = Lstdout;
  
  Lobj *parser_return = Lnil;
  LSET(parser_return, LPparse_lpp_file(input, output));
  if(parser_return && (parser_return->Ltype == LTEXCEPTION))
    Lprinto(parser_return);
  else
    LSET(parser_return, Lnil);

  if(outf && Lclose(output)) {
    printf("\nERROR: Unable to close output file: %s\n", outf);
    if(inf && Lclose(input))
      printf("ERROR: Unable to close input file: %s\n", inf);
    return 0;
  }
  
  if(inf && Lclose(input)) {
    printf("\nERROR: Unable to close input file: %s\n", inf);
    return 0;
  }
  
  return 1;
}

#endif /* LPP_TOP_C */
