"""
A list of all terminal tokens in the language. 
"""

_INT = 'int'
_FLOAT = 'float'
_BIN = 'bin'
_OCT = 'oct'
_HEX = 'hex'
_STRING = 'string'
_IDENTIFIER = 'identifier'
_RETURN = 'return'
_PASS = 'pass'
_BREAK = 'break'
_CONTINUE = 'continue'
_IMPORT = 'import'
_AS = 'as'
_RAISE  = 'raise'
_DELETE = 'delete'
_IF = 'if'
_ELIF = 'elif'
_ELSE = 'else'
_OR = 'or'
_AND = 'and'
_NOT = 'not'
_NOT_IN = 'not_in'
_IN = 'in'
_BITWISE_IS = 'is'
_BITWISE_IS_NOT = 'is'
_BITWISE_OR = 'bw_or'
_BITWISE_XOR = 'xor'
_BITWISE_AND = 'bw_and'
_BITWISE_LSHIFT = 'left_shift'
_BITWISE_RSHIFT = 'right_shift'
_BITWISE_LT = 'lt'
_BITWISE_LTE = 'lte'
_BITWISE_GT = 'gt'
_BITWISE_GTE = 'gte'
_BITWISE_EQ = 'eq'
_BITWISE_NEQ = 'neq'
_PLUS = 'plus'
_MINUS = 'minus'
_MULT = 'multiplication'
_EXP = 'exponentiation'
_DIV = 'division'
_MOD = 'modulus'
_FLOOR = 'floor_division'
_DOT = 'dot'
_LPARAN = 'left_parenthesis'
_RPARAN = 'right_parenthesis'
_LSQBRACK = 'left_squarebracket'
_RSQBRACK = 'right_squarebracket'
_TRUE = 'true'
_FALSE = 'false'
_NONE = 'none'
_COMMA = 'comma'
_COLON = 'colon'
_LCURLBRACK = 'left_curly_bracket'
_RCURLBRACK = 'right_curly_bracket'
_DEF = 'def'
_CLASS = 'class'
_COMMENT = 'comment'
_MULTI_COMMENT = 'multi_line_comment'
_INDENT = 'indent'
_DEDENT = 'dedent'
_NEWLINE = 'new_line'
_FOR = 'for'
_WHILE = 'while'
_SWITCH = 'switch'
_CASE = 'case'
_ASSIGN = 'assign'
_PLUS_ASSIGN = 'plus_assign'
_MINUS_ASSIGN = 'minus_assign'
_MULT_ASSIGN = 'multiplication_assign'
_DIV_ASSIGN = 'division_assign'
_MOD_ASSIGN = 'modulus_assign'
_AND_ASSIGN = 'and_assign'
_OR_ASSIGN = 'or_assign'
_XOR_ASSIGN = 'xor_assign'
_LSHIFT_ASSIGN = 'left_shift_assign'
_RSHIFT_ASSIGN = 'right_shift_assign'
_POWER_ASSIGN = 'power_assign'
_FLOOR_ASSIGN = 'floor_division_assign'
_INCR = 'increment'
_DECR = 'decrement'
_INVALID = 'invalid_token'
_EOF = 'end_of_file'

_CMP_STMT = [_DEF, _IF, _FOR, _WHILE, _CLASS, _SWITCH]
_COMPARISON = [_BITWISE_EQ, _BITWISE_NEQ, _BITWISE_LTE, _BITWISE_LT, _BITWISE_GTE, 
    _BITWISE_GT, _BITWISE_IS, _BITWISE_IS_NOT, _IN, _NOT, _NOT_IN, _BITWISE_AND, 
    _BITWISE_OR, _BITWISE_XOR, _BITWISE_LSHIFT, _BITWISE_RSHIFT
]

_ASSIGN_OPS = [
    _ASSIGN, _PLUS_ASSIGN, _MINUS_ASSIGN, _MULT_ASSIGN, _DIV_ASSIGN, _MOD_ASSIGN, 
    _AND_ASSIGN, _OR_ASSIGN, _XOR_ASSIGN, _LSHIFT_ASSIGN, _RSHIFT_ASSIGN, 
    _POWER_ASSIGN, _FLOOR_ASSIGN
]

_NUMBER_TOKENS = [_INT, _FLOAT, _BIN, _OCT, _HEX]

_PARANS = [_LPARAN, _LSQBRACK]