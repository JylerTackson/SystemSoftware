// THIS WILL BE THE SOURCE CODE FILE WE TURN IN
//------------------------------
// GROUP:
//     David T. Jackson
//     5298477
//     Santiago Henao Rojas
//     5122384
//------------------------------
// ASSIGNMENT 4:
//     Implement a PL/0 compiler that performs lexical
//     analysis, parsing, and code generation.
//         - Read source program written in PL/0
//         - Identify errors
//         - Generate assembly code and symbol table
//         - Write instructions to elf.txt
//------------------------------
// CONSTRAINTS:
//     Input:
//       - Identifiers can be max of 11 chars
//       - Numbers can be max of 5 digits
//       - Comments should be ignored
//       - Invisible characters should be ignored
//
//     Output:
//       - Assembly instructions in elf.txt
//       - Console output of assembly code and symbol table

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_IDENT_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define MAX_LEXEMES 2000
#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOLS 500

/* Token definitions */
typedef enum
{
	modsym = 1,
	identsym,
	numbersym,
	plussym,
	minussym,
	multsym,
	slashsym,
	eqlsym,
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
	elsesym,
	fisym,
	whilesym,
	dosym,
	callsym,
	constsym,
	varsym,
	procsym,
	readsym,
	writesym
} token_type;

static const char *reservedWords[] = {
	"mod", "const", "var", "procedure", "begin", "end",
	"if", "then", "else", "fi", "while", "do",
	"call", "read", "write", NULL};
static const token_type reservedTokens[] = {
	modsym, constsym, varsym, procsym, beginsym, endsym,
	ifsym, thensym, elsesym, fisym, whilesym, dosym,
	callsym, readsym, writesym};

/* Lexeme table */
typedef struct
{
	char lexeme[MAX_IDENT_LENGTH + 1];
	token_type token;
} Lexeme;
static Lexeme LT[MAX_LEXEMES];
static int ltCount = 0;
static void addLex(token_type tk, const char *lx)
{
	strcpy(LT[ltCount].lexeme, lx);
	LT[ltCount].token = tk;
	ltCount++;
}

void dumpLex(void)
{
	/* debug suppressed */
}

/* Scanner */
void scanFile(const char *fname)
{
	FILE *fp = fopen(fname, "r");
	if (!fp)
	{
		fprintf(stderr, "cannot open %s\n", fname);
		exit(1);
	}
	int ch;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (isspace(ch))
			continue;
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
			token_type tk = identsym;
			for (int i = 0; reservedWords[i]; i++)
			{
				if (!strcmp(buf, reservedWords[i]))
				{
					tk = reservedTokens[i];
					break;
				}
			}
			addLex(tk, buf);
			continue;
		}
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
			if (len > MAX_NUMBER_LENGTH)
			{
				fprintf(stderr, "Number too large\n");
				exit(1);
			}
			addLex(numbersym, buf);
			continue;
		}
		int la = fgetc(fp);
		bool two = false;
		char duo[3] = {(char)ch, (char)la, 0};
		if (ch == '<' && la == '=')
		{
			addLex(leqsym, duo);
			two = true;
		}
		else if (ch == '<' && la == '>')
		{
			addLex(neqsym, duo);
			two = true;
		}
		else if (ch == '>' && la == '=')
		{
			addLex(geqsym, duo);
			two = true;
		}
		else if (ch == ':' && la == '=')
		{
			addLex(becomessym, duo);
			two = true;
		}
		else
			ungetc(la, fp);
		if (two)
			continue;
		char one[2] = {(char)ch, 0};
		switch (ch)
		{
		case '+':
			addLex(plussym, one);
			break;
		case '-':
			addLex(minussym, one);
			break;
		case '*':
			addLex(multsym, one);
			break;
		case '/':
			addLex(slashsym, one);
			break;
		case '(':
			addLex(lparentsym, one);
			break;
		case ')':
			addLex(rparentsym, one);
			break;
		case ',':
			addLex(commasym, one);
			break;
		case ';':
			addLex(semicolonsym, one);
			break;
		case '.':
			addLex(periodsym, one);
			break;
		case '<':
			addLex(lessym, one);
			break;
		case '>':
			addLex(gtrsym, one);
			break;
		case '=':
			addLex(eqlsym, one);
			break;
		default:
			fprintf(stderr, "Invalid symbol '%c'\n", ch);
			exit(1);
		}
	}
	fclose(fp);
}

