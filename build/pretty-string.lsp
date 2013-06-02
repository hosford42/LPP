"
   pretty-string.lsp
     Functions provided to help generate 'pretty' string representations of LPP objects.
   
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

(set 'tab-spaces 2)

(func 'indention-string-helper '(ilevel tabstr ())
  "Returns a string containing ilevel repetitions of tabstr."
  '(let 'result ""
    '(while '(> ilevel 0)
      '(set 'result (cat result tabstr))
      '(set 'ilevel (- ilevel 1)))
    'result))

(func 'indention-string '(ilevel ())
  "Returns a string of (* ilevel tab-spaces) space characters (' ')."
  '(indention-string-helper ilevel " "))

(func 'pretty-string '(lobj ())
  "Returns a pretty string representation of a lisp expression."
  '(pretty-string-indented lobj 0))

(func 'pretty-string-indented '(lobj ilevel ())
  "Does the actual work of 'pretty-string'."
  '(cat (indention-string ilevel)
        (? (equals (type-of lobj) cons-cell)
          '(pretty-string-cons lobj ilevel)
          '(? (equals (type-of lobj) quote-cell)
            '(cat "'" (pretty-string-indented-remainder (quote-value lobj) (+ ilevel 1)))
            '($ lobj)))))

(func 'pretty-string-indented-remainder '(lobj ilevel ())
  "Helper function for 'pretty-string-*' functions."
  '(? (equals (type-of lobj) cons-cell)
    '(pretty-string-cons lobj ilevel)
    '(? (equals (type-of lobj) quote-cell)
      '(cat "'" (pretty-string-indented-remainder (quote-value lobj) (+ ilevel 1)))
      '($ lobj))))

"This version is replaced the one loaded from pretty-string.mlsp by music_autoload.lsp,
if mlpp is running instead of lpp."
(func 'pretty-string-cons '(lobj ilevel ())
  "Returns a pretty string representation of a lisp list."
  '(? lobj
    '(? (|| '(equals (type-of (first lobj)) cons-cell)
            '(equals (type-of (first lobj)) array))
      '(pretty-string-complex-list lobj ilevel)
      '(? (|| '(equals (type-of (first lobj)) function) '(equals (type-of (first lobj)) symbol))
        '(pretty-string-function-call lobj ilevel)
        '(pretty-string-simple-list lobj ilevel)))
    '"()"))

(func 'pretty-string-list-tail '(lobj ilevel ())
  "Helper function for 'pretty-string-*' functions."
  '(? lobj
    '(cat "\n"
          (indention-string ilevel)
          (pretty-string-indented-remainder (first lobj) ilevel)
          (pretty-string-list-tail (rest lobj) ilevel))
    '""))

(func 'pretty-string-complex-list '(lobj ilevel ())
  "Returns a pretty string representation of a complex lisp list."
  '(cat "("
        (pretty-string-list-tail lobj (+ ilevel tab-spaces))
        "\n"
        (indention-string ilevel)
        ")"))

(func 'pretty-string-function-call '(lobj ilevel ())
  "Returns a pretty string representation of a lisp function call."
  '(cat "("
        (pretty-string-indented-remainder (first lobj) ilevel)
        (pretty-string-list-tail (rest lobj) (+ ilevel tab-spaces))
        "\n"
        (indention-string ilevel)
        ")"))

(func 'pretty-string-simple-list '(lobj ilevel ())
  "Returns a pretty string representation of a simple lisp list."
  '(cat "("
        (pretty-string-indented-remainder (first lobj) ilevel)
        (pretty-string-list-tail (rest lobj) (+ ilevel 1))
        ")"))

'(func 'pretty-string-test '(lobj ())
  ""
  '(prints "\n\n")
  '(prints (pretty-string lobj))
  '(prints "\n\n")
  '())
