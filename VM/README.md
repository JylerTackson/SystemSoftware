# P-Machine Virtual Machine

## Overview

This project implements a virtual machine called the **P-machine**, a stack-based machine with a unified memory model called the Process Address Space (PAS). The machine reads instructions from an input file, loads them into memory, and executes them according to defined opcodes and operation rules.

---

## Author

**David Tyler Jackson**  
**Student ID:** 5298477  
**Course:** COP 3402 - Systems Software  
**Instructor:** Dr. Montagne

---

## File Structure

### `vm.c`

This is the **main source file** that contains:

- Initialization of global variables.
- File I/O for loading instructions into memory.
- The core **fetch-decode-execute** loop that drives the virtual machine.
- Full implementation of the 9 opcodes including system calls, arithmetic operations, and control flow instructions.
- Instruction output and stack tracing after every operation.

### `vm.h`

This is the **header file** that contains:

- `InstructionRegister` struct definition used to represent instructions.
- Arrays of opcode and operator names (`opCodeNames`, `oprNames`) for readable output.
- Constant definitions such as `ARRAY_SIZE` and `INITIAL_BP`.
- Function prototypes for `base(int L, int BP)` and `printState()`.

---

## How to Compile

To compile the project, use the following command:

```bash
gcc -Wall vm.c -o vm.exe

```

## How to Execute

To execute the compiled project, use the following command:

```bash
./vm.exe ../TestFiles/IS2.test

```
