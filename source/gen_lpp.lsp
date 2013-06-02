"
gen_lpp.lsp
  Processes the .lpp templates in the source/template folder to
  produce templated LPP source files.

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

(prints "\nLoading gen_lpp.lsp\n")
(set 'gen_lpp.lsp-loaded t)

(|| 'version-string
  '(&& '(load "make/version.lsp") 'version-string)
  '(prints "WARNING: Build version not specified! Defaults to \"unknown\"."))
(? version-string
  '(prints "\n\nBuilding: LPP Version " version-string "\n\n"))

(load "source/lpp.lsp")
(load "source/lpp_funcs.lsp")

(set 'lpp-files  (list-dir "source/templates/*.lpp" ()))
(while 'lpp-files
  '(set 'file-name (first lpp-files))
  '(set 'len (strlen file-name))
  '(prints (cat "Processing " file-name "\n"))
  '(lpp (cat "source/templates/" file-name) (cat "working/" (substr file-name 0 (- len 4))))
  '(set 'lpp-files (rest lpp-files)))

(prints "\nSuccessfully loaded gen_lpp.lsp\n")

()
  
  