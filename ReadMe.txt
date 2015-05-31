

ROSHAN PATEL
198:415:  COMPILERS PROJECT 1


Back end compiler tool.  Will convert ILOC code into equivalent ILOC code utilizing a finite number of physical registers.
Three allocation algorithms available:
	Simple top down allocation
	Top down implementation with consideration of virtual register life range
	Bottom up allocation



Programming language:	C

Only one source file named ‘alloc.c’

Usage to generate executable:  

	gcc alloc.c -o alloc


Example executable usage:	
	
	/.alloc 10 b block1.i


Creates and writes to 'stdout.i'.  Prints a confirmation statement if the program executes correctly.



