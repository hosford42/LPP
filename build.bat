@echo off
rem build.bat
rem   A convenience to automatically run the build process.
rem   You will need to modify the CC path to point to your C compiler.
rem 
rem Copyright 2013 Aaron Hosford
rem 
rem Licensed under the Apache License, Version 2.0 (the 'License');
rem you may not use this file except in compliance with the License.
rem You may obtain a copy of the License at
rem 
rem    http://www.apache.org/licenses/LICENSE-2.0
rem 
rem Unless required by applicable law or agreed to in writing, software
rem distributed under the License is distributed on an 'AS IS' BASIS,
rem WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
rem See the License for the specific language governing permissions and
rem limitations under the License.

set LPPPATH=.\bin
set CC=..\..\tcc\tcc.exe
bin\lpp.exe -l -i make\clean.lsp
bin\lpp.exe -l -i make\build.lsp

pause
