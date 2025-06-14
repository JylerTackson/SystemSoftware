// THIS WILL BE THE SOURCE CODE FILE WE TURN IN
//------------------------------
// GROUP:
//	David T. Jackson
//	5298477
//	Santiago H. R
//	#PID
//------------------------------
// ASSIGNMENT 2:
//	Implement a lexical analyzer for the programming lanague PL/0.
//		- Read Source program written in PL/0
//		- Identify Errors
//	Output:
//		- Source Program
//		- Source Program Lexeme Table
//		- Token List
//------------------------------
// CONSTRAINTS:
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
// ERRORS:
//	- Numbers too long
//	- Name too long
//	- Invalid Symbols
// Error detected, an error message is printed and the scanner continues

// Load Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define Constants
#define MAX_IDENT_LENGTH 11 // maximum letters in identifier
#define MAX_NUMBER_LENGTH 5 // max digits in number
#define MAX_LEXEMES 1000	// how many total token can be stored in lexeme list

// Function Prototypes
void printFile(FILE *fp);
void printLexemeTable(struct LexemeEntry lex[], int count);
void printTokenList(struct TokenEntry tokens[], int count);
int getToken(const char *L);

typedef enum
{ // declaration of Token Types
	skipsym = 1,
	identsym = 2,
	numbersym = 3,
	plussym = 4,
	minussym = 5,
	multsym = 6,
	slashsym = 7,
	fisym = 8,
	eqsym = 9,
	neqsym = 10,
	lessym = 11,
	leqsym = 12,
	gtrsym = 13,
	geqsym = 14,
	lparentsym = 15,
	rparentsym = 16,
	commasym = 17,
	semicolonsym = 18,
	periodsym = 19,
	becomessym = 20,
	beginsym = 21,
	endsym = 22,
	ifsym = 23,
	thensym = 24,
	whilesym = 25,
	dosym = 26,
	callsym = 27,
	constsym = 28,
	varsym = 29,
	procsym = 30,
	writesym = 31,
	readsym = 32,
	elsesym = 33
} token_type;

// Program reserved words
const char *reservedWords[] = {
	"const", "var", "procedure", "call",
	"begin", "end", "if", "then", "else",
	"fi", "while", "do", "read", "write",
	NULL};

// Program reserved tokens
const token_type reservedTokens[] = {
	constsym, varsym, procsym, callsym,
	beginsym, endsym, ifsym, thensym, elsesym,
	fisym, whilesym, dosym, readsym, writesym};

// Lexeme Entry Structure
typedef struct
{
	char lexeme[MAX_IDENT_LENGTH + MAX_NUMBER_LENGTH];
	int tokenNumber;
	bool isError;
	char errorMessage[50];
} LexemeEntry;

