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
    ("", 1, None, Error("Error: not enought characters in source code")),
    ("", 5, None, Error("Error: not enought characters in source code")),
    ("5+10==15", 0, None, Error("Error: count is expected be a positive integer")),
    ("5+10==15", 1, "+", None),
    ("5+10==15", 7, "+10==15", None),
    ("5+10==15", 8, None, Error("Error: not enought characters in source code"))
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