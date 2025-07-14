// THIS WILL BE THE SOURCE CODE FILE WE TURN IN
//------------------------------
// GROUP:
//	David T. Jackson
//	5298477
//	Santiago Henao Rojas
//	5122384
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
#include <ctype.h>

// Define Constants
#define MAX_IDENT_LENGTH 11 // maximum letters in identifier
#define MAX_NUMBER_LENGTH 5 // max digits in number
#define MAX_LEXEMES 1000	// how many total token can be stored in lexeme list

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

// Function Prototypes
void printFile(FILE *fp);
void printLexemeTable(LexemeEntry lex[], int count);
void printTokenList(LexemeEntry lex[], int count);
int getToken(const char *L);

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
	rewind(fp);	   // return pointer back to beginning of file

	int curr;	   // true value
	int lookAhead; // look ahead
	while ((curr = fgetc(fp)) != EOF)
	{

		// printf("\nEnter While Loop\n");
		if (lexemeCount >= MAX_LEXEMES)
		{
			printf("\nError: Lexeme table overflow, max %d lexemes allowed.\n", MAX_LEXEMES);
			break; // exit if lexeme table is full
		}

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

		// Check for identifiers
		if (isalpha(curr))
		{
			char buffer[MAX_IDENT_LENGTH + 1];
			int len = 0;
			// collect letters and digits
			do
			{
				if (len < MAX_IDENT_LENGTH)
				{
					buffer[len++] = curr; // if length is smaller than 11 save into buffer
				}
				else
				{
					len++; // count for overflow and do not store into buffer
				}
				curr = fgetc(fp); // read next character
			} while (isalnum(curr)); // continue do-while loop
			ungetc(curr, fp); // push back into stream if non-alnum

			if (len > MAX_IDENT_LENGTH)
			{
				buffer[MAX_IDENT_LENGTH] = '\0'; // truncate buffer
			}
			else
			{
				buffer[len] = '\0';
			}

			if (len > MAX_IDENT_LENGTH)
			{
				// ident name too long error
				lexemeTable[lexemeCount].isError = true;
				strcpy(lexemeTable[lexemeCount].errorMessage, "Error: name too long");
			}
			else
			{
				int tok = getToken(buffer);
				if (tok != 0)
				{
					lexemeTable[lexemeCount].tokenNumber = tok; // save reserved word in lexeme table
				}
				else
				{
					lexemeTable[lexemeCount].tokenNumber = identsym; // save user identifier in lexeme table
				}
				strcpy(lexemeTable[lexemeCount].lexeme, buffer);
				lexemeTable[lexemeCount].isError = false;
				lexemeTable[lexemeCount].errorMessage[0] = '\0';
			}
			lexemeCount++;
			continue;
		}

		// Check for numbers
		if (isdigit(curr))
		{
			char buffer[MAX_IDENT_LENGTH + 1];
			int len = 0;
			// collect digits
			do
			{
				if (len < MAX_NUMBER_LENGTH)
				{
					buffer[len++] = curr; // save into buffer if less than 5
				}
				else
				{
					len++; // count for overflow
				}
				curr = fgetc(fp);
			} while (isdigit(curr));
			ungetc(curr, fp); // push back any non-digit

			if (len > MAX_NUMBER_LENGTH)
			{
				buffer[MAX_NUMBER_LENGTH] = '\0'; // truncate buffer
			}
			else
			{
				buffer[len] = '\0';
			}

			if (len > MAX_NUMBER_LENGTH)
			{
				// number too long error
				lexemeTable[lexemeCount].isError = true; //
				strcpy(lexemeTable[lexemeCount].errorMessage, "Error: number too long");
			}
			else
			{
				lexemeTable[lexemeCount].tokenNumber = numbersym; // save token number
				strcpy(lexemeTable[lexemeCount].lexeme, buffer);  // copy buffer into lexeme index
				lexemeTable[lexemeCount].isError = false;
				lexemeTable[lexemeCount].errorMessage[0] = '\0';
			}
			lexemeCount++;
			continue;
		}

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
			lexemeTable[lexemeCount].tokenNumber = slashsym;   // set token number for division
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

			if (lookAhead == '=')
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
				lexemeTable[lexemeCount].tokenNumber = neqsym;	   // set token number for not equal
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

			if (lookAhead == '=')
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

		case '=':
			// Handle equality operator
			lexemeTable[lexemeCount].tokenNumber = eqsym;	   // set token number for “=”
			strcpy(lexemeTable[lexemeCount].lexeme, "=");	   // set lexeme to “=”
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // clear error message
			lexemeCount++;									   // increment lexeme count
			break;

		case '(':
			// Handle left parenthesis
			lexemeTable[lexemeCount].tokenNumber = lparentsym; // set token number for “(”
			strcpy(lexemeTable[lexemeCount].lexeme, "(");	   // set lexeme to “(”
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // clear error message
			lexemeCount++;									   // increment lexeme count
			break;

		case ')':
			// Handle right parenthesis
			lexemeTable[lexemeCount].tokenNumber = rparentsym; // set token number for “)”
			strcpy(lexemeTable[lexemeCount].lexeme, ")");	   // set lexeme to “)”
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // clear error message
			lexemeCount++;									   // increment lexeme count
			break;

		case ',':
			// Handle comma symbol
			lexemeTable[lexemeCount].tokenNumber = commasym;   // set token number for “,”
			strcpy(lexemeTable[lexemeCount].lexeme, ",");	   // set lexeme to “,”
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // clear error message
			lexemeCount++;									   // increment lexeme count
			break;

		case ';':
			// Handle semicolon symbol
			lexemeTable[lexemeCount].tokenNumber = semicolonsym; // set token number for “;”
			strcpy(lexemeTable[lexemeCount].lexeme, ";");		 // set lexeme to “;”
			lexemeTable[lexemeCount].isError = false;			 // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, "");	 // clear error message
			lexemeCount++;										 // increment lexeme count
			break;

		case '.':
			// Handle period symbol
			lexemeTable[lexemeCount].tokenNumber = periodsym;  // set token number for “.”
			strcpy(lexemeTable[lexemeCount].lexeme, ".");	   // set lexeme to “.”
			lexemeTable[lexemeCount].isError = false;		   // no error
			strcpy(lexemeTable[lexemeCount].errorMessage, ""); // clear error message
			lexemeCount++;									   // increment lexeme count
			break;

		default:
			// Handle invalid character
			lexemeTable[lexemeCount].isError = true;							// mark as error
			lexemeTable[lexemeCount].tokenNumber = 0;							// no token number
			strcpy(lexemeTable[lexemeCount].errorMessage, "Invalid character"); // set error message
			lexemeTable[lexemeCount].lexeme[0] = curr;							// set lexeme to the invalid character
			lexemeCount++;														// increment lexeme count

			break;
		} // end switch - end check for operators and symbols

	} // end while
	fclose(fp); // close the file

	// Print the results
	printLexemeTable(lexemeTable, lexemeCount); // print the lexeme table
	printTokenList(lexemeTable, lexemeCount);	// print the token list
} // end main

void printFile(FILE *fp)
{
	printf("\nSource Code:\n");
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