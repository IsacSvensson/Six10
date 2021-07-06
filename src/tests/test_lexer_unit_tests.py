import pytest
from lex_token import Token
import lexer
import terminal_tokens as tt
from error import Error

@pytest.mark.parametrize("test_input,expected_result, expected_error", [
    ("a", True, None),
    ("A", True, None),
    ("AA", False, Error("TypeError: 'char' expected to have a length of 1")),
    ("", False, Error("TypeError: 'char' expected to have a length of 1")),
    ("1", False, None),
    ("Ö", False, None),
    (True, False, Error("TypeError: 'char' expected to be a string")),
    (["a"], False, Error("TypeError: 'char' expected to be a string"))
])
def test_isLetter(test_input, expected_result, expected_error):
    result, error = lexer.isLetter(test_input)
    
    assert result == expected_result and error == expected_error
@pytest.mark.parametrize("test_input,expected_result, expected_error", [
    ("a", tt._IDENTIFIER, None),
    ("A", tt._IDENTIFIER, None),
    ("", tt._INVALID, Error("ValueError: 'symbol' is not allowed to be empty string")),
    (True, tt._INVALID, Error("TypeError: 'symbol' expected to be a string")),
    (["a"], tt._INVALID, Error("TypeError: 'symbol' expected to be a string")),
    ("True", tt._TRUE, None),
    ("False", tt._FALSE, None),
    ("true", tt._IDENTIFIER, None),
    ("false", tt._IDENTIFIER, None),
    
])
def test_isBool(test_input, expected_result, expected_error):
    res, error = lexer.isBool(test_input) 

    assert res== expected_result and error==expected_error

@pytest.mark.parametrize("test_input,expected_result, expected_error", [
    ("pass", tt._PASS, None),
    ("break", tt._BREAK, None),
    ("return", tt._RETURN, None),
    ("continue", tt._CONTINUE, None),
    ("import", tt._IMPORT, None),
    ("as", tt._AS, None),
    ("raise", tt._RAISE, None),
    ("delete", tt._DELETE, None),
    ("if", tt._IF, None),
    ("else", tt._ELSE, None),
    ("elif", tt._ELIF, None),
    ("and", tt._AND, None),
    ("or", tt._OR, None),
    ("not", tt._NOT, None),
    ("in", tt._IN, None),
    ("is", tt._BITWISE_IS, None),
    ("def", tt._DEF, None),
    ("class", tt._CLASS, None),
    ("for", tt._FOR, None),
    ("while", tt._WHILE, None),
    ("switch", tt._SWITCH, None),
    ("case", tt._CASE, None),
    ("True", tt._TRUE, None),
    ("False", tt._FALSE, None),
    ("true", tt._IDENTIFIER, None),
    ("false", tt._IDENTIFIER, None),
    ("Test123", tt._IDENTIFIER, None),
    ("", tt._INVALID, Error("ValueError: 'symbol' is not allowed to be empty string")),
    (True, tt._INVALID, Error("TypeError: 'symbol' expected to be a string")),
    ])
def test_isKeyword(test_input, expected_result, expected_error):
    res, error = lexer.isKeyword(test_input) 
    assert res == expected_result and error == expected_error

@pytest.mark.parametrize("source_code, file_name", [
    ("", ""),
    ("test", "aFile.six10"),
    ("123", "aFile.six10"),
    ("ÅÄÖ", "aFile.six10"),
    ("!\"?", "aFile.six10"),
    ("", "aFile.six10"),
    ("test", ""),
    ("123", "aFile"),
    ("ÅÄÖ", "aFile.six10"),
    ("!\"?", "aFile.six10"),
    ("", "aFile.six10"),
])
def test_accepted_init_lexer(source_code, file_name):
    l = None
    if source_code == "" and file_name == "":
        l = lexer.Lexer()
        file_name = "CLI"
    elif source_code == "":
        l = lexer.Lexer(filename=file_name)
    elif file_name == "":
        l = lexer.Lexer(source_code=source_code)
        file_name = "CLI"
    else:    
        l = lexer.Lexer(source_code, file_name)

    assert isinstance(l, (lexer.Lexer)) and (l.error is None) and \
        (l.filename == file_name) and (l.source_code == source_code)

