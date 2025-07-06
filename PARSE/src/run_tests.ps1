# run_tests.ps1
Param()

# 1) Compile
Write-Host "Compiling parsercodegen.c → parse.exe"
gcc .\parsercodegen.c -o parse.exe

# 2) Run tests 1–15
Write-Host ""
1..15 | ForEach-Object {
    $i = $_
    $test = Join-Path ..\test\error\input "errorin$i.txt"
    if (Test-Path $test) {
        Write-Host "---- Test #$i ($test) ----"
        & .\parse.exe $test
        Write-Host ""
    }
    else {
        Write-Warning "Missing test file: $test"
    }
}

Write-Host "All done."
