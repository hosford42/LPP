"
lpp.lsp
  Defines functions used by lpp_funcs.lsp to define built-in LPP functions.

Copyright 2013 Aaron Hosford

Licensed under the Apache License, Version 2.0 (the 'License');
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an 'AS IS' BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"

(prints "\nLoading lpp.lsp\n")
(set 'lpp.lsp-loaded t)

(set 'lpp-functions ())

(func 'lpp-func '(c-name lpp-names doc-string args body ())
  "Defines a new lpp function.
WARNING: Do not use newlines in the docstring; its contents are placed into a
C string and C does not permit newlines inside strings."
  '(set 'lpp-functions (cons (list c-name lpp-names doc-string args body) lpp-functions)))
  
(func 'lpp-func-get-c-name '(fdesc ())
  "Gets the C-syntax name of an lpp function."
  '(first fdesc))

(func 'lpp-func-get-lpp-names '(fdesc ())
  "Gets the list of lpp names of an lpp function."
  '(second fdesc))

(func 'lpp-func-get-doc-string '(fdesc ())
  "Gets the documentation string of an lpp function."
  '(third fdesc))
  
(func 'lpp-func-get-args '(fdesc ())
  ""
  '(fourth fdesc))

(func 'lpp-func-get-body '(fdesc ())
  ""
  '(fifth fdesc))
  
(func 'lpp-func-get-arg-qual '(fdesc ())
  ""
  '(first (lpp-func-get-args fdesc)))
  
(func 'lpp-func-get-arg-types '(fdesc ())
  ""
  '(second (lpp-func-get-args fdesc)))
  
(func 'lpp-make-func-decl '(fdesc ())
  ""
  '(cat "\n/* " (lpp-func-get-doc-string fdesc) " */\n"
        "Lobj *LF" (lpp-func-get-c-name fdesc) "(Lobj *args);\n"))
  
(func 'lpp-make-func-decls '(())
  ""
  '(do
    '(set 'x lpp-functions)
    '(set 'out "")
    '(while 'x
      '(do
        '(set 'out (cat (lpp-make-func-decl (first x)) out))
        '(set 'x (rest x))))
    'out))
    
(func 'lpp-make-func-defn '(fdesc ())
  ""
  '(cat "/* " (lpp-func-get-doc-string fdesc) " */\n"
        "Lobj *LF" (lpp-func-get-c-name fdesc) "(Lobj *args) {\n"
          (lpp-make-arg-check fdesc)
          (eval (lpp-func-get-body fdesc))
        "\n}\n"))

(func 'lpp-make-func-defns '(())
  ""
  '(do
    '(set 'x lpp-functions)
    '(set 'out "")
    '(while 'x
      '(do
        '(set 'out (cat "\n" (lpp-make-func-defn (first x)) out))
        '(set 'x (rest x))))
    'out))
    
(func 'lpp-make-func-init '(fdesc ())
  ""
  '(do
    '(set 'y (rest (lpp-func-get-lpp-names fdesc)))
    '(set 'lmfi-out (cat "LFUNC_SETUP("
                         (lpp-func-get-c-name fdesc)
                         ", \"@"
                         (first (lpp-func-get-lpp-names fdesc))
                         "\", \""
                         (lpp-func-get-doc-string fdesc)
                         "\")"))
    '(while 'y
      '(do
        '(set 'lmfi-out (cat "LFUNC_ALSO("
                             lmfi-out
                             ", \""
                             (first y)
                             "\")"))
        '(set 'y (rest y))))
    '(cat "\n  " lmfi-out ";")))

(func 'lpp-make-func-inits '(())
  ""
  '(do
    '(set 'x lpp-functions)
    '(set 'out "")
    '(while 'x
      '(do
        '(set 'out (cat (lpp-make-func-init (first x)) out))
        '(set 'x (rest x))))
    'out))

; TODO: Add a description of the argument list.
; TODO: Complain about undocumented functions.
(func 'lpp-make-func-description '(fdesc ())
  ""
  '(local 'out ; TODO: I think symbols should be local by default, and we should have to access global scopes through intentional language.
    '(set 'aliases (lpp-func-get-lpp-names fdesc))
	'(set 'doc-string (lpp-func-get-doc-string fdesc))
	'(set 'out "")
	'(while 'aliases
	  '(set 'out (cat out (? (> (strlen out) 0) " " "") (first aliases)))
	  '(set 'aliases (rest aliases)))
	'(set 'out (cat out "\n\n  "))
	'(? (== (strlen doc-string) 0)
	    '(set 'doc-string "No documentation available."))
	'(set 'chars (string-to-list doc-string))
	'(while 'chars
	  '(? (== (first chars) "\n")
	      '(set 'out (cat out "\n  "))
		  '(set 'out (cat out (first chars))))
	  '(set 'chars (rest chars)))
	'(cat out "\n\n\n")))
	