/* Parser globals */
static int tok = 0;
#define CURTOK (LT[tok].token)
#define CURLEX (LT[tok].lexeme)
#define CURNUM (atoi(LT[tok].lexeme))

/* Code & symbols */
typedef struct
{
	int op, l, m;
} Instruction;
static Instruction code[MAX_CODE_LENGTH];
static int cx = 0;
enum
{
	INC = 1,
	OPR,
	LOD,
	STO,
	CAL,
	LIT,
	JMP,
	JPC,
	SYS
};
enum
{
	RTN = 0,
	ADD = 1,
	SUB,
	MUL,
	DIV_,
	EQL,
	NEQ,
	LSS,
	LEQ,
	GTR,
	GEQ,
	MODOP
};

typedef struct
{
	int kind;
	char name[12];
	int val, level, addr, mark;
} Symbol;
static Symbol ST[MAX_SYMBOLS];
static int stCount = 0;
static int mainStart = 0;
#define KIND_CONST 1
#define KIND_VAR 2
#define KIND_PROC 3

/* Helpers */
static void advance(void) { tok++; }
static bool accept(token_type t)
{
	if (CURTOK == t)
	{
		advance();
		return true;
	}
	return false;
}
static void expect(token_type t, int e)
{
	if (!accept(t))
	{
		fprintf(stderr, "Error %d at tok=%d ('%s')\n", e, tok, CURLEX);
		exit(1);
	}
}
static int lookup(const char *n)
{
	for (int i = stCount - 1; i >= 0; i--)
		if (!strcmp(ST[i].name, n))
			return i;
	return -1;
}
static int enter(int kind, const char *n, int v, int lvl, int addr)
{
	Symbol s = {kind, "", v, lvl, addr, 1};
	strcpy(s.name, n);
	ST[stCount++] = s;
	return stCount - 1;
}
static void emit(int op, int l, int m)
{
	code[cx++] = (Instruction){op, l, m};
}

/* Forward */
void program(void);
void block(int);
void constDecl(int);
void varDecl(int, int *);
void procDecl(int);
void statement(int);
void condition(int);
void expression(int);
void term(int);
void factor(int);

/* Printing */
const char *opName(int op);
void printCode(void);
void printSymbolTable(void);

/* Parser */

void program(void)
{
	dumpLex();
	int j0 = cx;
	emit(JMP, 0, 0);
	block(0);
	expect(periodsym, 9);
	emit(SYS, 0, 3);
	/* set program‐jmp to first main stmt */
	code[j0].m = 10 + 3 * mainStart;

	printf("No errors, program is syntactically correct.\n");
	printCode();
	printSymbolTable();
}

void block(int level)
{
	int nVars = 0;
	if (accept(constsym))
	{
		constDecl(level);
		while (accept(commasym))
			constDecl(level);
		expect(semicolonsym, 5);
	}
	if (accept(varsym))
	{
		varDecl(level, &nVars);
		while (accept(commasym))
			varDecl(level, &nVars);
		expect(semicolonsym, 5);
	}
	while (accept(procsym))
		procDecl(level);

	if (level == 0)
		mainStart = cx;
	emit(INC, 0, 3 + nVars);
	statement(level);
	/* hide deeper‐level symbols */
	for (int i = stCount - 1; i >= 0 && ST[i].level > level; i--)
		ST[i].mark = 0;
}

void constDecl(int level)
{
	char name[12];
	if (CURTOK != identsym)
		exit(1);
	strcpy(name, CURLEX);
	advance();
	expect(eqlsym, 3);
	if (CURTOK != numbersym)
		exit(1);
	int v = CURNUM;
	advance();
	enter(KIND_CONST, name, v, level, 0);
}

void varDecl(int level, int *n)
{
	char name[12];
	if (CURTOK != identsym)
		exit(1);
	strcpy(name, CURLEX);
	advance();
	enter(KIND_VAR, name, 0, level, 3 + *n);
	(*n)++;
}

void procDecl(int level)
{
	char name[12];
	if (CURTOK != identsym)
	{
		fprintf(stderr, "Error 5 at tok=%d ('%s')\n", tok, CURLEX);
		exit(1);
	}
	strcpy(name, CURLEX);
	advance();
	expect(semicolonsym, 5);

	/* placeholder jump out of this proc */
	int jmpIdx = cx;
	emit(JMP, 0, 0);

	/* record symbol‐table entry using jmpIdx! */
	int procStart = cx;
	enter(KIND_PROC, name, 0, level, 10 + 3 * jmpIdx);

	block(level + 1);
	expect(semicolonsym, 6);
	emit(OPR, 0, RTN);

	/* now patch that JMP to jump to after proc’s code */
	code[jmpIdx].m = 10 + 3 * procStart;
}

