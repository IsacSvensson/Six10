# Grammar for Six10 - Version 2

This grammar is loosly based on BNF and describes the grammar that is intended for the second version of programming language Six10.

```
program : statments EOF

statments : statment+

statment : small_stmt NEW_LINE
  | compound_stmt NEW_LINE

small_stmt:
    assignment
  | return_stmt
  | import_stmt
  | raise_stmt
  | PASS
  | delete_stmt
  | BREAK
  | CONTINUE
  | expression

compound_stmt:
    function_def
  | if_stmt
  | for_stmt
  | while_stmt
  | class_stmt
  | switch_stmt

assignment:
    expression assignment_operator expression

assignment_operator:
    '='
  | '+='
  | '-='
  | '*='
  | '/='
  | '%='
  | '&='
  | '|='
  | '^='
  | '<<='
  | '>>='
  | '**='
  | '//='

return_stmt:
    RETURN expression*

import_stmt:
    'import' IDENTIFIER ('as' IDENTIFIER)?

raise_stmt:
    'raise' expression

delete_stmt:
    DELETE expression
   
expression:
    expression IF expression ELSE expression
  | disjunction

disjunction:
    conjunction (OR conjunction)+
  | conjunction

conjunction:
    invertion (AND invertion)
  | invertion

invertion:
    NOT comparison
  | comparison

comparison:
    

```