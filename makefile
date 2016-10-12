#*
#*************************************
#  Batch Number: 9
#  Amitayush Thakur 2012B4A7819P
#*************************************
#/

#Doing all steps in one go
all:
	mkdir bin
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

#removing the *.o files executables in bin
remove:
	rm -rf bin
	rm ./toycompiler

remove_all:
	make remove
	rm bin/a.out
	rm bin/*.asm
#linkning commands

toycompiler: bin/error.o bin/lexer.o bin/stack.o bin/set.o bin/parser.o bin/driver.o bin/ast.o bin/symbolTable.o bin/hash.o bin/codeGeneration.o
		   gcc bin/error.o bin/lexer.o bin/stack.o bin/set.o bin/parser.o bin/driver.o bin/ast.o bin/symbolTable.o bin/hash.o bin/codeGeneration.o -o bin/toycompiler
		   cp bin/toycompiler .
		   
driver.o: src/driver.c
		gcc -c src/driver.c
		mv driver.o bin/driver.o
	
parserExe: bin/error.o bin/lexer.o bin/stack.o bin/set.o bin/parser.o 
		gcc bin/error.o bin/lexer.o bin/stack.o bin/set.o bin/parser.o -o bin/parser


#Compiling commands
parser.o: src/parser.c			
		gcc -c src/parser.c
		mv parser.o bin/parser.o
		
set.o: src/set.c
	 gcc -c src/set.c
	 mv set.o bin/set.o

error.o: src/error.c
	 gcc -c src/error.c
	 mv error.o bin/error.o

lexer.o: src/lexer.c
	   gcc -c src/lexer.c
	   mv lexer.o bin/lexer.o

stack.o: src/stack.c
	   gcc -c src/stack.c
	   mv stack.o bin/stack.o

ast.o: src/ast.c
	   gcc -c src/ast.c
	   mv ast.o bin/ast.o

hash.o: src/hash.c
		gcc -c src/hash.c
		mv hash.o bin/hash.o

symbolTable.o: src/symbolTable.c
		gcc -c src/symbolTable.c
		mv symbolTable.o bin/symbolTable.o

codeGeneration.o: src/codeGeneration.c
		gcc -c src/codeGeneration.c
		mv codeGeneration.o bin/codeGeneration.o
