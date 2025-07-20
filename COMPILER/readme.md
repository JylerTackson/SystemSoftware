# README

## Overview

This project implements a PL/0 compiler and a P‑machine virtual machine, along with a PowerShell script to compile both programs and run a suite of test cases.

## Files

- **run\_tests.ps1**: A PowerShell script that automates the compilation of the C source files and executes them against test PL/0 programs (`ex1.pl0`, `ex2.pl0`, `ex3.pl0`).
- **hw4compiler.c**: The PL/0 compiler source code. It performs lexical analysis, parsing, and code generation for the PL/0 language. When invoked, it:
  1. Reads a PL/0 source file.
  2. Prints "No errors, program is syntactically correct." (or reports errors).
  3. Outputs the generated assembly code and symbol table to the console.
  4. Writes the assembled instructions into `elf.txt` for the VM to consume.
- **vm.c**: The P‑machine virtual machine source code. It simulates a stack‑based machine with instructions loaded from `elf.txt`. It:
  1. Loads the instruction stream into its process address space.
  2. Executes instructions (e.g., LIT, OPR, LOD, STO, CAL, JMP, JPC, SYS).
  3. Prints the machine state (PC, BP, SP, stack contents) after each instruction.

## Prerequisites

- **Operating System**: Windows (PowerShell).
- **Compiler**: GCC available in your PATH (e.g., via MinGW or Cygwin).
- **PowerShell Execution Policy**: Ensure script execution is allowed. In an elevated PowerShell prompt, you may run:
  ```powershell
  Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
  ```

## Usage

### Automated Tests with PowerShell

1. Open PowerShell and navigate to the project directory containing `run_tests.ps1`, `hw4compiler.c`, `vm.c`, and the test files (`ex1.pl0`, `ex2.pl0`, `ex3.pl0`).
2. Execute the script:
   ```powershell
   .\run_tests.ps1
   ```
3. The script will:
   - Compile **hw4compiler.c** into `pl0c.exe`.
   - Compile **vm.c** into `pvm.exe`.
   - For each test file (`ex1.pl0` through `ex3.pl0`):
     1. Run the compiler: `.\pl0c.exe exN.pl0` (produces console output and `elf.txt`).
     2. Run the VM: `.\pvm.exe elf.txt` (prints VM execution trace).

### Manual Compilation and Execution

If you prefer manual commands:

```powershell
# 1. Compile the PL/0 compiler
gcc -Wall -Wextra -std=c99 -o pl0c.exe hw4compiler.c

# 2. Compile the virtual machine
gcc -Wall -Wextra -std=c99 -o pvm.exe vm.c

# 3. Run the compiler on a PL/0 source file
#    This will generate elf.txt and print assembly + symbol table
.\pl0c.exe <source>.pl0

# 4. Run the VM on the generated code
.\pvm.exe elf.txt
```

## Notes

- The compiler and VM communicate via the `elf.txt` file: the compiler writes binary instructions to it, and the VM reads from it.
- You can extend the test loop in `run_tests.ps1` by adjusting the `foreach ($i in 1..N)` range or by adding additional `.pl0` files.
- Review console output for both compiler and VM traces to debug PL/0 programs and understand execution flow.

