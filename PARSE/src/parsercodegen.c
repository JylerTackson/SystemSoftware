/**********************************************************************/
/*  COP 3402 – Summer 2025  |  HW-3  Tiny-PL/0 Compiler               */
/*                                                                  */
/*  GROUP  :  David T. Jackson (5298477)                              */
/*                                                                  */
/*  This file contains scanner, recursive-descent parser, symbol-    */
/*  table builder, and code-generator for the subset of PL/0 given  */
/*  in Appendix B of the assignment hand-out.                        */
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* ──────────  GLOBAL CONSTANTS  ────────── */
#define MAX_IDENT_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define MAX_LEXEMES 1000
#define MAX_CODE_LENGTH 500
#define MAX_SYMBOLS 500

/* ─────────────  TOKEN SET  ───────────── */
typedef enum
{
	/* reserved */
	oddsym = 1, // "odd"
	identsym,
	numbersym,
	plussym,
	minussym,
	multsym,
	slashsym,
	eqsym,
	neqsym,
	lessym,
	leqsym,
	gtrsym,
	geqsym,
	lparentsym,
	rparentsym,
	commasym,
	semicolonsym,
	periodsym,
	becomessym,
	beginsym,
	endsym,
	ifsym,
	thensym,
	whensym,
	dosym,
	constsym,
	varsym,
	readsym,
	writesym
} token_type;

/* keywords *in the same order* as the matching token values above
   after oddsym (index 0 in this table).                              */
const char *reservedWords[] = {
	"odd",
	"const", "var", "begin", "end",
	"if", "then", "when", "do",
	"read", "write", NULL};

const token_type reservedTokens[] = {
	oddsym,
	constsym, varsym, beginsym, endsym,
	ifsym, thensym, whensym, dosym,
	readsym, writesym};

/* ─────────────  LEXEME TABLE  ────────── */
typedef struct
{
	char lexeme[MAX_IDENT_LENGTH + 1];
	token_type token;
	bool isError;
	char msg[80];
} Lexeme;

Lexeme LT[MAX_LEXEMES];
int ltCount = 0;

/* ─────────────  VM CODE ARRAY  ──────── */
typedef struct
{
	int op, l, m;
} Instruction;

Instruction code[MAX_CODE_LENGTH];
int cx = 0; // next free slot

enum
{
	LIT = 1,
	OPR,
	LOD,
	STO,
	CAL,
	INC,
	JMP,
	JPC,
	SYS
};
// OPR sub‑codes
enum
{
	RET = 0,
	NEG,
	ADD,
	MUL,
	SUB,
	DIV_,
	ODD,
	EQL,
	NEQ,
	LSS,
	LEQ,
	GTR,
	GEQ
};

/* ─────────────  SYMBOL TABLE  ───────── */
typedef struct
{
	int kind; // 1 = const, 2 = var
	char name[12];
	int val;
	int level; // always 0 for HW‑3
	int addr;
	int mark;
} Symbol;

Symbol ST[MAX_SYMBOLS];
int stCount = 0;

#define KIND_CONST 1
#define KIND_VAR 2

/* ─────────────  ERROR MESSAGES  ─────── */
const char *errMsg[] = {
	"", /* dummy so index == code */
	"Missing a period \".\" at the end of the program",
	"Missing identifier after const, var, or read keyword",
	"Symbol name has already been declared",
	"Constants must be assigned with \"=\" operator",
	"constants must be assigned an integer value",
	"constant (or variable) declarations must be followed by a semicolon",
	"undeclared identifier",
	"only variable values may be altered",
	"assignment statements must use \":=\"",
	"\"begin\" must be matched by \"end\"",
	"\"if\" must be followed by \"then\"",
	"when must be followed by do",
	"condition must contain a comparison operator",
	"right parenthesis must follow left parenthesis",
	"arithmetic equations must contain operands, parentheses, numbers, or symbols"};

