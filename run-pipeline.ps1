param(
    [string]$InputFile = "examples/variables/declare.ems",
    [string]$OutputDir = "pipeline_output",
    [switch]$Rebuild
)

$ErrorActionPreference = "Stop"

function Write-Step {
    param([string]$Message)
    Write-Host "`n[PIPELINE] $Message" -ForegroundColor Cyan
}

function Invoke-Checked {
    param(
        [scriptblock]$Action,
        [string]$ErrorMessage
    )

    & $Action
    if ($LASTEXITCODE -ne 0) {
        throw $ErrorMessage
    }
}

$repoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$compilerDir = Join-Path $repoRoot "backend\compiler"
$compilerExe = Join-Path $compilerDir "emotionscript.exe"

if (-not [System.IO.Path]::IsPathRooted($InputFile)) {
    $inputPath = Join-Path $repoRoot $InputFile
} else {
    $inputPath = $InputFile
}

if (-not (Test-Path $inputPath)) {
    throw "Input file not found: $inputPath"
}

if (-not [System.IO.Path]::IsPathRooted($OutputDir)) {
    $outputRoot = Join-Path $repoRoot $OutputDir
} else {
    $outputRoot = $OutputDir
}

$inputBase = [System.IO.Path]::GetFileNameWithoutExtension($inputPath)
$inputFolder = [System.IO.Path]::GetFileName((Split-Path $inputPath -Parent))
$artifactDir = Join-Path $outputRoot $inputFolder
New-Item -ItemType Directory -Force -Path $artifactDir | Out-Null

$syntaxOut = Join-Path $artifactDir "$inputBase.syntax.txt"
$tacOut = Join-Path $artifactDir "$inputBase.tac"
$executionOut = Join-Path $artifactDir "$inputBase.execution.txt"
$finalOut = Join-Path $artifactDir "$inputBase.output.txt"

Push-Location $compilerDir
try {
    if ($Rebuild -or -not (Test-Path $compilerExe)) {
        Write-Step "Building compiler (Flex + Bison + GCC)"
        Invoke-Checked -Action { bison -d emotionscript.y } -ErrorMessage "Bison failed"
        Invoke-Checked -Action { flex emotionscript.l } -ErrorMessage "Flex failed"
        Invoke-Checked -Action {
            gcc -Wall -Wextra -g -O2 -Wno-unused-label -Wno-unused-function emotionscript.tab.c lex.yy.c symbol_table.c interpreter.c intermediate_code.c function_table.c persona_table.c -o emotionscript.exe
        } -ErrorMessage "GCC build failed"
    }

    Write-Step "Running parser and TAC generation"
    Invoke-Checked -Action { .\emotionscript.exe $inputPath $syntaxOut $tacOut } -ErrorMessage "Compiler execution failed"
}
finally {
    Pop-Location
}

Write-Step "Extracting execution stage output"
$syntaxLines = Get-Content $syntaxOut
$startMarker = "=== EXECUTION TRACE ==="
$endMarker = "=== EXECUTION COMPLETE ==="
$startIndex = [Array]::IndexOf($syntaxLines, $startMarker)
$endIndex = [Array]::IndexOf($syntaxLines, $endMarker)

if ($startIndex -ge 0 -and $endIndex -gt $startIndex) {
    $executionLines = $syntaxLines[($startIndex + 1)..($endIndex - 1)]
} else {
    $executionLines = @("<no execution trace found>")
}
$executionLines | Set-Content $executionOut

Write-Step "Extracting final user-visible output"
$finalLines = @()
foreach ($line in $executionLines) {
    if ($line -match "^\[(SPEAK|ALERT)\]\s*(.*)$") {
        $finalLines += $line
    }
}
if ($finalLines.Count -eq 0) {
    $finalLines = @("<no SPEAK/ALERT output produced>")
}
$finalLines | Set-Content $finalOut

Write-Step "Pipeline complete"
Write-Host "Input:      $inputPath" -ForegroundColor Green
Write-Host "Parser:     $syntaxOut" -ForegroundColor Green
Write-Host "TAC:        $tacOut" -ForegroundColor Green
Write-Host "Execution:  $executionOut" -ForegroundColor Green
Write-Host "Output:     $finalOut" -ForegroundColor Green
