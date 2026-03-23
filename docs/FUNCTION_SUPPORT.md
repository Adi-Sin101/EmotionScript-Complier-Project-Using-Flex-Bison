# Function Support in EmotionScript

## Overview
EmotionScript now supports function declarations, parameters, return values, and function invocation with full local scope management.

## Features Implemented

### 1. Function Declarations
Define functions using the `interpret` keyword:

```
interpret function_name(type param1, type param2) yields return_type <<
    function_body
>>
```

**Example:**
```
interpret add(count a, count b) yields count <<
    reflect a + b;
>>
```

### 2. Parameters
- Functions can accept multiple parameters
- Parameter types: `count`, `measure`, `truth`, `words`, `emotion`, `level`
- Parameters are passed by value
- Parameter scope is strictly local to the function

**Example with multiple parameters:**
```
interpret multiply(count x, count y) yields count <<
    reflect x * y;
>>
```

### 3. Return Values
- Use `reflect` keyword to return a value from a function
- Return type is specified in the function declaration with `yields`
- Function execution stops at the `reflect` statement

**Example:**
```
interpret get_answer() yields count <<
    reflect 42;
>>
```

### 4. Function Invocation
Call functions using the `invoke` keyword:

```
invoke function_name(arg1, arg2);
```

Or as part of an expression:
```
invoke function_name(arg1, arg2)
```

**Example:**
```
invoke add(5, 6);
```

### 5. Local Scope Management
- Each function call creates a new local scope
- Parameters exist only within the function scope
- Variables declared in a function are local to that scope
- Parent scope is accessible for lookups (lexical scoping)
- Function return values replace the function call expression

### 6. Type System
Function parameters and return values respect EmotionScript's type system:
- `count`: integer values
- `measure`: floating-point values
- `truth`: boolean values
- `words`: string values
- `emotion`: enumerated emotions
- `level`: integer level/ranking values

Type conversions are handled automatically where compatible.

## Architecture

### New Files Added
1. **function_table.h / function_table.c**: Hash table for storing function definitions
   - Stores function name, parameter types, parameter names, return type, and body
   - Supports function lookup by name

2. **Updated emotionscript.y**:
   - Added function call expression type (EXPR_FUNC_CALL)
   - Added function declaration and return statement types
   - Added scope chain structure for local variable management
   - Updated variable lookup to use scope-aware symbol resolution
   - Proper parameter binding and function invocation logic

### Key Components

#### Scope Structure
```c
struct Scope {
    Symbol *local_vars[SYMBOL_TABLE_SIZE];
    RuntimeValue return_value;
    struct Scope *parent;
};
```

#### Function Definition Storage
```c
typedef struct FunctionDef {
    char *name;
    SymbolType return_type;
    char **param_names;
    SymbolType *param_types;
    int param_count;
    void *body;  /* Points to Block* */
    struct FunctionDef *next;
} FunctionDef;
```

## Execution Flow

### Function Definition
1. Parser creates ST_FUNC_DECL statement with parameter and body information
2. During execution, function is registered in the global function table
3. Function body is stored as an AST block without execution

### Function Call
1. Parser creates EXPR_FUNC_CALL expression with arguments
2. During evaluation:
   - Function lookup in function table
   - Argument count validation
   - New scope creation with current scope as parent
   - Parameter evaluation in caller's scope
   - Parameter binding in new scope
   - Function body execution in new scope
   - Return value capture
   - Scope cleanup
3. Return value replaces function call expression

## Limitations and Future Work

### Current Limitations
- No recursive functions
- No nested function definitions
- No default parameter values
- No variable argument lists
- No higher-order functions

### Future Enhancements
- Recursive function support
- Nested function definitions  
- Default parameters
- Variable arguments
- Method syntax for object-oriented features
- Function pointers/references

## Examples

### Basic Arithmetic
```
interpret add(count a, count b) yields count <<
    reflect a + b;
>>

invoke add(5, 6);  // Returns 11
```

### Multiple Functions
```
interpret multiply(count x, count y) yields count <<
    reflect x * y;
>>

interpret add(count a, count b) yields count <<
    reflect a + b;
>>

invoke multiply(3, 7);  // Returns 21
invoke add(10, 20);     // Returns 30
```

### Mixed Types
```
interpret divide(measure a, measure b) yields measure <<
    reflect a / b;
>>

invoke divide(10.5, 2.5);  // Returns 4.2
```

## Testing
- Main test case: `test_function.ems` - demonstrates basic add function
- Demo test case: `examples/functions/functions_demo.ems` - shows multiple functions

Run tests:
```bash
cd backend/compiler
./emotionscript.exe ../../test_function.ems ../../test_function.out
./emotionscript.exe ../../examples/functions/functions_demo.ems ../../examples/functions/functions_demo.out
```

## Implementation Notes

### Key Design Decisions
1. **Scope Chain**: Used parent pointer to form a scope chain, enabling efficient lexical scoping
2. **Type Checking**: Strict type checking at bind time ensures type safety
3. **Parameter By Value**: All parameters are evaluated in caller's scope before binding, preventing unintended side effects
4. **Return Mechanism**: Special EXEC_RETURN signal facilitates clean return from nested execution contexts
5. **Symbol Update**: Fixed runtime_to_symbol to update symbols directly instead of re-looking them up

### Known Issues Resolved
- Parameter type mismatch was caused by symbol lookup using global table instead of local scope
- Function argument list memory corruption was fixed by proper type handling in grammar union