int main(int argc, char *argv[])
{
	LexemeEntry lexemeTable[MAX_LEXEMES]; // Lexeme Table
	int lexemeCount = 0;				  // Count of lexemes

	// Validate and Open Input File
	//-------------------------------------------
	//  validate an input file was inserted
	if (argc != 2)
	{
		printf("Usage: %s <input file>\n", argv[0]);
		return 1;
	}

	// Open File
	FILE *fp = fopen(argv[1], "r");
	if (!fp)
	{
		printf("Error: Cannot open file %s\n", argv[1]);
		return 1;
	}
	//--------------------------------------------

	printFile(fp); // print the source file

	int curr;	   // true value
	int lookAhead; // look ahead
	while ((curr = fgetc(fp)) != EOF)
	{
		// Skip invisible characters
		if (curr == ' ' || curr == '\n' || curr == '\t')
		{
			continue; // skip invisible characters
		} // End Skip invisible characters

		// Check for comments
		if (curr == '/')
		{
			lookAhead = fgetc(fp);
			if (lookAhead == '*') // start of comment
			{
				while ((curr = fgetc(fp)) != EOF)
				{
					if (curr == '*')
					{
						lookAhead = fgetc(fp);
						if (lookAhead == '/') // end of comment
						{
							break; // exit comment
						}
					}
				}
				continue; // skip the comment
			}
			else
			{
				ungetc(lookAhead, fp); // put back lookahead if not a comment
			}
		} // End Check for comments

		// Check for operators and symbols
		switch (curr)
		{
		case '+':

			// Add plus symbol to table
			lexemeTable[lexemeCount].tokenNumber = plussym;	   // set token number for plus
			strcpy(lexemeTable[lexemeCount].lexeme, "+");	   // set lexeme
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
			lexemeCount++;									   // increment lexeme count
			break;

		case '-':

			// Handle minus operator
			lexemeTable[lexemeCount].tokenNumber = minussym;   // set token number for minus
			strcpy(lexemeTable[lexemeCount].lexeme, "-");	   // set lexeme
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
			lexemeCount++;									   // increment lexeme count

			break;
		case '*':

			// Handle multiplication operator
			lexemeTable[lexemeCount].tokenNumber = multsym;	   // set token number for multiplication
			strcpy(lexemeTable[lexemeCount].lexeme, "*");	   // set lexeme
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
			lexemeCount++;									   // increment lexeme count

			break;
		case '/':

			// Handle division operator
			lexemeTable[lexemeCount].tokenNumber = divsym;	   // set token number for division
			strcpy(lexemeTable[lexemeCount].lexeme, "/");	   // set lexeme
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
			lexemeCount++;									   // increment lexeme count

			break;
		case ':':
			// Handle assignment operator
			lookAhead = fgetc(fp); // look ahead for '='

			if (lookAhead == '=')
			{
				// If '=' found, it's an assignment operator
				lexemeTable[lexemeCount].tokenNumber = becomessym; // set token number for assignment
				strcpy(lexemeTable[lexemeCount].lexeme, ":=");	   // set lexeme
				lexemeTable[lexemeCount].isError = false;		   // no error
				strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
				lexemeCount++;									   // increment lexeme count
			}
			else
			{
				ungetc(lookAhead, fp); // put back lookahead if not '='
			}

			break;
		case '<':
			// Handle less than operator
			lookAhead = fgetc(fp); // look ahead for '=' or '>'

			if (lookahead == '=')
			{
				// If '=' found, it's a less than or equal operator
				lexemeTable[lexemeCount].tokenNumber = leqsym;	   // set token number for less than or equal
				strcpy(lexemeTable[lexemeCount].lexeme, "<=");	   // set lexeme
				lexemeTable[lexemeCount].isError = false;		   // no error
				strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
				lexemeCount++;									   // increment lexeme count
			}
			else if (lookAhead == '>')
			{
				// If '>' found, it's a not equal operator
				lexemeTable[lexemeCount].tokenNumber = neqSym;	   // set token number for not equal
				strcpy(lexemeTable[lexemeCount].lexeme, "<>");	   // set lexeme
				lexemeTable[lexemeCount].isError = false;		   // no error
				strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
				lexemeCount++;									   // increment lexeme count
			}
			else
			{
				ungetc(lookAhead, fp);							   // put back lookahead if not '=' or '>'
				lexemeTable[lexemeCount].tokenNumber = lessym;	   // set token number for less than
				strcpy(lexemeTable[lexemeCount].lexeme, "<");	   // set lexeme
				lexemeTable[lexemeCount].isError = false;		   // no error
				strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
				lexemeCount++;									   // increment lexeme count
			}

			break;
		case '>':
			// Handle greater than operator
			lookAhead = fgetc(fp); // look ahead for '='

			if (lookahead == '=')
			{
				// If '=' found, it's a greater than or equal operator
				lexemeTable[lexemeCount].tokenNumber = geqsym;	   // set token number for greater than or equal
				strcpy(lexemeTable[lexemeCount].lexeme, ">=");	   // set lexeme
				lexemeTable[lexemeCount].isError = false;		   // no error
				strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
				lexemeCount++;									   // increment lexeme count
			}
			else
			{
				ungetc(lookAhead, fp);							   // put back lookahead if not '='
				lexemeTable[lexemeCount].tokenNumber = gtrsym;	   // set token number for greater than
				strcpy(lexemeTable[lexemeCount].lexeme, ">");	   // set lexeme
				lexemeTable[lexemeCount].isError = false;		   // no error
				strcpy(lexemeTable[lexemeCount].errorMessage, ""); // no error message
				lexemeCount++;									   // increment lexeme count
			}

			break;
		default:
			// Handle invalid character
			lexemeTable[lexemeCount].isError = true;							// mark as error
			lexemeTable[lexemeCount].tokenNumber = 0;							// no token number
			strcpy(lexemeTable[lexemeCount].errorMessage, "Invalid character"); // set error message
			lexemeTable[lexemeCount].lexeme[0] = curr;							// set lexeme to the invalid character
			break;
		} // end switch - end check for operators and symbols
	} // end while
	fclose(fp); // close the file
} // end main

void printFile(FILE *fp)
{
	// Function to print the source file
	char ch;
	while ((ch = fgetc(fp)) != EOF)
	{
		putchar(ch); // print each character
	}
}

// Function to return correct token of reserved word
int getToken(const char *L)
{ // function to return correct token of reserved word
	for (int i = 0; reservedWords[i] != NULL; i++)
	{
		if (strcmp(L, reservedWords[i]) == 0)
		{
			return reservedTokens[i];
		}
	}
	return 0;
}

// Function to print the lexeme table
void printLexemeTable(LexemeEntry lex[], int count)
{
	printf("\nLexeme Table:\n\n");
	printf("lexeme\t\ttoken type\n");
	for (int i = 0; i < count; i++)
	{
		if (lex[i].isError)
		{
			printf("%s\t%s\n", lex[i].lexeme, lex[i].errorMessage); // if contains error print message
		}
		else
		{
			printf("%s\t%d\n", lex[i].lexeme, lex[i].tokenNumber); // if no error print token number
		}
	}
}

// Function to print the token list
void printTokenList(LexemeEntry lex[], int count)
{
	printf("\nToken List:\n");
	for (int i = 0; i < count; i++)
	{
		if (i > 0)
			printf(" ");
		if (lex[i].tokenNumber == identsym || lex[i].tokenNumber == numbersym)
		{
			printf("%d %s", lex[i].tokenNumber, lex[i].lexeme);
		}
		else
		{
			printf("%d", lex[i].tokenNumber);
		}
	}
}