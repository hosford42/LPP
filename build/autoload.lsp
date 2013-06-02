"
   autoload.lsp
     Standard lisp functions provided in the LPP runtime environment.
   
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

(set 'lpp-bin-path (cat lpp-path "/bin"))

(set 'current-project ())

(set 'mapcar
  (defun '(f l ())
    "Maps a 1-argument function to the items in a list, producing a list of return values."
    '(? l
        '(cons (f (car l)) (mapcar f (cdr l)))
        '())))

(set 'func
  (defun '(name args doc body)
    "Creates a new function."
    '(set name
      (defun args
             doc
             (? (> (list-length body) 1)
                '(cons do (mapcar quote body))
                '(? body
                    '(car body)
                    '()))))))

(func 'macro '(name args doc body)
  "Creates a new text macro."
  '(set name
    (defun args doc (cons cat body))))

(func 'apply '(f arglist ())
  "Applies a function to a list of arguments. Does not evaluate arguments."
  '(eval (cons f (mapcar quote arglist))))

(func 'second '(l ())
  "Returns the second member of a list."
  '(car (cdr l)))

(func 'third '(l ())
  "Returns the third member of a list."
  '(car (cdr (cdr l))))

(func 'fourth '(l ())
  "Returns the fourth member of a list."
  '(car (cdr (cdr (cdr l)))))
  
(func 'fifth '(l ())
  "Returns the fifth member of a list."
  '(car (cdr (cdr (cdr (cdr l))))))

(func 'sixth '(l ())
  "Returns the sixth member of a list."
  '(car (cdr (cdr (cdr (cdr (cdr l)))))))

(func 'seventh '(l ())
  "Returns the seventh member of a list."
  '(car (cdr (cdr (cdr (cdr (cdr (cdr l))))))))

(func 'eighth '(l ())
  "Returns the eighth member of a list."
  '(car (cdr (cdr (cdr (cdr (cdr (cdr (cdr l)))))))))

(func 'ninth '(l ())
  "Returns the ninth member of a list."
  '(car (cdr (cdr (cdr (cdr (cdr (cdr (cdr (cdr l))))))))))

"Needs to be fixed so that negative vals get nth-from-end instead of just nil."
(func 'nth '(n l ())
  "Returns the nth member of a list. Numbering starts at 1."
  '(? (> n 1)
      '(nth (+ n -1) (cdr l))
      '(? (< n 1)
          '()
          '(car l))))

(func 'list-cat '(list-of-lists)
  "Returns the concatenation of several lists."
  '(? list-of-lists
      '(? (first list-of-lists)
          '(cons (first (first list-of-lists))
                 (apply list-cat
                        (cons (rest (first list-of-lists))
                              (rest list-of-lists))))
          '(apply list-cat (rest list-of-lists)))
      '()))

(func 'list-reverse '(l ())
  "Returns a copy of the list with elements in the reverse order."
  '(local 'new-l
    '(set 'new-l ())
    '(while 'l
      '(set 'new-l (cons (car l) new-l))
      '(set 'l (cdr l)))
    'new-l))

(func '== '(l)
  "Returns t iff all arguments are equal, nil otherwise."
  '(or '(or '(equals l nil)
            '(equals (cdr l) nil))
       '(and '(equals (car l) (car (cdr l)))
             '(apply '== (cdr l)))))

(func '<= '(l)
  "Returns t iff all arguments are non-decreasing, nil otherwise. (Numeric arguments only.)"
  '(? (&& 'l '(cdr l))
      '(and '(or '(< (first l) (second l))
                 '(== (first l) (second l)))
            '(apply <= (cdr l)))
      't))

(func '>= '(l)
  "Returns t iff all arguments are non-increasing, nil otherwise. (Numeric arguments only.)"
  '(? (&& 'l '(cdr l))
      '(and '(or '(> (first l) (second l))
                 '(== (first l) (second l)))
            '(apply <= (cdr l)))
      't))

(func 'last '(l ())
  "Returns the last member of a list."
  '(while '(rest l)
    '(set 'l (rest l)))
  '(first l))

(func 'printl '(vals)
  "Prints its arguments separated with a space."
  '(while 'vals
    '(do
      '(prints (to-string (car vals)))
      '(set 'vals (cdr vals))
      '(if vals
        '(prints " ")))))

(func 'for '(i c s b ())
  "For loop. Arguments: (i c s b ()), where i is the initialization term,
c is the loop condition, s is the step statement, and b is the body."
  '(do
    i
    '(while c
      '(do
        b
        s))))

(func 'while '(test body)
  "While loop."
  '(#!(while) test
    '(apply do body)))

(func 'local '(var body)
  "Local scope."
  '(#!(local) var
    '(apply do body)))

(func 'sys-path '(())
  "Returns the current path."
  '(if working-path
       'working-path
       '(if project-path
            'project-path
            'lpp-path)))
          
(func 'lpp '(inf outf)
  "Preprocesses a text file and send the results to another file or stdout."
  '(? outf
     '(? (cdr outf)
         '(throw "Bad argument list to lpp: " (cons inf outf))
         '(preprocess (fopen inf "r") (fopen (car outf) "w")))
     '(preprocess (fopen inf "r"))))

(func 'lpp-append '(inf outf ())
  "Preprocess a text file, appending the results to the given output file."
  '(preprocess (fopen inf "r") (fopen outf "a")))

(func 'lppstr '(inf ())
  "Preprocess a text file and return a string containing the results."
  '(local 'lppstr-stream-string
    '(set 'lppstr-stream-string (copy ""))
    '(? (preprocess (fopen inf "r") (sopen lppstr-stream-string "w"))
        'lppstr-stream-string
        '())))

(func 'load '(inf p)
  "Loads a lisp file."
  '(load-file (fopen inf "r") (car p)))

(func 'string-to-list '(s ())
  "Converts a string to a list of characters."
  '(if (equals (strlen s) 0)
       '()
       '(cons (substr s 0 1) (string-to-list (substr s 1 (strlen s))))))

(func 'has-substring '(s substring ())
  "Returns whether or not the substring appears in the string."
  '(< (find-substring s substring) (strlen s)))

(func 'let '(letsym letval letbody)
  "Let the value of letsym be val for the duration of body."
  '(local letsym
          '(do '(set letsym letval)
               '(apply do letbody))))

(func 'init-array '(val-list)
  "Returns an array containing the arguments."
  '(list-to-array val-list))

(func 'set '(setsym setval setdoc-opt)
  "Set setsym to value setval, optionally setting setsym's documentation string to (car setdoc-opt)."
  '(? setdoc-opt
      '(? (|| '(cdr setdoc-opt) '(! (identical string (type-of (car setdoc-opt)))))
          '(throw "Bad argument list passed to 'set': " (cons setsym (cons setval setdoc-opt)))
          '(do
              '(#!(set) setsym setval)
              '(set-symbol-doc setsym (car setdoc-opt))
              'setval))
      '(#!(set) setsym setval)))

(func '- '(a b ())
  "Subtraction of two numeric values."
  '(? (&& '(== (type-of a) number) '(== (type-of b) number))
      '(+ a (negative b))
      '(throw "Cannot subtract non-numeric values: " (list a b))))

(func 'filter '(filter-func values ())
  "Filter out a list of values, returning only those that satisfy a truth function."
  '(? values
      '(? (filter-func (first values))
	      '(cons (first values) (filter filter-func (rest values)))
		  '(filter filter-func (rest values)))
	  '()))

(func 'string-ends-with '(s ending ())
  "Return whether the string has the given ending."
  '(&& '(>= (strlen s) (strlen ending))
       '(equals (substr s (- (strlen s) (strlen ending)) (strlen s)) ending)))

(func 'path-to-list '(path ())
  "Return a list of the elements of the path."
  '(set 'results ())
  '(set 'char-list (string-to-list path))
  '(set 'current "")
  '(while 'char-list
    '(set 'char (first char-list))
	'(? (|| '(== (char "/")) '(== (char "\\")))
	    '(do
		  '(? (> (strlen current) 0) '(set 'results (cons current results)))
		  '(set 'current ""))
		'(set 'current (cat current char)))
    '(set 'char-list (rest char-list)))
  '(? (> (strlen current) 0) '(set 'results (cons current results)))
  '(list-reverse results))

(func 'get-base-path '(path ())
  "Return the path, minus the last element."
  '(set 'reversed-path (list-reverse (path-to-list path)))
  '(set 'result "")
  '(? reversed-path
      '(set 'reversed-path (rest reversed-path)))
  '(while 'reversed-path
    '(set 'result (cat (first reversed-path) "/" result))
	'(set 'reversed-path (rest reversed-path)))
  'result)
	   
(func 'list-dir '(path add-path)
  "List the contents of a directory."
  '(set 'has-wildcard (|| '(has-substring path "*") '(has-substring path "?")))
  '(? (! has-wildcard)
      '(do
        '(? '(! (|| '(string-ends-with path "/") '(string-ends-with path "\\")))
            '(set 'path (cat path "/")))
	    '(set 'path (cat path "*"))))
  '(set 'no-dot (defun '(name ()) "" '(! (|| '(equals name ".") '(equals name "..")))))
  '(set 'results (filter no-dot (#!(list-dir) path)))
  '(? (&& add-path (car add-path))
      '(do
	    '(set 'dir-path (get-base-path path))
        '(set 'add-path-func (defun '(name ()) "" '(cat dir-path name)))
		'(mapcar add-path-func results))
	  'results))

;(load (cat lpp-bin-path "/pretty-string.lsp"))


; TODO: I haven't delved into why, but if the file ends with a comment, it complains about an unexpected end of file.
()
