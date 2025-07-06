# run_tests.ps1
Param()

# 1) Compile
Write-Host "Compiling parsercodegen.c → parse.exe"
gcc .\parsercodegen.c -o parse.exe

# 2) Run error tests 1–15
Write-Host ""
1..15 | ForEach-Object {
    $i     = $_
    $test  = Join-Path ..\test\error\input "errorin$i.txt"
    if (Test-Path $test) {
        Write-Host "---- Error Test #$i ($test) ----"
        & .\parse.exe $test
        Write-Host ""
    }
    else {
        Write-Warning "Missing error test file: $test"
    }
}

# 3) Run no-error test
$noErrTest = Join-Path ..\test\no_error\input "noerror1.txt"
Write-Host "---- No-Error Test ($noErrTest) ----"
if (Test-Path $noErrTest) {
    & .\parse.exe $noErrTest
    Write-Host "No errors detected."
}
else {
    Write-Warning "Missing no-error test file: $noErrTest"
}

Write-Host "All done."

