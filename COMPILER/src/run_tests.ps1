# run_tests.ps1
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Write-Host '==> Compiling parsercodegen.c at pl0c.exe'
gcc -Wall -Wextra -std=c99 -o pl0c.exe hw4compiler.c

Write-Host '==> Compiling vm.c at pvm.exe'
gcc -Wall -Wextra -std=c99 -o pvm.exe vm.c

foreach ($i in 1..3) {
    $test = "ex$i.pl0"
    Write-Host "`n===== Running $test ====="
    
    Write-Host '`--Compiler output --'
    .\pl0c.exe $test
    
    Write-Host '`-- VM output --'
    .\pvm.exe elf.txt

}