; TODO: Sort them alphabetically.
(func 'lpp-make-func-descriptions '(())
  ""
  '(do
    '(set 'x lpp-functions)
	'(set 'out "")
	'(while 'x
	  '(do
	    '(set 'out (cat out (lpp-make-func-description (first x))))
		'(set 'x (rest x))))
    'out))
	
(func 'lpp-make-arg-check-helper '(args ())
  ""
  '(? (&& 'args '(first args) '(rest args))
      '(? (|| '(second args) '(rest (rest args)))
          '(cat "LT" (first args) ", " (lpp-make-arg-check-helper (rest args)))
          '(cat "LT" (first args)))
      '(? args
          '(? (first args)
              '(cat "LT" (first args))
              '(? (rest args)
                  '(lpp-make-arg-check-helper (rest args))
                  ""))
          "")))
          
(func 'lpp-make-arg-check-minargs '(args ())
  ""
  '(? (&& 'args '(car args) '(cdr args))
      '(+ 1 (lpp-make-arg-check-minargs (cdr args)))
      0))

(func 'lpp-make-arg-check-maxargs '(args ())
  ""
  '(? args
      '(? (car args)
          '(+ 1 (lpp-make-arg-check-maxargs (cdr args)))
          '(? (cdr args)
              '(lpp-make-arg-check-maxargs (cdr args))
              0))))      

(func 'lpp-make-arg-check '(fdesc ())
  ""
  '(cat "  Lerror *errval = Lcheck_args(args, "
        ($ (lpp-make-arg-check-minargs (lpp-func-get-arg-types fdesc)))
        ", "
        ($ (? (last (lpp-func-get-arg-types fdesc))
              -1
              '(lpp-make-arg-check-maxargs (lpp-func-get-arg-types fdesc))))
        (? (first (lpp-func-get-arg-types fdesc))
           '(cat ", " (lpp-make-arg-check-helper (lpp-func-get-arg-types fdesc)))
           "")           
        ");\n  if(errval->level"
        (eval (lpp-func-get-arg-qual fdesc))
        ") {
    /* ERROR */
	LMfree(errval);
    return Lmake_exception(Lmake_string(\"Bad argument list passed to @"
      (first (lpp-func-get-lpp-names fdesc))
      ": \"), args);
  }
  LMfree(errval);
"))
  
(func 'lpp-func-getarg-helper '(n ())
  ""
  '(? (> n 0)
      '(cat "Lcons_cdr(" (lpp-func-getarg-helper (+ n -1)) ")")
      "args"))
    
(func 'lpp-func-getarg '(n ())
  ""
  '(cat "Lcons_car(" (lpp-func-getarg-helper (+ n -1)) ")"))

(func 'lpp-func-getarg-else-nil '(n ())
  ""
  '(cat "(" (lpp-func-is-arg n) " ? " (lpp-func-getarg n) " : Lnil)"))

(func 'lpp-func-arg-is-nil-or '(n t ())
  ""
  '(cat "(!(" (lpp-func-is-arg n) " && " (lpp-func-getarg n) ") || ("(lpp-func-getarg n) "->Ltype == LT" t "))"))

(func 'lpp-func-rest-args '(())
  ""
  '(lpp-func-getarg-helper 1))

(func 'lpp-func-args-after '(n ())
  ""
  '(lpp-func-getarg-helper n))

(func 'lpp-func-next-arg '(())
  ""
  '(cat "(args = " (lpp-func-rest-args) ", " (lpp-func-getarg 1) ")"))

(func 'lpp-func-more-args '(())
  ""
  "(!!args)")
  
(func 'lpp-func-arglist '(())
  ""
  "(args)")
  
(func 'lpp-func-protect-args '(())
  ""
  "Lcapture(args);\n")

(func 'lpp-func-unprotect-args '(())
  ""
  "Lrelease_no_delete(args);\n")
  
(func 'lpp-func-is-arg '(n ())
  "Does optional argument n exist? Arguments are known to exist up to and including n - 1."
  '(lpp-func-getarg-helper (+ n -1)))
  
(func 'lpp-func-return '(exp ())
  ""
  '(cat "return " exp ";\n"))

(prints "\nSuccessfully loaded lpp.lsp\n")
