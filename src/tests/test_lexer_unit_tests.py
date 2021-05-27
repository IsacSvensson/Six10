import pytest
from lex_token import Token
import lexer
import terminal_tokens as tt
from error import Error

@pytest.mark.parametrize("test_input,expected_result", [
    ("a", True),
    ("A", True),
    ("AA", False),
    ("", False),
    ("1", False),
    ("Ö", False),
    (True, False),
    (["a"], False)
])
def test_isLetter(test_input, expected_result):
    assert lexer.isLetter(test_input) == expected_result

@pytest.mark.parametrize("test_input,expected_result", [
    ("a", None),
    ("A", None),
    ("", None),
    ("1", None),
    ("Ö", None),
    (True, None),
    (["a"], None),
    ("True", tt._TRUE),
    ("False", tt._FALSE),
    ("true", None),
    ("false", None),
    
])
def test_isBool(test_input, expected_result):
    assert lexer.isBool(test_input) == expected_result

@pytest.mark.parametrize("test_input,expected_result", [
    ("pass", tt._PASS),
    ("break", tt._BREAK),
    ("return", tt._RETURN),
    ("continue", tt._CONTINUE),
    ("import", tt._IMPORT),
    ("as", tt._AS),
    ("raise", tt._RAISE),
    ("delete", tt._DELETE),
    ("if", tt._IF),
    ("else", tt._ELSE),
    ("elif", tt._ELIF),
    ("and", tt._AND),
    ("or", tt._OR),
    ("not", tt._NOT),
    ("in", tt._IN),
    ("is", tt._BITWISE_IS),
    ("def", tt._DEF),
    ("class", tt._CLASS),
    ("for", tt._FOR),
    ("while", tt._WHILE),
    ("switch", tt._SWITCH),
    ("case", tt._CASE),
    ("True", tt._TRUE),
    ("False", tt._FALSE),
    ("true", None),
    ("false", None),
    ("12", None),
    ("Test123", None)
    ])
def test_isKeyword(test_input, expected_result):
    assert lexer.isKeyword(test_input) == expected_result

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
