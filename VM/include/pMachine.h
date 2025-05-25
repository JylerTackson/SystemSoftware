// Header File For VM (P-Machine)

// Header Guards
#ifndef PMACHINE_H
#define PMACHINE_H

#define stackSize 500
int pas[stackSize];

// Struct to house the 3 different parameters:
// OPR - Opcode
// L - Lexicographical level
// M - Instruction specific to the Opcode
typedef struct
{
	int op;
	int l;
	int m;
} instructionRegister;

// main method for VM
int pMachine(int *cIc);
int base(int BP, int L);

#endif
