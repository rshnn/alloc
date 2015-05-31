
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char allocator;
int totalVR;
int offsetCounter = 1024;
int totalInstr;


/* ~~~~~~~~~~~PHYSICAL REGISTER STRUCTURE~~~~~~~~~~~
 * 		Used for bottom up allocation.*/
typedef struct physicalRegisters{
	int size;
	int* Name;
	int* Next;
	int* Free;

	int* Stack;
	int StackTop;
}physicalRegisters;
physicalRegisters physicalReg;







/* ~~~~~~~~~~VIRTUAL REGISTER STRUCTURE~~~~~~~~~~~~~~.
 * Used to keep frequency count and calculate life range.*/


typedef struct intnode{
	int x;
	struct intnode *next;
}intnode;

intnode* add(intnode* head, int x){

	intnode* temp = (intnode *)malloc(sizeof(intnode));

	temp->x = x;
	temp->next = NULL;

	if(head != NULL){
		temp->next = head;
	}
	head = temp;

	return temp;

}


typedef struct reg{
	int spilled;
	int assigned;
	int frequency;
	int born;
	int die;
	int VR;
	int sorted;
	int offset;
	intnode* occurs;
}reg;
reg virtualReg[256];
reg sorted[256];


/* Initialize register structures in virtualReg[].
 *
 * Set born to -1 to detect virtual registers which exist.
 * Frequency = 0							*/
void intializeVRArray(){
	int i;
	for (i = 0;  i < 256 ; i++){
		virtualReg[i].born = -1;
		virtualReg[i].frequency = 0;
		virtualReg[i].VR = i;
		virtualReg[i].assigned = -1;
		virtualReg[i].sorted = -1;
		virtualReg[i].offset = -1000;
		virtualReg[i].occurs = NULL;
		virtualReg[i].spilled = 0;

	}
}

/* Prints contents of all reg's in virtualRegs
 * For testing purposes */
void printArray(reg* VRArray){

	int i;
	for (i=0;i<256;i++){

		if(VRArray[i].born > 0){
			printf("Virtual Register No. %i\n",VRArray[i].VR);
			printf("\tBorn: %i\n",VRArray[i].born);
			printf("\tDie: %i\n",VRArray[i].die);
			printf("\tFreq: %i\n",VRArray[i].frequency);
			printf("\tAssigned: %i\n",VRArray[i].assigned);
			printf("\tLifeRange: %i\n\n",VRArray[i].die-VRArray[i].born+1);

			printf("\tLive instructions: ");
			intnode *temp = VRArray[i].occurs;
						while(temp != NULL){

							printf("%i -> ",temp->x);

							temp = temp->next;
						}
						printf("\n");
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
		}

	}

}


/* Counts number of virtual registers for testing */
int countVR(){
	int i;
	int counter = 0;
	for (i=0;i<256;i++){
		if(virtualReg[i].born != -1){
			counter++;
		}


	}
	return counter;
}


/* Sort VR array by frequency for simple TD allocator. */
void sortbyFreq(){


	int i; int j; int a;
	sorted[0].frequency = 0;

	for (i = 0; i < 256; i++){


		for (j = 0; j < 256; j++){

			if(virtualReg[j].sorted == -1){

				if (sorted[i-1].frequency == virtualReg[j].frequency && i != 0){
					a = j;
					sorted[i] = virtualReg[j];
					break;
				}
				if (sorted[i].frequency < virtualReg[j].frequency){
					a = j;
					sorted[i] = virtualReg[j];
				}


			}
		}
		virtualReg[a].sorted = 1;


	}

}









/* ~~~~~~~~~~~INSTRUCTION LINKED LIST~~~~~~~~~~~~~~
 * Use for storing instruction parameters in order. */
typedef struct instructionNode{
	int number;
	char opcode[15];
	char sources[15];
	char target[15];

	struct instructionNode *next;
}instrNode;

struct instructionNode *head = NULL;
struct instructionNode *curr = NULL;

/* Create and add to a list link containing instruction information.
 * Head and curr initialized and used to add new entries.
 * Entries are added to curr->next (the tail).
 * The resulting data structure, when traversing from head to tail,
 * is in the same order as the input file. */
void create_list(int instrnum, char* token1, char* token2, char* token3){

	instrNode *temp = (instrNode*)malloc(sizeof(instrNode));

    temp->number = instrnum;
    strcpy(temp->opcode, token1);
    strcpy(temp->sources, token2);
    strcpy(temp->target, token3);

    temp->next = NULL;

    head = curr = temp;


}
void add_to_list(int instrnum, char* token1, char* token2, char* token3){

    if(NULL == head)
        return (create_list(instrnum, token1, token2, token3));

    instrNode *temp = (instrNode*)malloc(sizeof(instrNode));

	temp->number = instrnum;
	strcpy(temp->opcode, token1);
	strcpy(temp->sources, token2);
	strcpy(temp->target, token3);
	temp->next = NULL;

	curr->next = temp;
	curr = temp;
}

/*Prints each instruction stored in the linked list data structure in order.
 * For testing purposes
 * Note: This function loses head pointer*/
