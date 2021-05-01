# Grammar for Six10 - Version 2

This grammar is loosly based on BNF and describes the grammar that is intended for the second version of programming language Six10.

## Notation used:
|Symbol       |description |
|-------------|------------|
|`*`          |Zero or more|
|`+`          |One or more|
|`?`          |Zero or one|
|`IDENTIFIER` |Terminal token, [a-zA-Z][a-zA-Z0-9]*|
|`NEW_LINE`   |Terminal token, '\n'|
|`PASS`       |Terminal token, 'pass'|
|`BREAK`      |Terminal token, 'break'|
|`CONTINUE`   |Terminal token, 'continue'|
|`STRING`     |Terminal token, ['"][.]['"]|
|`NUMBER`     |Terminal token, '-'? [0] ('.' [0-9]\*)?  \|  '-'? [1-9] [0-9]* ('.' [0-9]\*)? |
|`SINGLE_LINE_COMMENT`|Terminal token, '#' `STRING`|
|`MULTI_LINE_COMMENT`|Terminal token, ' """ ' `STRING` ' """ '|
|`INDENT`|Terminal token, adds four spaces to current indentation level|
|`DEDENT`|Terminal token, removes four spaces to current indentation level|


```
program: 
    statements EOF

statements: 
    statement+

statement: 
    small_stmt NEW_LINE
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
  | SINGLE_LINE_COMMENT

compound_stmt:
    function_definition
  | if_stmt
  | for_stmt
  | while_stmt
  | class_definition
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
    'return' expression*

import_stmt:
    'import' IDENTIFIER ('as' IDENTIFIER)?

raise_stmt:
    'raise' expression

delete_stmt:
    'delete' expression
   
expression:
    expression IF expression ELSE expression
  | disjunction

disjunction:
    conjunction (OR conjunction)+
  | conjunction

conjunction:
    invertion (AND invertion)+
  | invertion

invertion:
    NOT invertion
  | comparison

comparison:
    bitwise_or compare_pair+
  | bitwise_or

bitwise_or:
    bitwise_or '|' bitwise_xor 
  | bitwise_xor

bitwise_xor:
    bitwise_xor '^' bitwise_and 
  | bitwise_and

bitwise_and:
    bitwise_and '&' shift_expr 
  | shift_expr

shift_expr:
    shift_expr '<<' sum 
  | shift_expr '>>' sum 
  | sum

lte_bitwise_or: 
    '<=' bitwise_or 

lt_bitwise_or: 
    '<' bitwise_or 

gte_bitwise_or: 
    '>=' bitwise_or 

gt_bitwise_or: 
    '>' bitwise_or 

notin_bitwise_or: 
    'not' 'in' bitwise_or 

in_bitwise_or: 
    'in' bitwise_or 

isnot_bitwise_or: 
    'is' 'not' bitwise_or 

is_bitwise_or: 
    'is' bitwise_or

sum:
    sum '+' term 
  | sum '-' term 
  | term
term:
    term '*' factor 
  | term '/' factor 
  | term '%' factor 
  | term '//' factor
  | factor
factor:
    '+' factor 
  | '-' factor 
  | power
power:
    primary

primary:
    primary '.' IDENTIFIER 
  | primary '(' arguments* ')' 
  | primary '[' slices ']' 
  | atom

slice:
    slice (',' slice)*

atom:
    IDENTIFIER
  | literal
  | tuple
  | list
  | (dict | set)
  | '(' expression ')'

literal:
    'True'
  | 'False'
  | 'None'
  | STRING
  | NUMBER

tuple:
    '(' ')'
  | '(' expression tuple_end

tuple_end:
    ',' ')'
  | (',' expression)+ ')'

expression_list:
    expression (',' expression)*

dict:
    | '{' kv_pairs '}' 

kv_pairs:
    '{' kv_pair (',' kv_pairs)* '}'

kv_pair:
    expression ':' expression

arguments:
    argument (',' arguments)* 

argument:
    expression

function_definition:
    'def' IDENTIFIER '(' parameters? ')' ':' dockstring? scope_block

parameters:
    parameter_no_default (',' parameter_no_default)* (',' parameters_default)*

parameter_default:
    IDENTIFIER '=' expression

parameter_no_default:
    IDENTIFIER

dockstring:
    NEWLINE MULTI_LINE_COMMENT NEWLINE

scope_block:
    NEWLINE INDENT statements DEDENT

class_definition:
    'class' IDENTIFIER ('(' arguments? ')')? ':' scope_block 

if_stmt:
    'if' expression ':' scope_block elif_block?

elif_block:
    'elif' ':' scope_block elif_block?
  | else_block

else_block:
    'else' ':' scope_block

for_stmt:
    'for' targets 'in' expression ':' scope_block else_block?

while_stmt:

class_definition:

switch_stmt:


```