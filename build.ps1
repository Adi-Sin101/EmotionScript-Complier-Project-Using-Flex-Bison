# EmotionScript Build Script for Windows
# Usage: .\build.ps1 [lexer|parser|both|clean|test]

param(
    [string]$Target = "both"
)

$LEXER_EXE = "emotionscript_lexer.exe"
$PARSER_EXE = "emotionscript_parser.exe"

function Build-Lexer {
    Write-Host "`n=== Building Standalone Lexer ===" -ForegroundColor Cyan
    
    # Generate lexer C code
    Write-Host "Running flex..." -ForegroundColor Yellow
    flex emotionscript.l
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Flex failed" -ForegroundColor Red
        return $false
    }
    
    # Compile lexer
    Write-Host "Compiling lexer..." -ForegroundColor Yellow
    gcc -Wall -g lex.yy.c -o $LEXER_EXE
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Compilation failed" -ForegroundColor Red
        return $false
    }
    
    Write-Host "✓ Lexer built successfully: $LEXER_EXE" -ForegroundColor Green
    return $true
}

function Build-Parser {
    Write-Host "`n=== Building Parser with Integrated Lexer ===" -ForegroundColor Cyan
    
    # Generate parser C code and header
    Write-Host "Running bison..." -ForegroundColor Yellow
    bison -d emotionscript.y
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Bison failed" -ForegroundColor Red
        return $false
    }
    
    # Generate lexer in Bison mode
    Write-Host "Running flex in Bison mode..." -ForegroundColor Yellow
    $env:CFLAGS = "-DBISON_MODE"
    flex emotionscript.l
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Flex failed" -ForegroundColor Red
        return $false
    }
    
    # Compile parser with lexer
    Write-Host "Compiling parser..." -ForegroundColor Yellow
    gcc -Wall -g -DBISON_MODE emotionscript.tab.c lex.yy.c -o $PARSER_EXE
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Compilation failed" -ForegroundColor Red
        return $false
    }
    
    Write-Host "✓ Parser built successfully: $PARSER_EXE" -ForegroundColor Green
    return $true
}

function Test-Lexer {
    Write-Host "`n=== Testing Lexer ===" -ForegroundColor Cyan
    
    if (-not (Test-Path $LEXER_EXE)) {
        Write-Host "✗ Lexer not found. Building..." -ForegroundColor Yellow
        Build-Lexer
    }
    
    Write-Host "Running lexer on test.ems..." -ForegroundColor Yellow
    & ".\$LEXER_EXE" test.ems token.out
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Tokens generated in token.out" -ForegroundColor Green
        Write-Host "`nFirst 20 lines of output:" -ForegroundColor Cyan
        Get-Content token.out -TotalCount 20
    } else {
        Write-Host "✗ Lexer test failed" -ForegroundColor Red
    }
}

function Test-Parser {
    Write-Host "`n=== Testing Parser ===" -ForegroundColor Cyan
    
    if (-not (Test-Path $PARSER_EXE)) {
        Write-Host "✗ Parser not found. Building..." -ForegroundColor Yellow
        Build-Parser
    }
    
    Write-Host "Running parser on test.ems..." -ForegroundColor Yellow
    & ".\$PARSER_EXE" test.ems syntax.out
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Syntax validation completed" -ForegroundColor Green
        Write-Host "`nValidation result:" -ForegroundColor Cyan
        Get-Content syntax.out
    } else {
        Write-Host "✗ Parser test failed" -ForegroundColor Red
    }
}

function Clean-Build {
    Write-Host "`n=== Cleaning Build Files ===" -ForegroundColor Cyan
    
    $files = @(
        "lex.yy.c",
        "emotionscript.tab.c",
        "emotionscript.tab.h",
        $LEXER_EXE,
        $PARSER_EXE
    )
    
    foreach ($file in $files) {
        if (Test-Path $file) {
            Remove-Item $file
            Write-Host "Removed: $file" -ForegroundColor Yellow
        }
    }
    
    Write-Host "✓ Clean complete" -ForegroundColor Green
}

function Show-Help {
    Write-Host "`nEmotionScript Build Script" -ForegroundColor Cyan
    Write-Host "=========================="
    Write-Host "`nUsage: .\build.ps1 [target]"
    Write-Host "`nTargets:"
    Write-Host "  lexer   - Build standalone lexer only"
    Write-Host "  parser  - Build parser with integrated lexer"
    Write-Host "  both    - Build both lexer and parser (default)"
    Write-Host "  test    - Run all tests"
    Write-Host "  clean   - Remove generated files"
    Write-Host "  help    - Show this help"
    Write-Host "`nExamples:"
    Write-Host "  .\build.ps1"
    Write-Host "  .\build.ps1 lexer"
    Write-Host "  .\build.ps1 parser"
    Write-Host "  .\build.ps1 test"
    Write-Host "`nManual Usage:"
    Write-Host "  Lexer:  .\$LEXER_EXE input.ems output.tokens"
    Write-Host "  Parser: .\$PARSER_EXE input.ems output.syntax"
}

# Main execution
switch ($Target.ToLower()) {
    "lexer" {
        Build-Lexer
    }
    "parser" {
        Build-Parser
    }
    "both" {
        $lexerOk = Build-Lexer
        $parserOk = Build-Parser
        
        if ($lexerOk -and $parserOk) {
            Write-Host "`n✓ Both builds completed successfully" -ForegroundColor Green
        }
    }
    "test" {
        Test-Lexer
        Test-Parser
    }
    "clean" {
        Clean-Build
    }
    "help" {
        Show-Help
    }
    default {
        Write-Host "Unknown target: $Target" -ForegroundColor Red
        Show-Help
    }
}
