#*
#*************************************
#  Batch Number: 9
#  Amitayush Thakur 2012B4A7819P
#*************************************
#/

#Doing all steps in one go
all:
	make error.o
	make lexer.o
	make set.o
	make stack.o
	make parser.o
	make driver.o
	make ast.o
	make hash.o
	make symbolTable.o
	make codeGeneration.o
	make toycompiler

#removing the *.o files and executables
remove:
	rm *.o
	rm toycompiler

remove_all:
	make remove
	rm a.out
	rm *.asm
#linkning commands

toycompiler: error.o lexer.o stack.o set.o parser.o driver.o ast.o symbolTable.o hash.o codeGeneration.o
		   gcc error.o lexer.o stack.o set.o parser.o driver.o ast.o symbolTable.o hash.o codeGeneration.o -o toycompiler
		   
driver.o: driver.c
		gcc -c driver.c
	
parserExe: error.o lexer.o stack.o set.o parser.o 
		gcc error.o lexer.o stack.o set.o parser.o -o parser


#Compiling commands
parser.o: parser.c			
		gcc -c parser.c
		
set.o: set.c
	 gcc -c set.c

error.o: error.c
	 gcc -c error.c
	 
lexer.o: lexer.c
	   gcc -c lexer.c

stack.o: stack.c
	   gcc -c stack.c

ast.o: ast.c
	   gcc -c ast.c

hash.o: hash.c
		gcc -c hash.c

symbolTable.o: symbolTable.c
		gcc -c symbolTable.c

codeGeneration.o: codeGeneration.c
		gcc -c codeGeneration.c