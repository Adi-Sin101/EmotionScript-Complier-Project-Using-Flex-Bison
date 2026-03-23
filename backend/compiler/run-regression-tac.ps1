param(
    [switch]$VerboseLogs,
    [switch]$StopOnFailure,
    [string[]]$ExpectedFailPatterns = @("member_access_chain.ems")
)

$ErrorActionPreference = "Stop"

function Write-Step {
    param([string]$Message)
    Write-Host "`n[+] $Message" -ForegroundColor Cyan
}

function Invoke-Checked {
    param(
        [scriptblock]$Action,
        [string]$OnFail
    )

    & $Action
    if ($LASTEXITCODE -ne 0) {
        throw $OnFail
    }
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptRoot "..\..")
$examplesRoot = Join-Path $repoRoot "examples"
$compilerExe = Join-Path $scriptRoot "emotionscript.exe"

if (-not (Test-Path $examplesRoot)) {
    throw "Examples folder not found: $examplesRoot"
}

Push-Location $scriptRoot
try {
    Write-Step "Rebuilding compiler (Bison/Flex/GCC)"

    Invoke-Checked -Action { bison -d emotionscript.y } -OnFail "bison failed"
    Invoke-Checked -Action { flex emotionscript.l } -OnFail "flex failed"
    Invoke-Checked -Action {
        gcc -Wall -Wextra -g -O2 -Wno-unused-label -Wno-unused-function emotionscript.tab.c lex.yy.c symbol_table.c interpreter.c intermediate_code.c function_table.c persona_table.c -o emotionscript.exe
    } -OnFail "gcc build failed"

    if (-not (Test-Path $compilerExe)) {
        throw "Compiler executable was not generated: $compilerExe"
    }

    Write-Step "Running TAC regression over all examples/**/*.ems"

    $tests = Get-ChildItem -Path $examplesRoot -Filter *.ems -Recurse | Sort-Object FullName
    if ($tests.Count -eq 0) {
        throw "No .ems example files found under: $examplesRoot"
    }

    $passed = 0
    $failed = 0
    $expectedFailed = 0
    $failedFiles = @()

    foreach ($test in $tests) {
        $outPath = [System.IO.Path]::ChangeExtension($test.FullName, ".out")
        $tacPath = [System.IO.Path]::ChangeExtension($test.FullName, ".tac")

        if ($VerboseLogs) {
            Write-Host "  -> $($test.FullName)" -ForegroundColor DarkGray
        }

        & $compilerExe $test.FullName $outPath $tacPath

        if ($LASTEXITCODE -eq 0 -and (Test-Path $tacPath)) {
            $passed++
            Write-Host "  [PASS] $($test.Name)" -ForegroundColor Green
        } else {
            $isExpectedFail = $false
            foreach ($pattern in $ExpectedFailPatterns) {
                if ($test.Name -like $pattern) {
                    $isExpectedFail = $true
                    break
                }
            }

            if ($isExpectedFail) {
                $expectedFailed++
                Write-Host "  [XFAIL] $($test.Name)" -ForegroundColor Yellow
            } else {
                $failed++
                $failedFiles += $test.FullName
                Write-Host "  [FAIL] $($test.Name)" -ForegroundColor Red
                if ($StopOnFailure) {
                    break
                }
            }
        }
    }

    Write-Step "Regression summary"
    Write-Host "  Total : $($tests.Count)"
    Write-Host "  Passed: $passed" -ForegroundColor Green
    Write-Host "  XFail : $expectedFailed" -ForegroundColor Yellow
    Write-Host "  Failed: $failed" -ForegroundColor Red

    if ($failed -gt 0) {
        Write-Host "`nFailed files:" -ForegroundColor Yellow
        $failedFiles | ForEach-Object { Write-Host "  - $_" }
        exit 1
    }

    Write-Host "`nAll example programs generated syntax + TAC outputs successfully." -ForegroundColor Green
}
finally {
    Pop-Location
}
