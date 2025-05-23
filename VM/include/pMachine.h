//Header File For VM (P-Machine)

//Header Guards
#ifndef PMACHINE_H
#define PMACHINE_H

#define stackSize 500;

//main method for VM
int pMachine(int* cIc);

//convert from char -> int array
int* convertInstructionCode(char* textCode);


#endif
