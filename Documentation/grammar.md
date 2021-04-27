# Grammar for Six10 - Version 2

This grammar is loosly based on BNF and describes the grammar that is intended for the second version of programming language Six10.

```
program : statments EOF

statments : statment+

statment : small_stmt
    | compound_stmt

small_stmt:
      assignment
    | return_stmt
    | import_stmt
    | raise_stmt
    | pass_stmt
    | delete_stmt
    | break_stmt
    | continue_stmt
    | function_call
    | expression
compound_stmt:
      function_def
    | if_stmt
    | for_stmt
    | while_stmt
    | class_stmt
    | switch_stmt
```