/* ─────────────  FORWARD DECLS  ─────── */
void program();
void block();
void constDecl();
void varDecl(int *nVars);
void statement();
void condition();
void expression();
void term();
void factor();
void emit(int op, int l, int m);

/* error handling and symbol‐table helpers */
void error(int code);
void advance();
int lookup(const char *name);
int enter(int kind, const char *name, int val, int lvl, int addr);

/* ─────────────  TOKEN CURSOR  ─────── */
int tok = 0; // index into LT
#define CURTOK (LT[tok].token)
#define CURLEX (LT[tok].lexeme)
#define CURNUM (atoi(LT[tok].lexeme))

// advance - Moves to the next token in the lexeme table, if any remain.
void advance()
{
	if (tok < ltCount)
		++tok;
}

//  accept - Checks if the current token matches t;
//  if so, consumes it (advance) and returns true,
//  otherwise returns false without consuming.
bool accept(token_type t)
{
	if (CURTOK == t)
	{
		advance();
		return true;
	}
	return false;
}

// expect - Ensures the current token is t;
// if so, consumes it, otherwise raises error(e).
void expect(token_type t, int e)
{
	if (!accept(t))
		error(e);
}

/* ─────────────  SYMBOL HELPERS  ───── */

// lookup - Searches the symbol table for name;
// returns its index or -1 if not found/unmarked.
int lookup(const char *name)
{
	for (int i = stCount - 1; i >= 0; --i)
		if (!ST[i].mark && strcmp(ST[i].name, name) == 0)
			return i;
	return -1;
}

// enter - Adds a new symbol to the table;
// errors if duplicate or table full, returns its index.
int enter(int kind, const char *name, int val, int lvl, int addr)
{
	if (lookup(name) != -1)
		error(3);
	if (stCount >= MAX_SYMBOLS)
	{
		puts("Error: symbol table full");
		exit(1);
	}
	Symbol *s = &ST[stCount++];
	s->kind = kind;
	strcpy(s->name, name);
	s->val = val;
	s->level = lvl;
	s->addr = addr;
	s->mark = 0;
	return stCount - 1;
}

/* ─────────────  SCANNER UTILITIES  ── */
//  reservedLookup - Maps an identifier string to its reserved token,
//  or returns 0 if it is not a keyword.
token_type reservedLookup(const char *id)
{
	for (size_t i = 0; reservedWords[i]; ++i)
		if (strcmp(reservedWords[i], id) == 0)
			return reservedTokens[i];
	return 0;
}

