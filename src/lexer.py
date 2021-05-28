from error import Error
import terminal_tokens as tt
from position import Position
from lex_token import Token

def isLetter(char):
    """
    Checks if a character is a letter from the english/latin alphabet.

    PARAMS:
        char: a character
    RETURNS:
        bool
    """
    letters = "abcdefghijklmnopqrstuvwxyz"

    if not isinstance(char, str):
        return False, Error("TypeError: 'char' expected to be a string")
    if len(char) != 1:
        return False, Error("TypeError: 'char' expected to have a length of 1")
    if char.lower() in letters:
        return True, None
    return False, None

def isKeyword(symbol):
    """
    Checks whether a symbol is a keyword or not.

    PARAMS:
        symbol: the string to test
    RETURNS:
        string | None
    """
    if not isinstance(symbol, str):
        return tt._INVALID, Error("TypeError: 'symbol' expected to be a string")
    if len(symbol) < 1:
        return tt._INVALID, Error("ValueError: 'symbol' is not allowed to be empty string")
    if symbol == "pass":
        return tt._PASS, None
    elif symbol == "break":
        return tt._BREAK, None
    elif symbol ==  "continue":
        return tt._CONTINUE, None
    elif symbol == "return":
        return tt._RETURN, None
    elif symbol == "import":
        return tt._IMPORT, None
    elif symbol == "as":
        return tt._AS, None
    elif symbol == "raise":
        return tt._RAISE, None
    elif symbol == "delete":
        return tt._DELETE, None
    elif symbol == "if":
        return tt._IF, None
    elif symbol == "else":
        return tt._ELSE, None
    elif symbol == "elif":
        return tt._ELIF, None
    elif symbol == "and":
        return tt._AND, None
    elif symbol == "or":
        return tt._OR, None
    elif symbol == "not":
        return tt._NOT, None
    elif symbol == "in":
        return tt._IN, None
    elif symbol == "is":
        return tt._BITWISE_IS, None
    elif symbol == "def":
        return tt._DEF, None
    elif symbol == "class":
        return tt._CLASS, None
    elif symbol == "for":
        return tt._FOR, None
    elif symbol == "while":
        return tt._WHILE, None
    elif symbol == "switch":
        return tt._SWITCH, None
    elif symbol == "case":
        return tt._CASE, None
    return isBool(symbol)

def isBool(symbol):
    """
    Checks whether a symbol is a boolean or not.

    PARAMS:
        symbol: the string to test
    RETURNS:
        string | None
    """
    if not isinstance(symbol, str):
        return tt._INVALID, Error("TypeError: 'symbol' expected to be a string")
    if len(symbol) < 1:
        return tt._INVALID, Error("ValueError: 'symbol' is not allowed to be empty string")
    if symbol == "True":
        return tt._TRUE, None
    elif symbol == "False":
        return tt._FALSE, None
    return tt._IDENTIFIER, None

