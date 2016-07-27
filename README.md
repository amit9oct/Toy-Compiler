# Toy-Compiler

========================
Author: Amitayush Thakur
=========================
Use: Anyone who wants to learn how to design a compiler for simple language can use this as a tool. It has lexer, parser and code generator everything written in C language. It doesn't use any external libraries. Also for asm it uses nasm.

==============
Instructions
==============
1. Making the repository:
	###Use $> make or $> make all

  	This will make all the files and create toycompiler in the main folder as well as bin folder.

2. Compiling the files with code:
	###Use  $> toycompiler `<`path of source file`>` `<`path where you want to create asm file`>`
	
	This will create a file asm file which can later be executed using nasm.
	
