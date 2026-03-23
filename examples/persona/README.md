# Persona OOP Test Suite

This folder contains minimal and isolated tests for EmotionScript OOP features.

## Test Files and Coverage

- `basic_persona.ems`: Persona declaration + attribute declarations.
- `object_creation.ems`: Object instantiation (`Student s1;`).
- `constructor_public_ok.ems`: Constructor call on creation (`Student s1(101);`) with public field access.
- `constructor_private_error.ems`: Constructor call with private field access denial.
- `constructor_missing_error.ems`: Error when constructor arguments are passed but `init` does not exist.
- `constructor_param_error.ems`: Error when constructor argument type does not match `init` parameters.
- `member_assignment.ems`: Member assignment (`set s1.id -> 101;`).
- `member_access.ems`: Member read and print (`speak(s1.id);`).
- `inheritance.ems`: Single-level inheritance via `evolves_from`.
- `multi_level_inheritance.ems`: Multi-level chain (`A -> B -> C`).
- `method_basic.ems`: Method declared inside persona.
- `method_invocation.ems`: Method call using `invoke object.method();`.
- `method_with_params.ems`: Method parameter passing and message output.
- `method_return.ems`: Return value from method using `reflect`.
- `method_override.ems`: Override with `reshape` in child persona.
- `polymorphism.ems`: Parent and child method behavior comparison.
- `access_modifiers.ems`: `open` vs default private access enforcement.
- `multiple_objects.ems`: Independent state across multiple instances.
- `member_access_chain.ems`: Nested chain access probe (`obj.a.b`) if supported.
- `error_undeclared_object.ems`: Use object before declaration (error case).
- `error_invalid_member.ems`: Access undefined member (error case).
- `error_method_not_found.ems`: Invoke undefined method (error case).

## How To Run

From repository root:

```powershell
backend/compiler/emotionscript.exe examples/persona/basic_persona.ems examples/persona/basic_persona.out
```

Run all persona tests (PowerShell):

```powershell
$tests = Get-ChildItem examples/persona/*.ems
foreach ($t in $tests) {
  $out = [System.IO.Path]::ChangeExtension($t.FullName, ".out")
  backend/compiler/emotionscript.exe $t.FullName $out
  Write-Host "Ran $($t.Name) -> $([System.IO.Path]::GetFileName($out))"
}
```

## Notes

- Positive tests should parse and execute with expected `speak` output.
- Error tests are intentional and should emit syntax/semantic errors.
- `member_access_chain.ems` is a capability probe; current parser may reject chained access.
- Persona members are private by default; use `open` for external access.
- Object creation supports constructor-style syntax via `init` (for example, `Student s1(101);`).
