# Lexical Analyzer - PL/0 Language

## Authors:

- David T. Jackson (5298477)
- Santiago Henao Rojas (5122384)

## Description:

This lexical analyzer reads a source file written in the PL/0 programming language
and generates:

- A lexeme table
- A token list
- Error messages for invalid characters, too-long identifiers, and numbers

## Usage:

1. Compile the source file using a C compiler (e.g., gcc or clang):

   gcc -o lex lex.c

2. Run the lexer on a valid or test Pascal/PL0 file:

   ../tests/testA.txt

3. The output will display:
   - The source code
   - The lexeme table
   - The token list

## Error Handling:

The lexer will detect:

- Invalid characters (e.g., @, #, $, ^)
- Identifiers longer than 11 characters
- Numbers longer than 5 digits
- Lexeme table overflow (>1000 tokens)

## Notes:

- Your source file must be passed as a command line argument
- If no file is passed or the file cannot be opened, the program will print a usage error
- Comments enclosed in `/* ... */` are ignored
