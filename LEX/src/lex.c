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

#include <stdio.h> //load needed libraries
#include <stdlib.h> 
#include <string.h>

#define MAX_IDENT_LENGTH 11 //maximum letterrs in ideentifier
#define MAX_NUMBER_LENGTH 5 //max digits in number 
#define MAX_LEXEMES 1000 //how many total token can be stored in lexeme list

typedef enum { //declaration of Token Types
skipsym = 1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, fisym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym} token_type;

const char *reservedWords[] = { //program reserved words
  "const","var","procedure","call",
  "begin","end","if","then","else",
  "fi","while","do","read","write",
  NULL
};

const token_type reservedTokens[] = { //program reserved tokens
  constsym, varsym, procsym, callsym,
  beginsym, endsym, ifsym,  thensym, elsesym,
  fisym,   whilesym, dosym,   readsym, writesym
};

int main(int argc, char *argv[]){

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

	i=0 //true value
	j=1 //look ahead
	while(){
		
	}


}