/* ─────────────  SCANNER  ──────────── */
// scanFile - Reads a PL/0 source file, tokenizes it into LT[],
// handling whitespace, comments, identifiers, numbers, multi‐char and
// single‐char symbols, and reports scanner errors.
void scanFile(const char *fname)
{
	FILE *fp = fopen(fname, "r");
	if (!fp)
	{
		fprintf(stderr, "Error: cannot open %s\n", fname);
		exit(1);
	}
	int ch, la;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ltCount >= MAX_LEXEMES)
		{
			puts("Error: lexeme table overflow");
			exit(1);
		}
		if (isspace(ch))
			continue;

		/* comments */
		if (ch == '/')
		{
			la = fgetc(fp);
			if (la == '*')
			{ // block comment
				int prev = 0;
				while ((ch = fgetc(fp)) != EOF)
				{
					if (prev == '*' && ch == '/')
						break;
					prev = ch;
				}
				continue; // restart main loop
			}
			else
				ungetc(la, fp);
		}

		/* identifier / keyword */
		if (isalpha(ch))
		{
			char buf[MAX_IDENT_LENGTH + 2] = {0};
			int len = 0;
			do
			{
				if (len < MAX_IDENT_LENGTH)
					buf[len++] = ch;
				ch = fgetc(fp);
			} while (isalnum(ch));
			buf[len] = '\0';
			ungetc(ch, fp);
			token_type tk = reservedLookup(buf);
			if (!tk)
				tk = identsym;
			strcpy(LT[ltCount].lexeme, buf);
			LT[ltCount].token = tk;
			LT[ltCount].isError = false;
			++ltCount;
			continue;
		}

		/* number */
		if (isdigit(ch))
		{
			char buf[MAX_NUMBER_LENGTH + 2] = {0};
			int len = 0;
			do
			{
				if (len < MAX_NUMBER_LENGTH)
					buf[len++] = ch;
				ch = fgetc(fp);
			} while (isdigit(ch));
			buf[len] = '\0';
			ungetc(ch, fp);
			if (strlen(buf) > MAX_NUMBER_LENGTH)
			{
				LT[ltCount].isError = true;
				strcpy(LT[ltCount].msg, "Error: number too long");
			}
			else
			{
				LT[ltCount].token = numbersym;
				strcpy(LT[ltCount].lexeme, buf);
				LT[ltCount].isError = false;
			}
			++ltCount;
			continue;
		}

		/* two‑char tokens */
		if (ch == '<' || ch == '>' || ch == ':')
		{
			la = fgetc(fp);
			bool matched = true;
			if (ch == '<' && la == '=')
			{
				LT[ltCount].token = leqsym;
				strcpy(LT[ltCount].lexeme, "<=");
			}
			else if (ch == '<' && la == '>')
			{
				LT[ltCount].token = neqsym;
				strcpy(LT[ltCount].lexeme, "<>");
			}
			else if (ch == '>' && la == '=')
			{
				LT[ltCount].token = geqsym;
				strcpy(LT[ltCount].lexeme, ">=");
			}
			else if (ch == ':' && la == '=')
			{
				LT[ltCount].token = becomessym;
				strcpy(LT[ltCount].lexeme, ":=");
			}
			else
			{
				matched = false;
				ungetc(la, fp);
			}
			if (matched)
			{
				LT[ltCount].isError = false;
				++ltCount;
				continue;
			}
		}

		/* single‑char tokens */
		token_type tk = 0;
		char txt[3] = {0};
		txt[0] = ch;
		switch (ch)
		{
		case '+':
			tk = plussym;
			break;
		case '-':
			tk = minussym;
			break;
		case '*':
			tk = multsym;
			break;
		case '/':
			tk = slashsym;
			break;
		case '=':
			tk = eqsym;
			break;
		case '<':
			tk = lessym;
			break;
		case '>':
			tk = gtrsym;
			break;
		case '(':
			tk = lparentsym;
			break;
		case ')':
			tk = rparentsym;
			break;
		case ',':
			tk = commasym;
			break;
		case ';':
			tk = semicolonsym;
			break;
		case '.':
			tk = periodsym;
			break;
		default:
			sprintf(LT[ltCount].msg, "Invalid character %c", ch);
			LT[ltCount].isError = true;
			++ltCount;
			fclose(fp);
			return;
		}
		LT[ltCount].token = tk;
		strcpy(LT[ltCount].lexeme, txt);
		LT[ltCount].isError = false;
		++ltCount;
	}
	fclose(fp);
}

/* ─────────────  ERROR + EMIT  ─────── */
// error - Prints the message for code and exits.
// Code 7 also prints the offending identifier.
void error(int code)
{
	if (code == 7)
	{
		/* include the bad identifier */
		printf("Error: %s %s\n", errMsg[code], CURLEX);
	}
	else
	{
		printf("Error: %s\n", errMsg[code]);
	}
	exit(1);
}

// emit - Emits a quadruple (op, l, m) into the code array
// and increments the code counter.
void emit(int op, int l, int m)
{
	if (cx >= MAX_CODE_LENGTH)
	{
		puts("Error: code array full");
		exit(1);
	}
	code[cx++] = (Instruction){op, l, m};
}

/* ─────────────  PARSER / CODE GEN  ───────────── */