@pytest.mark.parametrize("source_code, file_name, error", [
    (None, None, Error("Error: expected 'str' as source_code")),
    ([], "file.six10", Error("Error: expected 'str' as source_code")),
    ("5+5", ("test",), Error("Error: expected 'str' as filename"))
])
def test_failed_init_lexer(source_code, file_name, error):
    l = lexer.Lexer(source_code, file_name)

    assert isinstance(l.error, Error) and l.error == error

@pytest.mark.parametrize("source_code, num_of_advances, expected_result, expected_cur_char", [
    ("", 0, False, None),
    ("", 1, False, None),
    ("5+10==15", 0, True, '5'),
    ("5+10==15", 1, True, '+'),
    ("5+10==15", 7, False, '5'),
    ("5+10==15", 8, False, None)
])
def test_lexer_advance(source_code, num_of_advances, expected_result, expected_cur_char):
    l = lexer.Lexer(source_code)
    
    for i in range(num_of_advances):
        l.advance()

    assert l.current_character == expected_cur_char and expected_result == l.advance()

@pytest.mark.parametrize("source_code, look_ahead_size, expected_result, error", [
    ("5+10==15", "15", None, Error("Error: count is expected to be an int")),
    ("", -5, None, Error("Error: count is expected be a positive integer")),
    ("", 0, None, Error("Error: count is expected be a positive integer")),
    ("", 1, None, None),
    ("", 5, None, None),
    ("5+10==15", 0, None, Error("Error: count is expected be a positive integer")),
    ("5+10==15", 1, "+", None),
    ("5+10==15", 7, "+10==15", None),
    ("5+10==15", 8, None, None)
])
def test_lexer_lookahead(source_code, look_ahead_size, expected_result, error):
    l = lexer.Lexer(source_code)
    
    if look_ahead_size == "":
        res = l.look_ahead()
    else:
        res = l.look_ahead(look_ahead_size)

    assert (res == expected_result) and (l.error == error)


