# Grammar Reference (Summary)

## Program Structure
A program starts with mind <Identifier>, then awake, statements, and ends with sleep.

## Statement Families
- declarations
- assignments
- expressions
- conditionals
- loops
- function declarations/calls
- io statements (speak, listen, alert)
- persona and fsm constructs

## Conditional Forms
- if_feel <condition> ... end_feel
- if_feel ... else_if_feel ... end_feel
- if_feel ... else_feel ... end_feel

## Loop Forms
- ruminate <condition> ... end_ruminate
- replay (set x -> init; condition; step) ... end_ruminate

## Flow Control
- break_free;
- continue_flow;

## Notes
The parser validates syntax and drives execution/trace behavior through integrated semantic actions.
