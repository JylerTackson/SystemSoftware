//HEADER FILE FOR THE SOURCE CODE OF THE LEXICAL ANALYSER
//--------------------------------------------------------
// Student Names: 
// 	David T. Jackson - 5298477
// 	Santiago - #######
// Course:
// 	COP 3402 - Systems Software
// Instructor:
// 	Dr. Montagne
#ifndef LEX_H
#define LEX_H



typedef struct{
	char *SS = {'+','-','*','/','(',')','=',',','.','<','>',';',':'};
	char *invis = {'\t',' ','\n'};
}differentTokens;

const char *reservedWords[] = {
	"const", "var", "procedure", "call",
	"begin", "end", "if", "then", "else",
	"fi", "while", "do", "read", "write", NULL};

const token_type reserved tokens[] = {
	constsym, varsym, procsym, callsym,
	beginsym, endsym, ifsym, thensym, elsesym,
	fisym, whilesym, dosym, readsym, writesym};

typedef enum {
skipsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, fisym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym} token_type;

int main(int argc, char *argv[]);
void printFile(FILE *fp);
int whitespace(int curr);

#endif
