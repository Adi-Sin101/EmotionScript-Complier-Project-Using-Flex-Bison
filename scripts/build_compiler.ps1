$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir
$compilerDir = Join-Path $rootDir "backend\compiler"

Push-Location $compilerDir
try {
  Write-Host "==> Running Flex (lexer)..."
  flex emotionscript.l

  Write-Host "==> Running Bison (parser)..."
  bison -d emotionscript.y

  Write-Host "==> Compiling EmotionScript compiler..."
  gcc `
    -Wall `
    -Wextra `
    -g `
    -O2 `
    -Wno-unused-label `
    -Wno-unused-function `
    emotionscript.tab.c `
    lex.yy.c `
    symbol_table.c `
    interpreter.c `
    intermediate_code.c `
    function_table.c `
    persona_table.c `
    -o emotionscript.exe

  Write-Host "==> Build complete: $compilerDir\emotionscript.exe"
}
finally {
  Pop-Location
}
