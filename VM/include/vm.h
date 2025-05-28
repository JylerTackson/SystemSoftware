// pMachine.h
// Student Name: Tyler Jackson

#ifndef PMACHINE_H
#define PMACHINE_H

#define ARRAY_SIZE 500

// Structure to represent an instruction with OP, L, and M fields
typedef struct {
    int op; // Opcode
    int l;  // Lexicographical Level
    int m;  // Modifier
} InstructionRegister;



// Function declarations
int base(int L, int BP);
void printState();

#endif // PMACHINE_H