@pytest.mark.parametrize("source_code, allowed_chars, expected_result, error", [
    ("5+10==15", "0123456789", True, None),
    ("abc", "A", True, None),
    ("ABC", "a", True, None),
    ("ABC", "A", True, None),
    ("abc", "a", True, None),
    ("5+10==15", "abc", False, None),
    ("", "abc", False, Error("Error: Unexpected end of source code.")),
    ("5+10==15", "", False, Error("Error: No characters to allow entered.")),
    ("5+10==15", None, False, Error("Error: No characters to allow entered."))
])
def test_lexer_allowed_character(source_code, allowed_chars, expected_result, error):
    l = lexer.Lexer(source_code)
    res = l.allowed_character(allowed_chars)

    assert (res == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ("51015", 51015, None),
    ("0", 0, None),
    ("1", 1, None),
    ("0.001", 0.001, None),
    ("1000.6", 1000.6, None),
    ("a", "a", Error("ValueError: Expected a digit or dot '.'"))
])
def test_lexer_make_decimal(source_code, expected_result, error):
    l = lexer.Lexer(source_code)

    res = l.make_decimal()

    assert (res.value == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ("0b0", 0, None),
    ("0b0000", 0, None),
    ("0b1", 1, None),
    ("0b0001", 1, None),
    ("0b1111", 15, None),
    ("0b100000000", 256, None),
    ("0b", "0b" ,Error("ValueError: Can not convert to a number")),
    ("0x", "0x" ,Error("ValueError: Can not convert to a number"))
])
def test_lexer_make_binary(source_code, expected_result, error):
    l = lexer.Lexer(source_code)

    res = l.make_binary()

    assert (res.value == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ("0o0", 0, None),
    ("0o0000", 0, None),
    ("0o1", 1, None),
    ("0o7", 7, None),
    ("0o10", 8, None),
    ("0o17", 15, None),
    ("0o0017", 15, None),
    ("0o10000", 4096, None),
    ("0o", "0o" ,Error("ValueError: Can not convert to a number")),
    ("0x", "0x" ,Error("ValueError: Can not convert to a number"))
])
def test_lexer_make_octal(source_code, expected_result, error):
    l = lexer.Lexer(source_code)

    res = l.make_octodecimal()

    assert (res.value == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ("0x0", 0, None),
    ("0x0000", 0, None),
    ("0x1", 1, None),
    ("0xa", 10, None),
    ("0xF", 15, None),
    ("0xFF", 255, None),
    ("0x100", 256, None),
    ("0x0000aA", 170, None),
    ("0x10000", 65536, None),
    ("0x", "0x" ,Error("ValueError: Can not convert to a number")),
    ("0b", "0b" ,Error("ValueError: Can not convert to a number"))
])
def test_lexer_make_hexadecimal(source_code, expected_result, error):
    l = lexer.Lexer(source_code)

    res = l.make_hexadecimal()

    assert (res.value == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, dataType, error", [
    ("a", tt._IDENTIFIER, None),
    ("aCamelCaseSymbol", tt._IDENTIFIER, None),
    ("_private", tt._IDENTIFIER, None),
    ("snake_case_symbol", tt._IDENTIFIER, None),
    ("0invalid_with_digit", tt._INVALID, Error("ValueError: Unexpected illegal character {}".format("0"))),
    ("valid_with_digit_1", tt._IDENTIFIER, None),
    ("0", tt._INVALID, Error("ValueError: Unexpected illegal character {}".format("0"))),
    ("åäö", tt._INVALID, Error("ValueError: Unexpected illegal character {}".format("å"))),
    ("Џџ_Њњ_cryllic", tt._INVALID, Error("ValueError: Unexpected illegal character {}".format("Џ"))),
    ("\U0001f600\U0001F606\U0001F923", tt._INVALID ,Error("ValueError: Unexpected illegal character {}".format("\U0001f600"))),
    ("0b", tt._INVALID ,Error("ValueError: Unexpected illegal character {}".format("0"))),
    ("", tt._INVALID ,Error("ValueError: Unexpected illegal character {}".format(None)))
])
def test_lexer_make_symbol(source_code, dataType, error):
    l = lexer.Lexer(source_code)
    res = l.make_symbol()

    assert ((res.datatype if res else None) == dataType) and (l.error == error)


@pytest.mark.parametrize("source_code, indent_level, error", [
    ("\n\n", 0, None),
    ("    "+ 'def', 1, None),
    ("    "*5 + 'hej', 5, None),
    (" katten", None, Error("IndentationError: Invalid indentation")),
    ("      Hejsan", None, Error("IndentationError: Invalid indentation")),
    ("5", 0, None),
])
def test_lexer_check_indent(source_code, indent_level, error):
    l = lexer.Lexer(source_code)
    res, blank = l.check_indent()

    assert (res == indent_level) and (l.error == error)

