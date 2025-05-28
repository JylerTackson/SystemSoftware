// vm.c
// Student Name: Tyler Jackson

#include <stdio.h>
#include <stdlib.h>
#include "../include/vm.h"

int pas[ARRAY_SIZE] = {0};
int PC = 10, BP = 499, SP = 500, halt = 1;
InstructionRegister IR;

const char* oprNames[] = {
	"RTN",	// 0: Return
	"ADD",   // 1: Addition
	"SUB",   // 2: Subtraction
	"MUL",   // 3: Multiplication
	"DIV",   // 4: Divison
	"EQL",   // 5: Equal
	"NEQ",   // 6: Not Equal
	"LSS",   // 7: Less than
	"LEQ",   // 8: Less than or equal
	"GTR",   // 9: Greater than
	"GEQ",   // 10: Greater than or equal
};

const char* opCodeNames[] = {
	"INC",     // 1 Increment
	"OPR",     // 2 Return
	"LOD",     // 3 Load to Stack
	"STO",     // 4 Store
	"CAL",     // 5 Call procedure
	"LIT",     // 6 Push Literal
	"JMP",     // 7 Jump to command
	"JPC",     // 8 Jump conditionally
	"SYS",     // 9 System calls
};


int base(int L, int BP) {
	int arb = BP;
	while (L > 0) {
		arb = pas[arb];
		L--;
	}
	return arb;
}

void printState() {


	if(IR.op == 2) printf("%s %d  %d",oprNames[IR.m], IR.l, IR.m);
	else	printf("%s %d  %d",opCodeNames[IR.op-1] ,IR.l , IR.m);

	printf("\t%d\t%d\t%d\t", PC, BP, SP);
	for (int i = ARRAY_SIZE - 1; i >= SP; i--) {
		if (i == BP) printf("| ");
		printf("%d ", pas[i]);
	}
	printf("\n");

}

int main(int argc, char *argv[]) {


	//Validate an input file was inserted.
	if (argc < 2) {
		printf("Usage: %s <input file>\n", argv[0]);        
		return 1;
	}

	//Open file
	FILE *fp = fopen(argv[1], "r");
	if (!fp){
		printf("Error: Cannot open file %s\n", argv[1]);
		return 1;
	}

	//Begin Reading in the file.
	int i = 0;
	while (fscanf(fp, "%d %d %d", &pas[10 + i], &pas[10 + i + 1], &pas[10 + i + 2]) == 3) i += 3;
	fclose(fp);

	for (int i = 0; i < ARRAY_SIZE; i++) printf("%d ",pas[i]);
	printf("\n\n");

	printf("\tPC\tBP\tSP\tStack\nInitial values:\t%d\t%d\t%d\n", PC, BP, SP);

	while (halt) {
		// FETCH
		IR.op = pas[PC];
		IR.l = pas[PC + 1];
		IR.m = pas[PC + 2];
		PC += 3;

		// EXECUTE
		switch (IR.op) {
			case 1: // INC
				SP -= IR.m;
				break;

			case 2: // OPR
				switch (IR.m) {
					case 0: // RTN
						SP = BP + 1;
						BP = pas[SP - 2];
						PC = pas[SP - 3];
						break;
					case 1: // ADD
						pas[SP + 1] += pas[SP]; SP++;
						break;
					case 2: // SUB
						pas[SP + 1] -= pas[SP]; SP++;
						break;
					case 3: // MUL
						pas[SP + 1] *= pas[SP]; SP++;
						break;
					case 4: // DIV
						pas[SP + 1] /= pas[SP]; SP++;
						break;
					case 5: // EQL
						pas[SP + 1] = (pas[SP + 1] == pas[SP]); SP++;
						break;
					case 6: // NEQ
						pas[SP + 1] = (pas[SP + 1] != pas[SP]); SP++;
						break;
					case 7: // LSS
						pas[SP + 1] = (pas[SP + 1] < pas[SP]); SP++;
						break;
					case 8: // LEQ
						pas[SP + 1] = (pas[SP + 1] <= pas[SP]); SP++;
						break;
					case 9: // GTR
						pas[SP + 1] = (pas[SP + 1] > pas[SP]); SP++;
						break;
					case 10: // GEQ
						pas[SP + 1] = (pas[SP + 1] >= pas[SP]); SP++;
						break;
				}
				break;

			case 3: // LOD
				SP--;
				pas[SP] = pas[base(IR.l, BP) - IR.m];
				break;

			case 4: // STO
				pas[base(IR.l, BP) - IR.m] = pas[SP];
				SP++;
				break;

			case 5: // CAL
				pas[SP - 1] = base(IR.l, BP);
				pas[SP - 2] = BP;
				pas[SP - 3] = PC;
				BP = SP - 1;
				PC = IR.m;
				break;

			case 6: // LIT
				SP--;
				pas[SP] = IR.m;
				break;

			case 7: // JMP
				PC = IR.m;
				break;

			case 8: // JPC
				if (pas[SP] == 0) PC = IR.m;
				SP++;
				break;
			case 9: // SYS
				switch (IR.m) {
					case 1:
						printf("Output result is: %d\n", pas[SP]);
						SP++;
						break;
					case 2:
						printf("Please Enter an Integer: ");
						SP--;
						scanf("%d", &pas[SP]);
						break;
					case 3:
						halt = 0;
						break;
				}
				break;
		}
		//pass in current activation record
		printState();	
	}
	return 0;
}

