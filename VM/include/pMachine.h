//Header File For VM (P-Machine)

//Header Guards
#ifndef PMACHINE_H
#define PMACHINE_H

#define stackSize 500;

//define ActionRegister for use within stack
typedef struct{
	//houses parameters
	struct parameters;
	
	//points to the next instruction.
	int locals;
	
	//Points to the enxt instruction to be executed.
	int returnAddress;
	
	//Points to the base of the previous stack frame.
	int dynamicLink;
	
	//Points to the stack frame that statically 
	//encloses the current function.
	int staticLink;
	
	//Location to store the function.
	int functionalValue;
}actionRegister;

//Struct to house the 3 different parameters:
//OPR - Opcode
//L - Lexicographical level
//M - Instruction specific to the Opcode
typedef struct{
	int op;
	int l;
	int m;
}instructionRegister;

//individual stack frame for stack pMachine
typedef struct{
	//Frame details
	struct actionRegister;

	//position in stack
	int position;
	
	//Stack pointers:
	struct* stackFrame next;
	struct* stackFrame last;
}stackFrame;

//main method for VM
int pMachine(int* cIc);

//convert from char -> int array
int* convertInstructionCode(char* textCode);


#endif
