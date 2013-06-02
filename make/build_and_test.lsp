"
   build_and_test.lsp
     Builds the LPP system from the source, and then runs the newly build
	 executable for testing purposes.
   
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

(set 'success (load "make/build.lsp"))

(? success
   '(system "build\\lpp.exe")
   '(prints "\n\nCompilation failed.\n"))

