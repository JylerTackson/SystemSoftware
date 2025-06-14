//THIS WILL BE THE SOURCE CODE FILE WE TURN IN
//------------------------------
//GROUP:
//	David T. Jackson
//	5298477
//	Santiago H. R
//	#PID
//------------------------------
//ASSIGNMENT 2:
//	Implement a lexical analyzer for the programming lanague PL/0.
//		- Read Source program written in PL/0
//		- Identify Errors
//	Output:
//		- Source Program
//		- Source Program Lexeme Table
//		- Token List
//------------------------------
//CONSTRAINTS:
//	Input:
//	- Identifiers can be max of 11 chars
//	- Numbers can be max of 5 digits
//	- Comments should be ignored
//	- Invisible Chars should be ignored
//
//	Outputs:
//	- Token seperator can be space OR |
//	- Identifiers must show the token and the var
//	- In token list, numbers must show the token and value
//		- Value must be transformed to ASCII
//	- Consistent output, we will use spaces
//	- Token representation will be used in the Parser
//------------------------------
//ERRORS:
//	- Numbers too long
//	- Name too long
//	- Invalid Symbols
//Error detected, an error message is printed and the scanner continues

//includes
#include "../include/lex.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//restrictions
#define MAX_IDENT_LENGTH 11 //max identifier length
#define MAX_NUMBER_LENGTH 5 //max digits length
#define MAX_LEXEMES 1000 // total token length

//globals

//driver function
int main(int argc, char *argv[]){

	//Read in and Validate File
	//--------------------------------------
	//validate an input file was inserted
	if(argc < 2){
		printf("Usage: %s <input file>\n", argv[0]);
		return 1;
	}

	//Open File
	FILE *fp = fopen(argv[1], "r");
	if(!fp){
		printf("Error: Cannot open file %s\n", argv[1]);
		return 1;
	}
	//--------------------------------------


	int curr;	//true value
	int lookAhead; 	//lookAhead value

	printFile(fp)

	//main loop that will read the file and assign the token id's
	while((curr = fgetc(fp)) != EOF){
		lookAhead = fgetc(fp);

		//skip white space
		if(whitespace(curr)){
		
		}
		
		//skip comments
		if(1){
		
		}


	}//end while
}//end main



//print the whole file
void printFile(FILE *fp){
	int charptr;
	while((charptr = fgetc(fp))!=EOF){
		printf("%c", charptr);
	}//end while
}//end printFile



//returns 1 if the parsed in character is white space
int whitespace(int curr){
	
}