// program - Entry point of the parser.
// Emits an initial jump over the data area (to be patched later),
// invokes block() to parse declarations and the main statement,
// expects a terminating period, emits a return operation,
// and pads the generated code to a fixed size.
void program()
{
	emit(JMP, 0, 0); // jump over data area; will patch later
	int entryJmp = 0;
	block();
	expect(periodsym, 1);
	emit(OPR, 0, RET);
	code[entryJmp].m = 13; // spec value
	while (cx < 13)
		emit(INC, 0, 0); // pad to 13 lines
}

// block - Parses a block of PL/0 code.
// Handles optional const and var declarations,
// allocates space for variables, and parses the statement that follows.
void block()
{
	int nVars = 0;
	if (accept(constsym))
	{
		constDecl();
		while (accept(commasym))
			constDecl();
		expect(semicolonsym, 6);
	}
	if (accept(varsym))
	{
		varDecl(&nVars);
		while (accept(commasym))
			varDecl(&nVars);
		expect(semicolonsym, 6);
	}
	emit(INC, 0, 3 + nVars); // allocate space in stack SL | DL | RA + locals
	statement();
}

// constDecl - Parses a constant declaration.
// Expects an identifier, an '=' sign, and an integer literal.
// Rejects ':=' or arithmetic expressions in the declaration,
// and enters the constant into the symbol table.
void constDecl()
{
	/* 2: must start with an identifier */
	if (CURTOK != identsym)
		error(2);
	/* save the name *before* we munch it */
	char name[MAX_IDENT_LENGTH + 1];
	strcpy(name, CURLEX);
	advance();

	/* 4: forbid := here */
	if (CURTOK == becomessym)
		error(4);

	/* 4: must have = */
	if (CURTOK != eqsym)
		error(4);
	advance();

	/* 5: must have a number */
	if (CURTOK != numbersym)
		error(5);
	int val = CURNUM;
	advance();

	/* 5: no arithmetic after the literal */
	if (CURTOK == plussym || CURTOK == minussym || CURTOK == multsym || CURTOK == slashsym)
		error(5);

	/* 3: duplicate check lives in enter() */
	enter(KIND_CONST, name, val, 0, 0);
}

// varDecl - Parses a variable declaration.
// Expects an identifier and enters it into the symbol table.
// Increments the local variable count.
void varDecl(int *nVars)
{
	/* 2: must start with an identifier */
	if (CURTOK != identsym)
		error(2);
	/* save the name before we advance */
	char name[MAX_IDENT_LENGTH + 1];
	strcpy(name, CURLEX);
	advance(); /* consume the identifier */
	enter(KIND_VAR, name, 0, 0, 3 + *nVars);
	++*nVars;
}

// statement - Parses a statement.
// Supports assignment, begin...end blocks, if, while,
// read, and write statements, or does nothing for empty statements.
void statement()
{
	/* assignment */
	if (CURTOK == identsym)
	{
		int i = lookup(CURLEX);
		if (i == -1)
			error(7);
		if (ST[i].kind == KIND_CONST)
			error(8);
		advance();
		expect(becomessym, 9);
		expression();
		emit(STO, 0, ST[i].addr);
		return;
	}

	/* begin ... end */
	if (accept(beginsym))
	{
		do
		{
			statement();
		} while (accept(commasym) || accept(semicolonsym));
		expect(endsym, 10);
		return;
	}

	/* if */
	if (accept(ifsym))
	{
		condition();
		expect(thensym, 11);
		int jpcIdx = cx;
		emit(JPC, 0, 0);
		statement();
		code[jpcIdx].m = cx;
		return;
	}

	/* when */
	if (accept(whensym))
	{
		int loopStart = cx;
		condition();
		expect(dosym, 12);
		int jpcIdx = cx;
		emit(JPC, 0, 0);
		statement();
		emit(JMP, 0, loopStart);
		code[jpcIdx].m = cx;
		return;
	}

	/* read */
	if (accept(readsym))
	{
		expect(identsym, 2);
		int i = lookup(CURLEX);
		if (i == -1)
			error(7);
		if (ST[i].kind != KIND_VAR)
			error(8);
		advance();
		emit(SYS, 0, 2);
		emit(STO, 0, ST[i].addr);
		return;
	}

	/* write */
	if (accept(writesym))
	{
		expression();
		emit(SYS, 0, 1);
		return;
	}
}

