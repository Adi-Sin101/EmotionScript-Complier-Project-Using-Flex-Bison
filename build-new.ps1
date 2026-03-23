# EmotionScript Build Script for Windows
# Refactored to properly integrate Flex and Bison
# Usage: .\build-new.ps1 [all|build|clean|test|run]

param(
    [string]$Target = "all"
)

# Configuration
$BISON = "bison"
$FLEX = "flex"
$CC = "gcc"
$CFLAGS = "-Wall -Wextra -g -O2 -Wno-unused-label -Wno-unused-function"
$EXECUTABLE = "emotionscript.exe"

# Generated files
$LEX_FILE = "emotionscript.l"
$YACC_FILE = "emotionscript.y"
$LEX_C = "lex.yy.c"
$YACC_C = "emotionscript.tab.c"
$YACC_H = "emotionscript.tab.h"

function Write-Header {
    param([string]$msg)
    Write-Host "`n$([char]0x2500 * 60)" -ForegroundColor Cyan
    Write-Host "  $msg" -ForegroundColor Cyan
    Write-Host "$([char]0x2500 * 60)" -ForegroundColor Cyan
}

function Test-Tools {
    Write-Host "`n[*] Checking required tools..." -ForegroundColor Yellow
    
    $tools = @($BISON, $FLEX, $CC)
    foreach ($tool in $tools) {
        if (Get-Command $tool -ErrorAction SilentlyContinue) {
            Write-Host "    ✓ $tool found" -ForegroundColor Green
        } else {
            Write-Host "    ✗ $tool NOT found" -ForegroundColor Red
            Write-Host "      Please install: Bison, Flex, and GCC (MinGW or similar)" -ForegroundColor Yellow
            exit 1
        }
    }
}

function Build-All {
    Write-Header "EmotionScript Compiler Build"
    
    Test-Tools
    
    # Step 1: Generate parser
    Write-Host "`n[1/3] Generating parser with Bison..." -ForegroundColor Yellow
    & $BISON -d $YACC_FILE
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Bison failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Parser generated: $YACC_C, $YACC_H" -ForegroundColor Green
    
    # Step 2: Generate lexer
    Write-Host "`n[2/3] Generating lexer with Flex..." -ForegroundColor Yellow
    & $FLEX $LEX_FILE
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Flex failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Lexer generated: $LEX_C" -ForegroundColor Green
    
    # Step 3: Compile
    Write-Host "`n[3/3] Compiling and linking..." -ForegroundColor Yellow
    & $CC $CFLAGS $YACC_C $LEX_C symbol_table.c interpreter.c intermediate_code.c function_table.c persona_table.c -o $EXECUTABLE
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Compilation failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Build successful: $EXECUTABLE" -ForegroundColor Green
    
    Write-Host "`n✓ COMPILATION COMPLETE" -ForegroundColor Green
    Write-Host "`nUsage: .\$EXECUTABLE <input.ems> <output.txt>" -ForegroundColor Cyan
}

function Clean-Build {
    Write-Header "Cleaning Build Artifacts"
    
    $files = @($LEX_C, $YACC_C, $YACC_H, $EXECUTABLE, "emotionscript_lexer.exe", "emotionscript_parser.exe")
    
    foreach ($file in $files) {
        if (Test-Path $file) {
            Remove-Item $file -Force -ErrorAction SilentlyContinue
            Write-Host "  ✓ Removed: $file" -ForegroundColor Green
        }
    }
    
    Write-Host "`n✓ Clean complete" -ForegroundColor Green
}

function Run-Test {
    param([string]$InputFile = "test.ems", [string]$OutputFile = "syntax_output.txt")
    
    if (-not (Test-Path $EXECUTABLE)) {
        Write-Host "✗ Executable not found. Building first..." -ForegroundColor Yellow
        Build-All
    }
    
    Write-Header "Running EmotionScript Compiler"
    
    if (-not (Test-Path $InputFile)) {
        Write-Host "✗ Input file not found: $InputFile" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "Input:  $InputFile" -ForegroundColor Cyan
    Write-Host "Output: $OutputFile" -ForegroundColor Cyan
    Write-Host ""
    
    & ".\$EXECUTABLE" $InputFile $OutputFile
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`n✓ Compilation completed" -ForegroundColor Green
        Write-Host "`nOutput file contents:" -ForegroundColor Yellow
        Write-Host "$([char]0x2500 * 60)" -ForegroundColor DarkGray
        Get-Content $OutputFile
        Write-Host "$([char]0x2500 * 60)" -ForegroundColor DarkGray
    } else {
        Write-Host "`n✗ Compilation failed" -ForegroundColor Red
    }
}

function Show-Help {
    Write-Host @"
EmotionScript Compiler Build System
====================================

Usage: .\build-new.ps1 [Target]

Targets:
  all     - Build the complete compiler (default)
  build   - Same as 'all'
  clean   - Remove all generated files
  test    - Build and test on test.ems
  run     - Same as 'test'

Examples:
  .\build-new.ps1           # Builds the compiler
  .\build-new.ps1 clean     # Cleans build artifacts
  .\build-new.ps1 test      # Builds and tests

After building, run the compiler:
  .\emotionscript.exe test.ems output.txt
"@
}

# Main
switch ($Target.ToLower()) {
    "all" { Build-All }
    "build" { Build-All }
    "clean" { Clean-Build }
    "test" { Build-All; Run-Test }
    "run" { Build-All; Run-Test }
    "help" { Show-Help }
    default { Show-Help }
}