@pytest.mark.parametrize("cur_indent_lvl, new_indent_lvl, indent_type, error", [
    (0, 0, None, None),
    (0, 1, tt._INDENT, None),
    (1, 0, tt._DEDENT, None),
    (1, 1, None, None),
    (0, 10, tt._INDENT, None),
    (10, 0, tt._DEDENT, None),
    (0, -1, None, Error("ValueError: Positive integer expected")),
    (5, "Hej", None, Error("ValueError: Positive integer expected")),
])
def test_lexer_change_indent(cur_indent_lvl, new_indent_lvl, indent_type, error):
    l = lexer.Lexer()
    l.position.indent = cur_indent_lvl

    l.change_indent(new_indent_lvl)

    assert (l.tokens[-1].datatype if l.tokens else None == indent_type) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ("0", 0, None),
    ("0b0", 0, None),
    ("0o0", 0, None),
    ("0x0", 0, None),
    ("1", 1, None),
    ("0b1", 1, None),
    ("0o1", 1, None),
    ("0x1", 1, None),
    ("100", 100, None),
    ("0b100", 4, None),
    ("0o100", 64, None),
    ("0x100", 256, None),
    ("a", None, Error("ValueError: Expected a digit or dot '.'")),
])
def test_lexer_make_symbol(source_code, expected_result, error):
    l = lexer.Lexer(source_code)
    res = l.make_number()

    assert ((res.value if res.datatype != tt._INVALID else None) == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ("\"Hejsan\"", "Hejsan", None),
    ("\'Katten\'", "Katten", None),
    ("\"åäö\"", "åäö", None),
    ("\"Hola\\n\"", "Hola\n", None),
    ("\"Hola\n\"", None, Error("StringError: Incorrect line break in string")),
])
def test_lexer_make_string(source_code, expected_result, error):
    l = lexer.Lexer(source_code)
    res = l.make_string()

    assert ((res.value if res.datatype != tt._INVALID else None) == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_result, error", [
    ('=',tt._ASSIGN, None,),
    ('==',tt._BITWISE_EQ, None,),
    ('+',tt._PLUS, None,),
    ('++',tt._INCR, None,),
    ('+=',tt._PLUS_ASSIGN, None,),
    ('-',tt._MINUS, None,),
    ('--',tt._DECR, None,),
    ('-=',tt._MINUS_ASSIGN, None,),
    ('*',tt._MULT, None,),
    ('*=',tt._MULT_ASSIGN, None,),
    ('**',tt._EXP, None,),
    ('**=',tt._POWER_ASSIGN, None,),
    ('/',tt._DIV, None,),
    ('/=',tt._DIV_ASSIGN, None,),
    ('//',tt._FLOOR, None,),
    ('//=',tt._FLOOR_ASSIGN, None,),
    ('%',tt._MOD, None,),
    ('%=',tt._MOD_ASSIGN, None,),
    ('&=',tt._AND_ASSIGN, None,),
    ('&',tt._BITWISE_AND, None,),
    ('|=',tt._OR_ASSIGN, None,),
    ('|',tt._BITWISE_OR, None,),
    ('^',tt._BITWISE_XOR, None,),
    ('^=',tt._XOR_ASSIGN, None,),
    ('<',tt._BITWISE_LT, None,),
    ('<=',tt._BITWISE_LTE, None,),
    ('<<',tt._BITWISE_LSHIFT, None,),
    ('<<=',tt._LSHIFT_ASSIGN, None,),
    ('>',tt._BITWISE_GT, None,),
    ('>=',tt._BITWISE_GTE, None,),
    ('>>',tt._BITWISE_RSHIFT, None,),
    ('>>=',tt._RSHIFT_ASSIGN, None,),
    ('(',tt._LPARAN, None,),
    (')',tt._RPARAN, None,),
    ('[' ,tt._LSQBRACK, None,),
    (']' ,tt._RSQBRACK, None,),
    ('{' ,tt._LCURLBRACK, None,),
    ('}' ,tt._RCURLBRACK, None,),
    ('.',tt._DOT, None,),
    (',',tt._COMMA, None,),
    (':',tt._COLON, None,),
    ('', None, Error("LexicalError: No characters in buffer"),),
    (' ', None, Error("ValueError: Token not a operator"),),
])
def test_lexer_make_operator(source_code, expected_result, error):
    l = lexer.Lexer(source_code)
    res = l.make_operator()

    assert ((res.datatype if res.datatype != tt._INVALID else None) == expected_result) and (l.error == error)

@pytest.mark.parametrize("source_code, expected_value, expected_type, error", [
    ("\"\"\"Detta är en flerradskommentar\nDen skrivs över flera rader\"\"\"", "\"\"\"Detta är en flerradskommentar\nDen skrivs över flera rader\"\"\"", "multi_line_comment", None),
    ("#Katten\n", "#Katten", "comment", None),
    ("#\"åäö\"\n", "#\"åäö\"", "comment", None),
    ("\"Hola\\n\"", None, "invalid", Error("LexicalError: Invalid Comment")),
    ("#Detta är en vanlig kommentar\nden kan inte hantera radbryt", "#Detta är en vanlig kommentar", "comment", None),
])
def test_lexer_make_comment(source_code, expected_value, expected_type,  error):
    l = lexer.Lexer(source_code)
    res = l.make_comment()

    assert ((res.value if res.datatype != tt._INVALID else None) == expected_value) and (l.error == error)