// condition - Parses a condition.
// Supports odd(...) and relational expressions,
// emitting the appropriate operation code.
void condition()
{
	if (accept(oddsym))
	{
		expression();
		emit(OPR, 0, ODD);
		return;
	}
	expression();
	int rel;
	switch (CURTOK)
	{
	case eqsym:
		rel = EQL;
		break;
	case neqsym:
		rel = NEQ;
		break;
	case lessym:
		rel = LSS;
		break;
	case leqsym:
		rel = LEQ;
		break;
	case gtrsym:
		rel = GTR;
		break;
	case geqsym:
		rel = GEQ;
		break;
	default:
		error(13);
	}
	advance();
	expression();
	emit(OPR, 0, rel);
}

// expression - Parses an expression.
// Handles optional leading '+' or '-',
// followed by terms separated by '+' or '-'.
void expression()
{
	bool neg = accept(minussym);
	accept(plussym);
	term();
	if (neg)
		emit(OPR, 0, NEG);
	while (CURTOK == plussym || CURTOK == minussym)
	{
		int op = CURTOK;
		advance();
		term();
		emit(OPR, 0, op == plussym ? ADD : SUB);
	}
}

// term - Parses a term.
// Consists of factors separated by '*' or '/'.
void term(void)
{
	factor();
	while (CURTOK == multsym || CURTOK == slashsym)
	{
		int op = CURTOK;
		advance();
		factor();
		emit(OPR, 0, op == multsym ? MUL : DIV_);
	}
}

// factor - Parses a factor.
// Can be an identifier, a number literal, or a parenthesized expression.
void factor()
{
	if (CURTOK == identsym)
	{
		int i = lookup(CURLEX);
		if (i == -1)
			error(7);
		if (ST[i].kind == KIND_CONST)
			emit(LIT, 0, ST[i].val);
		else
			emit(LOD, 0, ST[i].addr);
		advance();
		return;
	}
	if (CURTOK == numbersym)
	{
		emit(LIT, 0, CURNUM);
		advance();
		return;
	}
	if (accept(lparentsym))
	{
		expression();
		expect(rparentsym, 14);
		return;
	}
	error(15);
}

/* ─────────────── Print functions ─────────────── */
const char *opName(int op)
{
	switch (op)
	{
	case LIT:
		return "LIT";
	case OPR:
		return "OPR";
	case LOD:
		return "LOD";
	case STO:
		return "STO";
	case CAL:
		return "CAL";
	case INC:
		return "INC";
	case JMP:
		return "JMP";
	case JPC:
		return "JPC";
	case SYS:
		return "SYS";
	}
	return "???";
}

void printCode()
{
	puts("\nAssembly Code:\n");
	puts("Line\tOP\tL\tM");
	for (int i = 0; i < cx; ++i)
		printf("%3d\t%s\t%d\t%d\n", i, opName(code[i].op), code[i].l, code[i].m);
}

void printSymbolTable()
{
	puts("\nKind | Name | Value | Level | Address | Mark");
	puts("----------------------------------------------");
	for (int i = 0; i < stCount; ++i)
		if (!ST[i].mark)
			printf("%4d | %-4s | %5d | %5d | %7d | %4d\n", ST[i].kind, ST[i].name, ST[i].val, ST[i].level, ST[i].addr, ST[i].mark);
}

/* ─────────────  MAIN  ─────────────── */
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <source.pl0>\n", argv[0]);
		return 1;
	}

	scanFile(argv[1]);
	for (int i = 0; i < ltCount; ++i)
	{
		if (LT[i].isError)
		{
			puts(LT[i].msg);
			return 0;
		}
	}

	tok = 0;
	program();
	printCode();
	printSymbolTable();
	return 0;
}