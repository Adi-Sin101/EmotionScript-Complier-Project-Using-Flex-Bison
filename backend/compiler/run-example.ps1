param(
    [string]$File = "examples/variables/declare.ems",
    [string]$Out = ""
)

$root = Resolve-Path "$PSScriptRoot/../.."
$inputPath = Join-Path $root $File

if (-not (Test-Path $inputPath)) {
    Write-Error "Example not found: $inputPath"
    exit 1
}

if ($Out -eq "") {
    $Out = [System.IO.Path]::ChangeExtension($File, ".out")
}

$outputPath = Join-Path $root $Out
$outputDir = Split-Path $outputPath -Parent
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
}

Push-Location $PSScriptRoot
try {
    bison -d emotionscript.y
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    flex emotionscript.l
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    gcc -Wall -Wextra -g -O2 emotionscript.tab.c lex.yy.c symbol_table.c interpreter.c intermediate_code.c -o emotionscript.exe
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    .\emotionscript.exe $inputPath $outputPath
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    Write-Host "Output written to: $outputPath"
    Get-Content $outputPath
}
finally {
    Pop-Location
}
