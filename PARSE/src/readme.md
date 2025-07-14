# PL/0 Parser & Code Generator

This repo contains **`parsercodegen.c`**, which parses PL/0 source and emits intermediate code, plus **`run_tests.ps1`** to automate testing.

---

## Features

- **Scanner:** Tokenizes identifiers, numbers, keywords, operators, and punctuation.
- **Parser:** Recursive‑descent for PL/0:

  - `program` → `block .`
  - Declarations: `const`, `var`
  - Control: `if`, `while`, `begin ... end`
  - I/O: `read`, `write`
  - Expressions, terms, factors, conditions

- **Semantic checks:** Duplicate/undeclared identifier detection and declaration/type validations.
- **Codegen:** Emits quadruple-format code for a stack VM.
- **Error handling:** 15 distinct error codes with clear messages.

<!-- Could shorten bullet titles further for brevity. -->

---

## Project Structure

```
src/
├─ parsercodegen.c       # Parser + code generator
└─ run_tests.ps1         # PowerShell test runner
test/
├─ error/input/       # 15 error-case PL/0 files (errorin1.txt…errorin15.txt)
└─ no_error/input/    # Valid PL/0 file (noerror1.txt)
```

---

## Build

```powershell
gcc .\parsercodegen.c -o parse.exe
```

This creates **`parse.exe`**.

---

## Usage

**Single test:**

```powershell
# Error case:
.\parse.exe ..\test\error\input\errorin3.txt

# Valid case:
.\parse.exe ..\test\no_error\input\noerror1.txt
```

The tool prints `Error: ...` on failure or finishes silently on success.

---

## Automated Tests

1. Allow script execution:

   ```powershell
   Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
   ```

2. Run the test runner:

   ```powershell
   .\run_tests.ps1
   ```

This will recompile, run the 15 error tests, then the no-error test.
