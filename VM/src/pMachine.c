#include "./include/pMachine.h"

// VM souce Code

// Contains the main logic for the VM Stack P-Machine
int pMachine(int *cIc)
{
	int sp = 0;
	int bp = 0;
	int pc = 0;

	instructionRegister IR;
	IR.op = cIc[0];
	IR.l = cIc[1];
	IR.m = cIc[2];

	// 1-11 Basic Instructions
	// 12-24 Arithmetic & Operations
	switch (IR.op)
	{
	case (1):
		// INC - Allocate n locals

		sp = sp - IR.m;

		break;
	case (2):
		// Arithmetic Operations

		switch (IR.m)
		{
		case (1):
			// ADD

			pas[sp + 1] = pas[sp + 1] + pas[sp];
			sp++;

			break;
		case (2):
			// SUB

			pas[sp + 1] = pas[sp + 1] - pas[sp];
			sp++;

			break;
		case (3):
			// MUL

			pas[sp + 1] = pas[sp + 1] * pas[sp];
			sp++;

			break;
		case (4):
			// DIV

			// Division by zero check
			if (pas[sp] == 0)
			{
			}
			pas[sp + 1] = pas[sp + 1] / pas[sp];
			sp++;

			break;
		case (5):
			// EQL

			pas[sp + 1] = pas[sp + 1] == pas[sp];
			sp++;

			break;
		case (6):
			// NEQ - Check odd

			pas[sp + 1] = pas[sp + 1] != pas[sp];
			sp++;

			break;
		case (7):
			// LSS

			pas[sp + 1] = pas[sp + 1] < pas[sp];
			sp++;

			break;
		case (8):
			// LEQ

			pas[sp + 1] = pas[sp + 1] <= pas[sp];
			sp++;

			break;
		case (9):
			// GTR

			pas[sp + 1] = pas[sp + 1] > pas[sp];
			sp++;

			break;
		case (10):
			// GEQ

			pas[sp + 1] = pas[sp + 1] >= pas[sp];
			sp++;

			break;
		default:
			// invalid operation
			printf("Invalid Arithmetic Operation");
			return 0;
			break;
		}

		break;
	case (3):
		// LOD - load from stack to register

		sp--;
		pas[sp] = pas[base(bp, IR.l) + IR.m];

		break;
	case (4):
		// STO - store

		pas[base(bp, IR.l) - IR.m] = pas[sp];
		sp++;

		break;
	case (5):
		// CAL - call procedure

		pas[sp - 1] = base(bp, IR.l); // return address
		pas[sp - 2] = bp;			  // old base pointer
		pas[sp - 3] = pc;			  // old program counter
		bp = sp - 1;				  // new base pointer
		pc = IR.m;					  // jump to new program counter

		break;
	case (6):
		// LIT - increment SP

		sp--;
		pas[sp] = IR.m; // load literal value

		break;
	case (7):
		// JMP - jump

		pc = IR.m; // jump to new program counter

		break;
	case (8):
		// JPC - jump conditional

		if (pas[sp] == 0)
		{
			pc = IR.m; // jump to new program counter
		}
		sp++; // increment stack pointer

		break;
	case (9):
		// SIO1 - Output

		break;
	case (10):
		// SIO2 - Input

		break;
	case (11):
		// SIO3 - Halt

		break;

	default:
		printf("Improper Instruction Given.");
		return 0;
	}
}

int base(int BP, int L)
{

	int arb = BP;
	while (L > 0)
	{
		arb = pas[arb]; // arb = activation record base
		L--;			// find base L levels down
	}
	return arb;
}