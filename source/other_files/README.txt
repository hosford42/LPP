README.txt
  A general explanation of the LPP system and its setup/use.

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




Setup
-----

Define a system environment variable named 'LPPPATH'. Set it to the location of your binary (most likely lpp.exe), and place autoload.lsp, platform.lsp, and settings.lsp in the same directory.
If you would like to use a gcc/tcc-compatible C compiler with LPP, define a system environment variable named 'CC' that points to the compiler's .exe.

AUTOMATICALLY LOADED FILES:

autoload.lsp
  This file contains function definitions and other initialization code that is required every time lpp loads in order to function properly. It contains no platform-specific code, but may access such code.

platform.lsp
  This file contains code that is platform specific. It should only be altered when porting to a new system. Delete nothing from this file; it is standardized and user code may break if anything goes missing.

settings.lsp
  This file contains setup code specific to each installation or user. It may be altered freely.


What's in a name?
-----------------
LPP stands for Lisp PreProcessor. LPP was originally designed for (but is not
restricted to) machine generation of source files in any programming language
using file templates. Since LPP includes an interpreter for a computationally
complete dialect of Lisp, it can be used as a resource for many other purposes.
It is my hope that not only will it serve as a convenient means for generating
templated, machine generated text files, but as a full fledged scripting
language, and eventually as an operating system shell. Other plans I have in
the works include a theorem prover built on LPP syntax, an algorithmic music
generation platform that sits on top of CSound, a Genetic Programming library,
and a library for providing back-end scripting support in other programs, much
as Python is frequently used. Until then, though, at the very least it's a
small and fast scripting and content-generation tool.


How is this thing used?
-----------------------

The LPP interpreter operates in two distinct modes: Lisp mode and LPP mode. LPP
mode is the default. Use the -l flag to indicate Lisp mode. In LPP mode, the
source file is treated as a template. Text appearing in the source file is
written verbatim to the output, until a special escape sequence is encountered.
There are several different escape sequences, all of which start with the #
character:

  #;    Indicates parenthesized code that is to be executed when it is
        encountered, but whose return value is to be disregarded; everything up
		to and including the closing parenthesis is not written to the output.

  #$    Indicates parenthesized code that is to be executed when it is
        encountered. The return value is converted to a string, if necessary,
		and injected into the output.
		
  ##	Writes a single # to the output.

After an escape sequence is processed, processing resumes with the next
character in the source file, until the entire source file has been processed.

In Lisp mode, the source file is treated as an ordinary Lisp source file, and
the expressions therein are evaluated in the order of their appearance.