class Lexer:
    """
    Lexer class that handles lexical analysis of the source code.

    Attributes:
        index: int, Current index in source code string
        row: int, Current row in source code
        column: int, Current column in source code
        filename: string, Name of the file currently being lexed
        source_code: string, The source code
        current_indent_lvl: int, The number of indentations at current position in code
        tokens: list, A list of all lexed tokens 
    """
    def __init__(self, source_code="", filename="CLI"):
        """
        Inits class with source code and filename, init a Position object and gets first character.
        """

        self.error = None
        if not isinstance(source_code, str):
            self.error = Error("Error: expected 'str' as source_code")
            return
        if not isinstance(filename, str):
            self.error = Error("Error: expected 'str' as filename")
            return
        self.filename = filename
        self.source_code = source_code
        self.position = Position(-1, 0, -1, 0, filename)
        self.current_character = None
        self.tokens = []
        self.advance()

    def advance(self):
        """
        Advances current_token to next character in source code. 
        Changes Position-object to new postion in code.
        """
        self.position.advance(self.current_character)

        if self.position.index < len(self.source_code):
            self.current_character = self.source_code[self.position.index] 
            return True
        else: 
            self.current_character = None
            return False

    def look_ahead(self, count = 1):
        """
        Looks ahead on comming characters in the source code.

        PARAMS:
            count: int - Specifies how many characters to fetch
        RETURNS:
            str: the following characters
        """
        if not isinstance(count, int):
            print(1)
            self.error = Error("Error: count is expected to be an int")
            return None

        if count < 1:
            print(2)
            self.error = Error("Error: count is expected be a positive integer")
            return None
        
        sc_length = len(self.source_code)
        next_char = self.position.index + 1

        if sc_length < next_char + count:
            print(3)
            self.error = Error("Error: not enought characters in source code")
            return None
        return self.source_code[next_char : next_char + count]

    def allowed_character(self, allowed_characters):
        if self.current_character is None:
            self.error = Error("Error: Unexpected end of source code.")
            return False
        if allowed_characters is None or allowed_characters == "":
            self.error = Error("Error: No characters to allow entered.")
            return False
        elif self.current_character.lower() in allowed_characters.lower():
            return True
        return False

    def make_tokens(self):
        """
        Preforms the lexical analysis on the source code and breaks it down to terminal tokens.

        RETURNS:
            list, containing token-objects 
        """
        while self.current_character:
            if self.allowed_character("0123456789"):
                self.tokens.append(self.make_number())
            elif "{}".format(self.current_character) == "\n":
                indent = self.check_indent()
                if indent != self.position.indent:
                    self.change_indent(indent)
            else:
                letterResult, error = isLetter(self.current_character)
                if letterResult: 
                    self.tokens.append(self.make_symbol())
                elif error:
                    self.error = error
                    return error
                self.advance()

    def make_number(self):
        """
        Reads characters from source code and returns a number token. 
        The method can parse integers, floats, octadecimal, hexdecimal and binary numbers. 
        """

        if self.current_character == '0':
            next_character = self.look_ahead().lower()
            if next_character == 'b':
                return self.make_binary()
            elif next_character == 'o':
                return self.make_octodecimal()
            elif next_character == 'x':
                return self.make_hexadecimal()
        else:
            return self.make_decimal()
        

    def make_binary(self):
        """
        Reads binary characters until not allowed character appers.
        Returns a binary token
        """
        binary_string = ""
        allowed_chars = "01"

        start_position = self.position.copy()
        end_position = None

        binary_string += self.current_character
        self.advance()
        binary_string += self.current_character
        self.advance()

        if binary_string.lower() != '0b':
            end_position = self.position.copy()
            self.error = Error("ValueError: Can not convert to a number")
            return Token(tt._INVALID, binary_string, start_position, end_position)
        
        while self.current_character and self.allowed_character(allowed_chars):
            binary_string += self.current_character

            self.advance()

        end_position = self.position.copy()
        
        if len(binary_string) < 3:
            self.error = Error("ValueError: Can not convert to a number")
            return Token(tt._INVALID, binary_string, start_position, end_position)
        return Token(tt._BIN, int(binary_string, base=2), start_position, end_position)
        
    def make_octodecimal(self):
        """
        Reads octodecimal characters until not allowed character appers.
        Returns a octodecimal token
        """
        oct_string = ""
        allowed_chars = "01234567"

        start_position = self.position.copy()
        end_position = None

        oct_string += self.current_character
        self.advance()
        oct_string += self.current_character
        self.advance()

        if oct_string.lower() != '0o':
            end_position = self.position.copy()
            self.error = Error("ValueError: Can not convert to a number")
            return Token(tt._INVALID, oct_string, start_position, end_position)

        while self.current_character and self.allowed_character(allowed_chars):
            oct_string += self.current_character

            self.advance()
        
        end_position = self.position.copy()
        
        if len(oct_string) < 3:
            self.error = Error("ValueError: Can not convert to a number")
            return Token(tt._INVALID, oct_string, start_position, end_position)

        return Token(tt._OCT, int(oct_string, base=8), start_position, end_position)

    def make_hexadecimal(self):
        """
        Reads hexadecimal characters until not allowed character appers.
        Returns a hexdecimal token
        """
        hex_string = ""
        allowed_chars = "0123456789abcdef"

        start_position = self.position.copy()
        end_position = None

        hex_string += self.current_character
        self.advance()
        hex_string += self.current_character
        self.advance()

        if hex_string.lower() != '0x':
            end_position = self.position.copy()
            self.error = Error("ValueError: Can not convert to a number")
            return Token(tt._INVALID, hex_string, start_position, end_position)

        while self.current_character and self.allowed_character(allowed_chars):
            hex_string += self.current_character

            self.advance()
        
        end_position = self.position.copy()
        
        if len(hex_string) < 3:
            self.error = Error("ValueError: Can not convert to a number")
            return Token(tt._INVALID, hex_string, start_position, end_position)

        return Token(tt._HEX, int(hex_string, base=16), start_position, end_position)

    def make_decimal(self):
        """
        Reads decimal characters until not allowed character appers.
        Returns a integer or float token
        """

        number_string = ""
        dot_counter = 0
        allowed_chars = "1234567890."


        start_position = self.position.copy()
        end_position = None

        if self.current_character not in allowed_chars:
            self.error = Error("ValueError: Expected a digit or dot '.'")
            char = self.current_character
            self.advance()
            end_position = self.position.copy()
            return Token(tt._INVALID, char, start_position, end_position)

        while self.allowed_character(allowed_chars) and dot_counter < 2:
            number_string += self.current_character

            self.advance()
            if self.current_character == ".":
                dot_counter += 1
            elif self.current_character is None:
                break
        
        end_position = self.position.copy()

        if dot_counter:
            return Token(tt._FLOAT, float(number_string), start_position, end_position)
        else:
            return Token(tt._INT, int(number_string), start_position, end_position)
    
    def make_symbol(self):
        """
        Creates identifiers and keywords.
        Reads characters from the source code until getting to a non allowed
        character, decides if it's a keyword or identifier and returns a token.

        RETURNS:
            Token
        """
        allowed_chars = "_1234567890abcdefghijklmnopqrstuvwxyz"
        symbol = ""
        start = self.position.copy()

        while self.current_character.lower() in allowed_chars:
            symbol += self.current_character
            if not self.advance():
                break
        
        end = self.position.copy()

        keyword = isKeyword(symbol)

        if keyword:
            return Token(keyword, symbol, start, end)

        return Token(tt._IDENTIFIER, symbol, start, end)


    def check_indent(self):
        """
        Checks indentation level.

        Returns
            int - Level of indentation
        """
        self.advance()
        count = 0
        while self.current_character == " ":
            count += 1
            self.advance()
        
        if count % 4 == 0:
            return count/4
        else:
            return "Invalid indentation"

    def change_indent(self, indent):
        """
        Generates indent and dedent tokens to change indentation level.
        """
        while self.position.indent < indent:
            self.position.indent += 1
            self.tokens.append(Token(tt._INDENT, "    ", self.position, self.position))

        while self.position.indent > indent:
            self.position.indent -= 1
            self.tokens.append(Token(tt._DEDENT, "    ", self.position, self.position))
        