void statement(int level)
{
	if (CURTOK == identsym)
	{
		int i = lookup(CURLEX);
		advance();
		expect(becomessym, 13);
		expression(level);
		emit(STO, level - ST[i].level, ST[i].addr);
		return;
	}
	if (accept(beginsym))
	{
		do
			statement(level);
		while (accept(semicolonsym));
		expect(endsym, 17);
		return;
	}
	if (accept(ifsym))
	{
		condition(level);
		expect(thensym, 16);
		int jpc = cx;
		emit(JPC, 0, 0);
		statement(level);
		if (accept(elsesym))
		{
			int jmp = cx;
			emit(JMP, 0, 0);
			code[jpc].m = 10 + 3 * cx;
			statement(level);
			expect(fisym, 28);
			code[jmp].m = 10 + 3 * cx;
		}
		else
		{
			expect(fisym, 28);
			code[jpc].m = 10 + 3 * cx;
		}
		return;
	}
	if (accept(whilesym))
	{
		int loop = cx;
		condition(level);
		expect(dosym, 18);
		int jpc = cx;
		emit(JPC, 0, 0);
		statement(level);
		emit(JMP, 0, 10 + 3 * loop);
		code[jpc].m = 10 + 3 * cx;
		return;
	}
	if (accept(callsym))
	{
		expect(identsym, 14);
		int i = lookup(LT[tok - 1].lexeme);
		emit(CAL, level - ST[i].level, ST[i].addr);
		return;
	}
	if (accept(readsym))
	{
		expect(identsym, 27);
		int i = lookup(LT[tok - 1].lexeme);
		emit(SYS, 0, 2);
		emit(STO, level - ST[i].level, ST[i].addr);
		return;
	}
	if (accept(writesym))
	{
		expression(level);
		emit(SYS, 0, 1);
		return;
	}
}

void condition(int level)
{
	expression(level);
	token_type rel = CURTOK;
	advance();
	expression(level);
	int r = EQL;
	switch (rel)
	{
	case eqlsym:
		r = EQL;
		break;
	case neqsym:
		r = NEQ;
		break;
	case lessym:
		r = LSS;
		break;
	case leqsym:
		r = LEQ;
		break;
	case gtrsym:
		r = GTR;
		break;
	case geqsym:
		r = GEQ;
		break;
	default:
		exit(1);
	}
	emit(OPR, 0, r);
}

void expression(int level)
{
	bool neg = accept(minussym);
	if (neg)
		emit(LIT, 0, 0);
	accept(plussym);
	term(level);
	if (neg)
		emit(OPR, 0, SUB);
	while (CURTOK == plussym || CURTOK == minussym)
	{
		int op = CURTOK;
		advance();
		term(level);
		emit(OPR, 0, op == plussym ? ADD : SUB);
	}
}

void term(int level)
{
	factor(level);
	while (CURTOK == multsym || CURTOK == slashsym || CURTOK == modsym)
	{
		int op = CURTOK;
		advance();
		factor(level);
		emit(OPR, 0, op == multsym ? MUL : (op == slashsym ? DIV_ : MODOP));
	}
}

void factor(int level)
{
	if (CURTOK == identsym)
	{
		int i = lookup(CURLEX);
		if (ST[i].kind == KIND_CONST)
			emit(LIT, 0, ST[i].val);
		else
			emit(LOD, level - ST[i].level, ST[i].addr);
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
		expression(level);
		expect(rparentsym, 22);
		return;
	}
	exit(1);
}

/* ─── Printing ─── */
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

void printCode(void)
{
	puts("\nAssembly Code:\n");
	puts("Line\tOP\tL\tM");
	for (int i = 0; i < cx; i++)
	{
		printf("%3d\t%s\t%d\t%d\n",
			   i, opName(code[i].op), code[i].l, code[i].m);
	}
}

void printSymbolTable(void)
{
	puts("\nKind | Name | Value | Level | Address | Mark");
	puts("----------------------------------------------");
	for (int i = 0; i < stCount; i++)
	{
		if (ST[i].mark)
		{
			printf("%4d | %-4s | %5d | %5d | %7d | %4d\n",
				   ST[i].kind,
				   ST[i].name,
				   ST[i].val,
				   ST[i].level,
				   ST[i].addr,
				   ST[i].mark);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}
	scanFile(argv[1]);
	program();
	return 0;
}
