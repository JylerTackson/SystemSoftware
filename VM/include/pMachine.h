// Header File For VM (P-Machine)

// Header Guards
#ifndef PMACHINE_H
#define PMACHINE_H

#define stackSize 500

// Struct to house the 3 different parameters:
// OPR - Opcode
// L - Lexicographical level
// M - Instruction specific to the Opcode
typedef struct{
	int op;
	int l;
	int m;
} instructionRegister;

//Define globals (mimic 'stack' & Instruction Register struct)
int* pas[stackSize];
instructionRegister IR;

// main method for VM
int pMachine(int *cIc);

//Fetch & Execute cycles for VM
int fetch(int *cIc, int programCounter);
void execute(int *cIc);

//helper functions
int base(int BP, int L);

#endif
