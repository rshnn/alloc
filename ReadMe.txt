

ROSHAN PATEL
198:415:  COMPILERS PROJECT 1


Programming language:	C

Only one source file named ‘alloc.c’

Usage to generate executable:  

	gcc alloc.c -o alloc


Example executable usage:	
	
	/.alloc 10 b block1.i


Creates and writes to 'stdout.i'.  Prints a confirmation statement if the program executes correctly.
Note:  For (seemingly) random cases, executing the executable (for example, inputting "./alloc 7 b block1.i") will result in a segmentation fault error.  I have not yet determined the cause of this.  However, if I just reexecute the command, it will run successfully and will produce correct ILOC instructions in the stdout.i file.