void printList(){

	instrNode* temp = head;
	int count = 1;
	while(temp->next != NULL){

			printf("\n\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
			printf("Instruction %i\n", count);
			printf("%i\t",temp->number);
			printf("%s\t",temp->opcode);
			printf("sources:%s\t",temp->sources);
			printf("target:%s\n",temp->target);



			temp = temp->next;
			count++;
		}
}











/* ~~~~~~~~~~~~~APPLICATION FUNCTIONS~~~~~~~~~~~~~~~~ */

/* Given an instruction, determines which virtual register to spill based on LifeRange */
int TDspillselect(int* ArrayRow){
	int i; int smallest = 10000; int smallestReg = -1;

	for(i=0;i<256;i++){

		if(ArrayRow[i] == 1){
			int y = virtualReg[i].die - virtualReg[i].born + 1;

			if(y < smallest){
				smallest = y;
				smallestReg = i;
			}


		}
	}

	return smallestReg;
}


/* Given an instruction, determines that MaxLive during the instruction */
int calculateMaxLive(int* ArrayRow){

	int x = 0; int i;

	for(i=0;i<256;i++){

		if(ArrayRow[i] == 1)
			x++;
	}


	return x;

}


/* Reads raw virtual register data and puts into VRarray.
 * Allocates data into virtual register array */
void processVR(char* input, int count){
	char* target[5];
	int x = -1;
	int y = -1;


	/* Ignore non-register inputs*/
	if(input[0] != 'r'){
		return;
	}

	/* Read first token.  Interpret as integer.
	 * Update virtual register array 			*/
	char* token = strtok(input, ",");

	memcpy( target, &token[1], 5 );
	x = atoi((char *)target);

	if(virtualReg[x].born == -1){
		virtualReg[x].born = count;
		virtualReg[x].die = count;
		virtualReg[x].frequency++;
		virtualReg[x].occurs = add(virtualReg[x].occurs,count);


	}else{
		virtualReg[x].frequency++;
		virtualReg[x].die = count-1;
		virtualReg[x].occurs = add(virtualReg[x].occurs,count);
	}



	/* Read second token if it exists. */
    token = strtok(NULL, "\t");
    while(token != NULL){

    	memcpy( target, &token[2], 5 );

    	y = atoi((char *)target);

    	if(virtualReg[y].born == -1){
    		virtualReg[y].born = count;
    		virtualReg[y].die = count;
   			virtualReg[y].frequency++;
   			virtualReg[y].occurs = add(virtualReg[y].occurs,count);
   		}else{
   			virtualReg[y].frequency++;
   			virtualReg[y].die = count-1;
   			virtualReg[y].occurs = add(virtualReg[y].occurs,count);
   		}
	    token = strtok(NULL, "\t");
    }

}


/* Reads raw instruction and puts into instruction linked list.
 * Stores opcode, sources, and target into LL
 * Sends raw register data (as strings) into processVR()*/
void buildInstruction(char* rawInstruction, int count){
	int length;
	char op[15];
	char source[15];
	char target[15];
	char almost[15];


	/* Use length of instruction to omit empty lines*/
	length = (int) strlen(rawInstruction);


	if(count == 0)
		return;



	/*Storing opcode*/
	char* token = strtok(rawInstruction, "\t");
	strcpy(op,token);
	token = strtok(NULL, "\t");

	/* Search for sources and target. */
	while(token != NULL){


		/* Storing target (source) of an Output instruction
		 * Must omit the \n symbol */
		if(!strncmp(op, "output",6)){
			memcpy( source, &token[0],15 );
			break;
		}


		/*Storing of target.
		 * Used 'almost' array to skip \n.  End of instruction*/
		if(token[0] == '='){
			memcpy( almost, &token[3], 4);

			int i; int j = 0;
			for( i=0;i<10;i++){
				if(isalpha(almost[i]) || isdigit(almost[i])){
					target[j] = almost[i];
					j++;
				}
			}
			break;
		}

		/*Storing the source.  Continue onto next part of token.*/

		strcpy(source,token);


		token = strtok(NULL, "\t");
	}

	add_to_list(count, op, source, target);

	processVR(source, count);

	if(!strncmp(op, "output",6)){
		/* Do not read target of an output instruction */
	}else{
		processVR(target, count);
	}


}


/*Open and parse through file.
 * Send each line that is not a comment into buildInstruction().*/
void openFile(char* filename){
	FILE *fp;
	char str[100];

	fp = fopen(filename,"r");
	int count = 1;


	if( fp == NULL ){
		  perror("Error while opening the file.\n");
		  exit(EXIT_FAILURE);
	}




   while( fgets(str, 99, fp) != NULL ){

	   /* Reject lines that are commented.*/
	   if( str[0] == '/'){
	     continue;
	   }



	   /* Reject lines that are only \n */
	   if(strlen(str) > 1){

		   buildInstruction(str, count);
		   count++;
	   }

   }
   fclose(fp);

   totalInstr = count-1;

   add_to_list(-1,"END","END","END");

}


/* Prints data from node to file unaltered.  For testing purposes. */
void writeInstruction(FILE* fp, instrNode* node){

	if(!strncmp(node->opcode, "output",6)){
		fputs("\t",fp);
		fputs(node->opcode,fp); fputs("\t",fp);

		char c; int i;
		for( i=0;i<15;i++){
			c = node->sources[i];
			if(isalpha(c) || isdigit(c)){
				fputc(c,fp);
			}
		}
		fputs("\n",fp);
		return;
	}


	fputs("\t",fp);
	fputs(node->opcode,fp); fputs("\t",fp);

	fputs(node->sources,fp); 	fputs("\t",fp);
	fputs("=> ",fp);

	char c; int i;
	for( i=0;i<15;i++){

		c = node->target[i];
		if(isalpha(c) || isdigit(c)){
			fputc(c,fp);
		}
	}

	fputs("\n",fp);

}


/* Checks if a VR is spilled for STD.  0 = Spilled */
int isSpilled(instrNode* thing, int targetVRnum){

	/* Gets integer value of target register (if not Output instruction) */
			if(strncmp(thing->opcode, "output",6)){


				/* Checks if target virtual register is assigned a physical register.*/
				int i;
				for(i = 0; i<totalVR;i++){
					if(targetVRnum == sorted[i].VR){
						if(sorted[i].assigned != -1){
							return 1;
							//THIS REGISTER IS NOT SPILLED
						}else{
							return 0;
							//THIS REGISTER IS SPILLED
						}
					}
				}
			}
			return -1;
}

int isSpilledTD(instrNode* thing, int targetVRnum){
		/* Gets integer value of target register (if not Output instruction) */
			if(strncmp(thing->opcode, "output",6)){


			//Checks if target virtual register is assigned a physical register.
			int i;
			for(i = 0; i<256;i++){
				if(targetVRnum == virtualReg[i].VR){
					if(virtualReg[i].assigned != -1){
						return 1;
						//THIS REGISTER IS NOT SPILLED
					}else{
						return 0;
						//THIS REGISTER IS SPILLED
					}
				}
			}
		}
		return -1;

}


/* Get target register number */
int getTargetRegNum(instrNode* thing){
		int targetVRnum;
		char temp[10];
		memcpy(temp,&thing->target[1],5);
		targetVRnum = atoi((char *)temp);

		return targetVRnum;
}


/* Get source register numbers.
 * 	Note:  Input is an preinitialized array that will update to include the numbers */
void getSourceRegNums(instrNode* thing, int* sourcenum){
	/* Get source register number(s) */
	char temp[12];

	if(thing->sources[0] == 'r'){
		char* token = strtok(thing->sources, ",");
		memcpy(temp,&thing->sources[1],10);
		sourcenum[0] = atoi((char *)temp);

		/*For 2nd source*/
		token = strtok(NULL, "\t");
		if(token != NULL){
			memcpy(temp,&token[2],10);
			sourcenum[1] = atoi((char *)temp);
		}
	}

}


/* Printing function for the simple top down allocator */
void printSTD(instrNode* thing, FILE *fp){


	/* Loop through every instruction in the Instructions Linked List */
	int i=1;
	while(thing->next != NULL){


			/* Find out if registers have been spilled or allocated to a PR. */
			int targetspill; int source1spill; int source2spill; int isStore = 0;
			int sourceVRnum[2] = {-1, -1};
			int targetVRnum = getTargetRegNum(thing);
			getSourceRegNums(thing, sourceVRnum);
			targetspill = isSpilled(thing, targetVRnum);
			source1spill = isSpilled(thing,sourceVRnum[0]);
			source2spill = isSpilled(thing,sourceVRnum[1]);

			if(!strncmp(thing->opcode, "store",5)){
				isStore = 1;
			}

			/* PRINTING INSTRUCTION
			 * 		1. First checking if source1 was spilled.
			 * 				If so, retrieve into r1.
			 * 				If not, print "r" + virtualReg[].assigned.
			 * 		2. Next checking if source2 was spilled.
			 * 				If so, retrieve into r2.
			 * 				Same process.
			 * 		3.  Print opcode and sources based on above.
			 * 		4.  Check if target needs to be spilled.
			 * 				If so, print r1 and add two lines to load into memory
			 * 				If not, same process as above.
			 */


			/* Source 1 was spilled and is a VR.  Retrieve from main memory into r1. */
			if(source1spill == 0 && thing->sources[0] == 'r'){

				int x = virtualReg[sourceVRnum[0]].offset;

				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",x-1020); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r1",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r1",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r1",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r1",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r1",fp);
				fputs("\n",fp);
			}

			/* Source 2 was spilled and is a VR.  Retrieve from main memory into r2. */
			if(source2spill == 0 && thing->sources[0] == 'r'){

				int x = virtualReg[sourceVRnum[1]].offset;


				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",x-1020); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r2",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r2",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);
			}


			/* SPECIAL CASE FOR STORE OPERATION */

			/* 		Both target and source are spilled.*/
			if(isStore == 1 && source1spill == 0 && targetspill == 0){

				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",virtualReg[targetVRnum].offset-1020);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r2", fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r2",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("store",fp); fputs("\t",fp);
				fputs("r1",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				thing = thing->next;
				continue;
			}

			/* 		Source is not spilled.  Target is spilled. */
			if( isStore == 1 && source1spill == 1 && targetspill == 0 ){

				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",virtualReg[targetVRnum].offset-1020);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r2",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r2",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("store",fp); fputs("\t",fp);
				fputs("r",fp); fprintf(fp,"%d",virtualReg[sourceVRnum[0]].assigned);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				thing = thing->next;
				continue;

			}


			/* PRINTING THE MAIN INSTRUCTION */
			fputs("\t",fp);
			fputs(thing->opcode,fp); fputs("\t",fp);


			/* Print first source. (Could be only source) */

				/* Spilled and source1 is a register */
			if(source1spill == 0 && thing->sources[0] == 'r'){
				fputs("r1",fp);
			}

				/* Not spilled and source has a physical register */
			else if(source1spill != 0 && thing->sources[0] == 'r'){
				fputs("r",fp); fprintf(fp,"%d",virtualReg[sourceVRnum[0]].assigned);
			}

				/* Not spilled and source is an integer */
			else if(source1spill == -1 && thing->sources[0] != 'r'){
				fputs(thing->sources,fp);
			}




			/* Print second source if applicable */
			if(source2spill == 0 && thing->sources[0] == 'r'){
				fputs(", r2",fp);
			}else if(source2spill == -1){
			}
			else if(source2spill != 0 && thing->sources[0] == 'r'){
				fputs(" ,r",fp); fprintf(fp,"%d",virtualReg[sourceVRnum[1]].assigned);
			}





			/* TARGET:
			 * Checking if opcode was an output.  If so, the print is complete. */
			if(strncmp(thing->opcode, "output",6)){
				fputs("\t=> ",fp);

				/* Printing target */
				if(targetspill == 0){
					fputs("r1",fp); fputs("\n",fp);

					if(virtualReg[targetVRnum].offset == -1000){
						offsetCounter = offsetCounter -4;
						virtualReg[targetVRnum].offset = offsetCounter;
					}

					fputs("\t",fp);
					fputs("loadI",fp); fputs("\t",fp);
					fprintf(fp,"%d",virtualReg[targetVRnum].offset-1020); fputs("\t",fp);
					fputs("=> ",fp);
					fputs("r2",fp);
					fputs("\n",fp);

					fputs("\t",fp);
					fputs("add",fp); fputs("\t",fp);
					fputs("r0, r2",fp); fputs("\t",fp);
					fputs("=> ",fp);
					fputs("r2",fp);
					fputs("\n",fp);

					fputs("\t",fp);
					fputs("store",fp); fputs("\t",fp);
					fputs("r1",fp); fputs("\t",fp);
					fputs("=> ",fp);
					fputs("r2",fp);
					fputs("\n",fp);

				}else if(targetspill == -1 && thing->sources[0] != 'r' ){
					fputs(thing->target,fp); fputs("\n",fp);
				}else{
					fputs("r",fp); fprintf(fp,"%d",virtualReg[targetVRnum].assigned); fputs("\n",fp);
				}

				thing = thing->next;
				i++;

			}else{
				thing = thing->next;
				continue;
			}

	}/* End while  loop */


}


/* Printing function for the top down allocator (Using LiveRanges) */
void printTD(instrNode* thing, FILE *fp){

	int i;
	while(thing->next != NULL){

		/* Mostly the same as STD printing since all the VRs are preallocated. */
		int targetspill; int source1spill; int source2spill; int isStore = 0;
		int sourceVRnum[2] = {-1, -1};

		int targetVRnum = getTargetRegNum(thing);
		getSourceRegNums(thing, sourceVRnum);

		targetspill = isSpilledTD(thing, targetVRnum);
		source1spill = isSpilledTD(thing,sourceVRnum[0]);
		source2spill = isSpilledTD(thing,sourceVRnum[1]);


		if(!strncmp(thing->opcode, "store",5)){
			isStore = 1;
		}

			if(source1spill == 0 && thing->sources[0] == 'r'){

				int x = virtualReg[sourceVRnum[0]].offset;

				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",x-1020); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r1",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r1",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r1",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r1",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r1",fp);
				fputs("\n",fp);
			}

			if(source2spill == 0 && thing->sources[0] == 'r'){

				int x = virtualReg[sourceVRnum[1]].offset;


				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",x-1020); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r2",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r2",fp); fputs("\t",fp);
				fputs("=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);
			}


			/* Special cases for store operation */

			 /* Both target and source are spilled.*/
			if(isStore == 1 && source1spill == 0 && targetspill == 0){

				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",virtualReg[targetVRnum].offset-1020);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r2", fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r2",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("store",fp); fputs("\t",fp);
				fputs("r1",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				thing = thing->next;
				continue;
			}/* Source is not spilled.  Target is spilled. */
			if( isStore == 1 && source1spill == 1 && targetspill == 0 ){

				fputs("\t",fp);
				fputs("loadI",fp); fputs("\t",fp);
				fprintf(fp,"%d",virtualReg[targetVRnum].offset-1020);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("add",fp); fputs("\t",fp);
				fputs("r0, r2", fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("load",fp); fputs("\t",fp);
				fputs("r2",fp);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				fputs("\t",fp);
				fputs("store",fp); fputs("\t",fp);
				fputs("r",fp); fprintf(fp,"%d",virtualReg[sourceVRnum[0]].assigned);
				fputs("\t=> ",fp);
				fputs("r2",fp);
				fputs("\n",fp);

				thing = thing->next;
				continue;

			}


			/* THE MAIN INSTRUCTION */
			fputs("\t",fp);
			fputs(thing->opcode,fp); fputs("\t",fp);


			/* Print first source. (Could be only source) */

			/* Spilled and source1 is a register */
			if(source1spill == 0 && thing->sources[0] == 'r'){
				fputs("r1",fp);
			}

			/* Not spilled and source has a physical register */
			else if(source1spill != 0 && thing->sources[0] == 'r'){
				fputs("r",fp); fprintf(fp,"%d",virtualReg[sourceVRnum[0]].assigned);
			}

			/* Not spilled and source is an integer */
			else if(source1spill == -1 && thing->sources[0] != 'r'){
				fputs(thing->sources,fp);
			}

			/* Print second source if applicable */
			if(source2spill == 0 && thing->sources[0] == 'r'){
				fputs(", r2",fp);
			}else if(source2spill == -1){
			}
			else if(source2spill != 0 && thing->sources[0] == 'r'){
				fputs(" ,r",fp); fprintf(fp,"%d",virtualReg[sourceVRnum[1]].assigned);
			}

			/* TARGET:
			 * Checking if opcode was an output.  If so, the print is complete. */
			if(strncmp(thing->opcode, "output",6)){
				fputs("\t=> ",fp);

				/* Print target */
				if(targetspill == 0){
					fputs("r1",fp); fputs("\n",fp);

					if(virtualReg[targetVRnum].offset == -1000){
						offsetCounter = offsetCounter -4;
						virtualReg[targetVRnum].offset = offsetCounter;
					}

					fputs("\t",fp);
					fputs("loadI",fp); fputs("\t",fp);
					fprintf(fp,"%d",virtualReg[targetVRnum].offset-1020); fputs("\t",fp);
					fputs("=> ",fp);
					fputs("r2",fp);
					fputs("\n",fp);

					fputs("\t",fp);
					fputs("add",fp); fputs("\t",fp);
					fputs("r0, r2",fp); fputs("\t",fp);
					fputs("=> ",fp);
					fputs("r2",fp);
					fputs("\n",fp);


					fputs("\t",fp);
					fputs("store",fp); fputs("\t",fp);
					fputs("r1",fp); fputs("\t",fp);
					fputs("=> ",fp);
					fputs("r2",fp);
					fputs("\n",fp);

				}else if(targetspill == -1 && thing->sources[0] != 'r' ){
					fputs(thing->target,fp); fputs("\n",fp);
				}else{
					fputs("r",fp); fprintf(fp,"%d",virtualReg[targetVRnum].assigned); fputs("\n",fp);
				}

				thing = thing->next;
				i++;

			}else{
				thing = thing->next;
				continue;
			}

	}/* End while  loop */

}


/* Updates the distance value in Next[physicalReg] for given vr */
void BUupdateNext(int vr, int instructionNum){

	int distance= 999;
	intnode* temp = virtualReg[vr].occurs;

	while(temp->next != NULL){
		if(temp->x <= instructionNum){
			break;
		}

		distance = temp->x;
		temp = temp->next;
	}

	int i;
	for(i=0;i<physicalReg.size;i++){
		if(physicalReg.Name[i] == vr){
			physicalReg.Next[i] = distance;
		}
	}

}


/* Will return a physical register from the Free list (if one exists).
 * 	Otherwise, it will select one to spill (based on distance) and reassign. */
int BUallocate(int vr, FILE* fp){

	int assigned = 0;
	if(physicalReg.StackTop >= 0){
		assigned = physicalReg.StackTop;

		/* Popping from the stack. This register is not available anymore. */
		physicalReg.Free[assigned] = 0;
		int i; int popsuccess = 0;
		for(i=0; i<physicalReg.size; i++){
			if(physicalReg.Free[i] == 1){
				physicalReg.StackTop = i;
				popsuccess = 1;
				break;
			}
		}
		if(popsuccess == 0){
			/* i.e. All physical registers are now full. */
			physicalReg.StackTop = -1;
		}

	}else{
		/*Umm...
		 * assigned = j where physicalReg[j].Next is the biggest.
		 * Must print code to store stuff in physicalReg[j] into main memory.*/

		/* Register with largest distance to next use. */
		int i; int maxDist = -10;
		for(i=0; i<physicalReg.size;i++){

			if(physicalReg.Next[i] > maxDist){
				maxDist = physicalReg.Next[i];
				assigned = i;
			}
		}

		/* SPILL CODE GOES HERE FOR physicalReg.Name[assigned] */
		int VRtospill = physicalReg.Name[assigned];

		/* This virtual register has not yet been spilled before.  ALlocate mainMem space for it */
		if(virtualReg[VRtospill].offset == -1000){
			offsetCounter = offsetCounter - 4;
			virtualReg[VRtospill].offset = offsetCounter;
		}

		fputs("\t",fp);
		fputs("loadI",fp); fputs("\t",fp);
		fprintf(fp,"%d",virtualReg[VRtospill].offset-1020); fputs("\t",fp);
		fputs("=> ",fp);
		fputs("r1",fp);
		fputs("\n",fp);

		fputs("\t",fp);
		fputs("add",fp); fputs("\t",fp);
		fputs("r0, r1",fp);
		fputs("\t=> ",fp);
		fputs("r1",fp);
		fputs("\n",fp);


		fputs("\t",fp);
		fputs("store",fp); fputs("\t",fp);
		fputs("r",fp);	fprintf(fp,"%d",assigned+2); fputs("\t",fp);
		fputs("=> ",fp);
		fputs("r1",fp);
		fputs("\n",fp);
	}

	physicalReg.Name[assigned] = vr;
	/* Note:  Change Next field to -1 to ensure that this register is not chosen for the other operand in the current operation */
	physicalReg.Next[assigned] = -1;
	physicalReg.Free[assigned] = 0;

	return assigned;
}


/* Will return a physical register number for the virtual register given.
 * 	Uses helper functions */
int BUensure(int vr, FILE* fp){

	/* Return the physical register number if vr already exists in one. */
	int i;
	for(i=0;i<physicalReg.size;i++){
		if(vr == physicalReg.Name[i]){
			return i;
		}

	}


	/* Allocate a physical register for vr otherwise. */
	int result = BUallocate(vr, fp);

	/* Retrieve it's data from mainMem if it was previously spilled. */
	if(virtualReg[vr].offset != -1000){

		fputs("\t",fp);
		fputs("loadI",fp); fputs("\t",fp);
		fprintf(fp,"%d",virtualReg[vr].offset-1020); fputs("\t",fp);
		fputs("=> ",fp);
		fputs("r1",fp);
		fputs("\n",fp);

		fputs("\t",fp);
		fputs("add",fp); fputs("\t",fp);
		fputs("r0, r1",fp);
		fputs("\t=> ",fp);
		fputs("r1",fp);
		fputs("\n",fp);

		fputs("\t",fp);
		fputs("load",fp); fputs("\t",fp);
		fputs("r1",fp);
		fputs("\t=> ",fp);
		fputs("r",fp); fprintf(fp,"%d",result+2);
		fputs("\n",fp);

	}

	return result;

}


/* Will push a physical register back onto the stack (i.e. It is available for allocation) */
void BUfree(int physR){
	physicalReg.Free[physR] = 1;
	physicalReg.Name[physR] = -10;
	physicalReg.Next[physR] = 1000;
	physicalReg.StackTop = physR;
}


/* Printing function for bottom-up allocator (uses dynamic allocation process). */
void printBU(instrNode* thing, FILE *fp){

	int instructionNum = 1;

	while(thing->next != NULL){
		int sourceVRnum[2] = {-1, -1}; int isStore = 0; int isOutput = 0;

		/* Get all register numbers as integers.  Detect store or output operations. */
		int targetVRnum = getTargetRegNum(thing);
		getSourceRegNums(thing, sourceVRnum);
		if(!strncmp(thing->opcode, "store",5))
			isStore = 1;
		if(!strncmp(thing->opcode, "output",6))
			isOutput = 1;


		/* For "op vr1 vr2 => vr3"  (Exclude output and store instructions.)*/
		if(isOutput == 0 && isStore == 0){
			int rx = -1;
			int ry = -1;
			int rz = -1;

			/* A:  Allocate physical registers for the sources. */
			if(sourceVRnum[0] != -1)
				rx = BUensure(sourceVRnum[0],fp);
			if(sourceVRnum[1] != -1)
				ry = BUensure(sourceVRnum[1],fp);



			/* B:  If either source dies, Free the physical register. */
			if(sourceVRnum[0] != -1 && virtualReg[sourceVRnum[0]].die < instructionNum){
				if(rx != -1){
					BUfree(rx);
				}
			}
			if(sourceVRnum[1] != -1 && virtualReg[sourceVRnum[1]].die < instructionNum){
				ry = BUensure(sourceVRnum[1], fp);
				if(ry != -1){
					BUfree(ry);
				}
			}
			/* C:  Allocate a physical register for the target. */
				rz = BUallocate(targetVRnum,fp);


			/* D:  Print the instruction. */

				fputs("\t",fp);
				fputs(thing->opcode,fp); fputs("\t",fp);
				if(rx != -1){
					fputs("r",fp); fprintf(fp,"%d",rx+2);
				}else{
					fputs(thing->sources,fp);
				}

				if(ry != -1){
					fputs(", r",fp); fprintf(fp,"%d",ry+2); fputs("\t",fp);}
				fputs("\t=> ",fp);
				fputs("r",fp); fprintf(fp,"%d",rz+2); fputs("\t",fp);
				fputs("\n",fp);


			/* E:  If sources are needed later (not dead this instruction), update Next[rx], Next[ry]. */
				if(virtualReg[sourceVRnum[0]].die-1 > instructionNum){
					BUupdateNext(sourceVRnum[0],instructionNum);
				}
				if(virtualReg[sourceVRnum[1]].die-1 > instructionNum){
					BUupdateNext(sourceVRnum[1],instructionNum);
				}

			/* F:  Update Next[rz] (for the target). **Must happen every iteration of this while loop. */
				BUupdateNext(targetVRnum,instructionNum);

		}


		/* For a store operation. "store rx => rz" */
		else if(isStore == 1){
			int rx = -1; int rz = -1;

			/* Allocate physical registers for source and target. */
			rx = BUensure(sourceVRnum[0],fp);
			rz = BUensure(targetVRnum,fp);

			/* Print the instruction. */
			fputs("\t",fp);
			fputs("store",fp); fputs("\t",fp);
			fputs("r",fp); fprintf(fp,"%d",rx+2); fputs("\t",fp);
			fputs("=> ",fp);
			fputs("r",fp); fprintf(fp,"%d",rz+2); fputs("\t",fp);
			fputs("\n",fp);


			/* Update distances in Next. */
			if(virtualReg[sourceVRnum[0]].die-1 > instructionNum){
				BUupdateNext(sourceVRnum[0],instructionNum);
			}

			if(virtualReg[targetVRnum].die-1 > instructionNum){
				BUupdateNext(targetVRnum,instructionNum);
			}

			/* Free the physical registers if they die this line. */
			if(sourceVRnum[0] != -1 && virtualReg[sourceVRnum[0]].die < instructionNum){
				if(rx != -1){
					BUfree(rx);
				}
			}

			if(targetVRnum != -1 && virtualReg[targetVRnum].die < instructionNum){
				if(rz != -1){
					BUfree(rz);
				}
			}





		}

		/* For an output operation.  Literally spit out the same instruction. */
		else if(isOutput == 1){
			fputs("\t",fp);
			fputs(thing->opcode,fp); fputs("\t",fp);
			fputs(thing->sources,fp); fputs("\n",fp);
		}


		instructionNum++;
		thing = thing->next;
	}

}


/* Creates and writes to stdout.i file.*/
void createFile(char* inputfile, int k){

	FILE *fp;
	fp = fopen("stdout.i","w");


	fputs("// ROSHAN PATEL\n", fp);
	fputs("// CS415: COMPILERS PROJECT 1\n", fp);
	fputs("// \t LOCAL REGISTER ALLOCATION\n", fp);
	fputs("// Output ILOC instructions for ", fp); fputs(inputfile,fp);	fputs("\n", fp);
	fputs("// Total physical register = ", fp); fprintf(fp, "%d", k); fputs("\n", fp);
	fputs("// Allocator type: ", fp); fputc(allocator,fp); fputs("\n\n", fp);


	/* Print loadI 1020 -> r0 */
	fputs("\t",fp);
	fputs("loadI",fp); fputs("\t",fp);
	fprintf(fp,"%d",1020); fputs("\t",fp);
	fputs("=> ",fp);
	fputs("r0",fp);
	fputs("\n",fp);

	instrNode* thing = head;

	if(allocator == 's'){
		printSTD(thing,fp);
		fclose(fp);
		return;
	}

	if(allocator == 't'){
		printTD(thing,fp);
		fclose(fp);
		return;
	}


	if(allocator == 'b'){
		printBU(thing,fp);
		fclose(fp);
	}


}


/* Initialize physicalReg data structure for printing for the bottom up allocator. */
void BottomUp(int k, char* inputfile){

	int size = k-2;

	/* Initialize physical register structure */
	physicalReg.size = size;
	physicalReg.Name = (int *)malloc(sizeof(int)*size);
	physicalReg.Next = (int *)malloc(sizeof(int)*size);
	physicalReg.Free = (int *)malloc(sizeof(int)*size);
	physicalReg.Stack = (int *)malloc(sizeof(int)*size);
	int i;
	for(i=0;i<size;i++){
		physicalReg.Name[i] = -10;
		physicalReg.Next[i] = 1000;
		physicalReg.Free[i] = 1;
		/* Stack holds the name of the physical register. */
		physicalReg.Stack[i] = i;
		physicalReg.StackTop = 0;
	}
	/* Done creating empty physical register structure. */

	createFile(inputfile,k);


}


int main(int argc, char *argv[]){

	/* Number of arguments must be 4.
		 * argv[1] = k*
		 *
		 * argv[2] = Allocator type
		 * 				s = Simple Top Down Allocator
		 * 				t = Top Down Allocator (from class)
		 * 				b = Bottom Up Allocator
		 *
		 * argv[3] = input file name (Ex.  block.i)
		 *
		 * We are assuming the feasible register set (F) to be 3.
		 * Therefore, the available physical register set is (k-F).
		 * */
	if ( argc != 4 ){
	        printf("Correct input example: alloc 5 b block.i");
     		printf("\n\t	argv[1] = k (Total number of physical registers)");
	  		printf("\n\t	argv[2] = Allocator type");
     		printf("\n\t\t	s = Simple Top Down Allocator");
	       	printf("\n\t\t	t = Top Down Allocator (from class)");
	        printf("\n\t\t	b = Bottom Up Allocator");
	        printf("\n\t	argv[3] = input file name (Ex.  block.i)\n" );
	}



	int k = atoi(argv[1]);
	char* input = argv[2];
	allocator = input[0];


	/* Initialize virtual register data structure, offset counter, ... */
	intializeVRArray();
	offsetCounter = 1024;
	totalInstr = 0;

	/* Read input file and collect data into data structures. */
	openFile(argv[3]);
	totalVR = countVR();


	/* For s and t, The Feasible array set will be r0, r1, and r2. 	*/
	/* 						Physical registers will be [r3, r(k-F) ]*/
	/* For b, The Feasible array set will be r0 and r1.				*/
	/* 						Physical registers will be [r2, r(k-F) ]*/


	/* SIMPLE TOP-DOWN ALLOCATOR CASE
	 * 	Use frequency values to determine which VRs will get physical registers.
	 * 	Total usable registers are (k-3)
	 * 	Sort VRs by frequency (sortbyFreq())
	 * 	The top (k-3) will get physical registers.
	 * 	The rest will be spilled.*/
	if(allocator == 's'){
		sortbyFreq();

		/* Copy assigned register values into node's data. */
		int i=3; int j = 0;
		while(k-1 >= i){
			int VR;
			VR = sorted[j].VR;
			sorted[j].assigned = i;
			virtualReg[VR].assigned = i;
			i++; j++;
		}


		createFile(argv[3], k);
	}


	/* THE TOP DOWN ALLOCATOR CASE
	 * 	Use Life Range as the factor which determines whether a register will be spilled. */
	if(allocator == 't'){

		int satisfied = 0;
		int spilled[totalVR];
		int spillcount = 0;


		/* Initialize liveArray to zero. */
		int liveArray[totalInstr][256];
		int i; int j;
		for(i=0; i < totalInstr; i++){
			for(j=0; j < 256; j++){
				liveArray[i][j] = 0;
			}
		}

		/* Create Live array */
		int x;
		for(i=0; i < 256 ;i++){
			x = virtualReg[i].born;
			if(x != -1){
				int die = virtualReg[i].die +1;
				while(x <= die){
					liveArray[x][virtualReg[i].VR] = 1;
					x++;
				}
			}
		}

		/*PRINTING LIVE ARRAY
		for(i=1; i < totalInstr; i++){

			printf("Instruction %i:  \t",i);

			for(j=1; j < 30; j++){
				printf("%i ",liveArray[i][j]);
			}
			printf("\n");

		}*/




		while(satisfied != 1){
		/* Finding MaxLive for each row. */
			for(i=1;i<totalInstr;i++){
				int x = calculateMaxLive(liveArray[i]);

				/* Make decision about which to register to spill. */
				if(x > k-3){
					int spillregister = TDspillselect(liveArray[i]);

					spilled[spillcount] = spillregister;
					spillcount++;

					int j;
					for(j=0;j<totalInstr;j++){
						liveArray[j][spillregister] = 0;
					}


				}
			}
			satisfied = 1;
		}

		/* Set spilled registers to spilled in virtualReg. */
		for(i=0;i<spillcount;i++){
			int VR = spilled[i];
			virtualReg[VR].spilled = 1;
		}

		/* Assign physical registers! */
		int available[k-3];
		for(i=0;i<k-3;i++){
			available[i] = 1;
		}

		for(i=1; i < totalInstr; i++){ //each instruction.

			for(j=0; j < 256; j++){ //each VR

				if(liveArray[i][j] == 1){
					/* Checking for birthday */
					if(virtualReg[j].born == i){
						/* Search for an available register.  a+3 is the assigned Reg. */
						int w;
						for(w=0;w<k-3;w++){
							if(available[w] == 1){
								int a = w;
								available[a] = 0;
								virtualReg[j].assigned = a+3;
								break;
							}
						}

					}

					/* Checking for death */
					if(virtualReg[j].die +1 == i){
						int bruh = virtualReg[j].assigned - 3;
						available[bruh] = 1;
					}
				}
			}
		}


		createFile(argv[3], k);
	}



	/* THE BOTTOM UP ALLOCATOR CASE
	 * 	Dynamically allocates physical registers to virtual registers by demand at current instruction. */
	if(allocator == 'b'){
		BottomUp(k, argv[3]);
	}



	printf("Local register allocation complete for %s.\nFile stored in stdout.i\n\n",argv[3]);
	return 0;
}















