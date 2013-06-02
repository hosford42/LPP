@echo off
rem emergency_build.bat
rem   Use this if you don't have a working copy of lpp.exe to use
rem   for the normal build process. It builds lpp.exe from the existing
rem   pre-processed source files in .\working, assuming they are present,
rem   and doesn't try to do any pre-processing of source files itself.
rem   You will need to modify the path to point to your C compiler.
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

..\..\tcc\tcc.exe -g working\lpp.c -o build\lpp.exe -D__WINDOWS__
