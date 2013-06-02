"
build.lsp
  Builds the LPP system from the source.

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

(func 'is-non-c-file '(path ())
  "Returns whether the file is a C file."
  '(! (|| '(string-ends-with path ".h")
          '(string-ends-with path ".H")
	      '(string-ends-with path ".c")
	      '(string-ends-with path ".C"))))

(func 'build-all '(())
  "Builds the LPP system."
  '(set 'OSFLAG "WINDOWS") ; WINDOWS or STARNIX, for now...
  '(system "copy /Y source\\c_source working")
  '(system "copy /Y source\\other_files build")
  '(load "source/gen_lpp.lsp")
  '(set 'c-compiler-path (get-env "CC"))
  '(set 'success (== (system (cat c-compiler-path " -g working/lpp.c -o build/lpp.exe -D__" OSFLAG "__")) 0))
  '(set 'listing (list-dir "working"))
  '(set 'non-c-files (filter is-non-c-file (list-dir "working" ())))
  '(while 'non-c-files
    '(system (cat "copy /Y \"working\\" (first non-c-files) "\" build"))
    '(set 'non-c-files (rest non-c-files)))
  'success)

(build-all)
 
