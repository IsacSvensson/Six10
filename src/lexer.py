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
        self.position = Position(-1, 0, -1, 0, filename, source_code=source_code)
        self.current_character = None
        self.tokens = []
        self.advance() # Inits instance, setting current_char to first char in src code and setting that as position

    def get_cur_char(self):
        """
        RETURNS: 
            str: A copy of current_character
        """
        return self.current_character

    def advance(self, n=1):
        """
        Advances current_token to next character in source code. 
        Changes Position-object to new postion in code.
        """
        for i in range(n):
            self.position.advance(self.current_character)

            if self.position.index < len(self.source_code):
                self.current_character = self.source_code[self.position.index] 
            else: 
                self.current_character = None
                return False
        return True

    def look_ahead(self, count = 1):
        """
        Looks ahead on comming characters in the source code.

        PARAMS:
            count: int - Specifies how many characters to fetch
        RETURNS:
            str: the following characters
        """
        if not isinstance(count, int):
            self.error = Error("Error: count is expected to be an int", self.position)
            return None

        if count < 1:
            self.error = Error("Error: count is expected be a positive integer", self.position)
            return None
        
        sc_length = len(self.source_code)
        next_char = self.position.index + 1

        if sc_length < next_char + count:
            return None
        return self.source_code[next_char : next_char + count]

    def allowed_character(self, allowed_characters):
        """
        Checks whether current character is part of the set of characters inputed.

        PARAMS:
            allowed_characters: iterable - a iterable containing characters, (set, list or string)
        RETURNS:
            bool
        """
        if self.get_cur_char() is None:
            self.error = Error("Error: Unexpected end of source code.", self.position)
            return False
        if allowed_characters is None or allowed_characters == "":
            self.error = Error("Error: No characters to allow entered.", self.position)
            return False
        elif self.get_cur_char().lower() in allowed_characters.lower():
            return True
        return False

    def make_tokens(self):
        """
        Preforms the lexical analysis on the source code and breaks it down to terminal tokens.

        RETURNS:
            list, containing token-objects 
        """
        while self.get_cur_char():
            if self.allowed_character("0123456789"):
                self.tokens.append(self.make_number())
                if self.error: return
            elif self.get_cur_char() == "\n":
                start = self.position.copy()
                self.advance()
                self.tokens.append(Token(tt._NEWLINE, '\n',start, self.position.copy()))
                indent, blank = self.check_indent()
                if blank:
                    continue
                if self.error: return
                if indent != self.position.indent:
                    self.change_indent(indent)
            elif self.allowed_character("'\""):
                if self.look_ahead(2) == '""':
                    self.tokens.append(self.make_comment())
                else:
                    self.tokens.append(self.make_string())
                if self.error: return
                continue
            elif self.is_operator():
                self.tokens.append(self.make_operator())
                if self.error: return
                continue
            elif self.allowed_character('#'):
                self.tokens.append(self.make_comment())
                if self.error: return
                continue
            else:
                letterResult, error = isLetter(self.get_cur_char())
                if error: 
                    error.position = self.position
                    self.error = error
                    return 
                if letterResult: 
                    self.tokens.append(self.make_symbol())
                    continue               
                else:
                    if self.allowed_character(" \n"):
                        self.advance()
                        continue
                    start = self.position.copy()
                    char = self.get_cur_char()
                    self.advance()
                    end = self.position.copy()
                    self.tokens.append(Token(tt._INVALID, char, start, end))
                    self.error = Error("ValueError: Unexpected character", self.position)
            if self.error:
                    return
        self.tokens.append(Token(tt._EOF, "", self.position.copy(), self.position.copy()))

    def make_number(self):
        """
        Reads characters from source code and returns a number token. 
        The method can parse integers, floats, octadecimal, hexdecimal and binary numbers. 
        """

        if self.get_cur_char() == '0':
            next_character = self.look_ahead()
            if next_character:
                next_character = next_character.lower() 
                if next_character == 'b':
                    return self.make_binary()
                elif next_character == 'o':
                    return self.make_octodecimal()
                elif next_character == 'x':
                    return self.make_hexadecimal()
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

        binary_string += self.get_cur_char()
        self.advance()
        binary_string += self.get_cur_char()
        self.advance()

        if binary_string.lower() != '0b':
            end_position = self.position.copy()
            self.error = Error("ValueError: Can not convert to a number", start_position)
            return Token(tt._INVALID, binary_string, start_position, end_position)
        
        while self.get_cur_char() and self.allowed_character(allowed_chars):
            binary_string += self.get_cur_char()
            self.advance()

        end_position = self.position.copy()
        
        if len(binary_string) < 3:
            self.error = Error("ValueError: Can not convert to a number", start_position)
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

        oct_string += self.get_cur_char()
        self.advance()
        oct_string += self.get_cur_char()
        self.advance()

        if oct_string.lower() != '0o':
            end_position = self.position.copy()
            self.error = Error("ValueError: Can not convert to a number", start_position)
            return Token(tt._INVALID, oct_string, start_position, end_position)

        while self.get_cur_char() and self.allowed_character(allowed_chars):
            oct_string += self.get_cur_char()

            self.advance()
        
        end_position = self.position.copy()
        
        if len(oct_string) < 3:
            self.error = Error("ValueError: Can not convert to a number", start_position)
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

        hex_string += self.get_cur_char()
        self.advance()
        hex_string += self.get_cur_char()
        self.advance()

        if hex_string.lower() != '0x':
            end_position = self.position.copy()
            self.error = Error("ValueError: Can not convert to a number", start_position)
            return Token(tt._INVALID, hex_string, start_position, end_position)

        while self.get_cur_char() and self.allowed_character(allowed_chars):
            hex_string += self.get_cur_char()

            self.advance()
        
        end_position = self.position.copy()
        
        if len(hex_string) < 3:
            self.error = Error("ValueError: Can not convert to a number", start_position)
            return Token(tt._INVALID, hex_string, start_position, end_position)

        return Token(tt._HEX, int(hex_string, base=16), start_position, end_position)

    def make_decimal(self):
        """
        Reads decimal characters until not allowed character appers.
        Returns a integer or float token
        """

        number_string = ""
        dot_counter = 0 # To make sure that there is only one decimal point
        allowed_chars = "1234567890."


        start_position = self.position.copy()
        end_position = None

        if self.get_cur_char() not in allowed_chars:
            self.error = Error("ValueError: Expected a digit or dot '.'", start_position)
            char = self.get_cur_char()
            self.advance()
            end_position = self.position.copy()
            return Token(tt._INVALID, char, start_position, end_position)

        while self.allowed_character(allowed_chars) and dot_counter < 2:
            number_string += self.get_cur_char()

            self.advance()
            if self.get_cur_char() == ".":
                dot_counter += 1
            elif self.get_cur_char() is None:
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
        allowed_chars = "1234567890_abcdefghijklmnopqrstuvwxyz"
        symbol = ""
        start = self.position.copy()

        # To make sure that the symbol doesn't start with a digit
        if not self.allowed_character(allowed_chars[10:]):
            symbol = self.get_cur_char()
            self.advance()
            end = self.position.copy()
            self.error = Error("ValueError: Unexpected illegal character {}".format(symbol), start)
            return Token(tt._INVALID, symbol, start, end)

        while self.allowed_character(allowed_chars):
            symbol += self.get_cur_char()
            if not self.advance(): # if reaching end of src code 
                break
        
        end = self.position.copy()

        symbol_type, error = isKeyword(symbol)

        if error:
            error.position = start
            self.error = error
            return Token(tt._INVALID, symbol, start, end)

        return Token(symbol_type, symbol, start, end)

    def make_string(self):
        """
        Creates a string.
        Reads characters for src code until terminating character is reached.

        RETURNS:
            Token
        """
        start = self.position.copy()
        qm = self.get_cur_char()
        not_allowed_chars = qm + "\n"

        string = str()
        while self.advance() and not self.allowed_character(not_allowed_chars):
            if self.get_cur_char() == '\\':
                next = self.look_ahead()
                if next == '\n':
                    self.advance()
                    continue
                elif next == "\"\'":
                    string += self.get_cur_char()
                elif next == 'n':
                    self.advance()
                    string += '\n'
                    continue
                elif next == 't':
                    self.advance()
                    string += '\t'
                    continue
                elif next == '\\':
                    self.advance()

            string += self.get_cur_char()
        
        if self.get_cur_char() == qm:
            self.advance()
            end = self.position.copy()
            return Token(tt._STRING, string, start, end)
        elif self.get_cur_char() == "\n":
            self.advance()
            end = self.position.copy()
            self.error = Error("StringError: Incorrect line break in string", start, end)
            return Token(tt._INVALID, string, start, end)
        else:
            self.advance()
            end = self.position.copy()
            self.error = Error("StringError: Non-terminated string", start, end)
            return Token(tt._INVALID, string, start, end)

    def check_indent(self):
        """
        Checks indentation level.

        Returns
            int - Level of indentation
        """
        if self.get_cur_char() == "\n": # If blank row without indent
            return 0, True
        elif self.get_cur_char() != " ": # If row without indent
            return 0, False
        count = 0
        start, end = None, None
        while self.get_cur_char() == " ":
            if count % 4 == 0:
                start = self.position.copy()
            count += 1
            self.advance()
        
        if self.allowed_character('\n'): # if blank row with indent 
            return None, True

        if count % 4 == 0: 
            return int(count/4), False
        else:
            self.error = Error("IndentationError: Invalid indentation", start)
            end = self.position.copy()
            self.tokens.append(Token(tt._INVALID, None, start, end))
            return None, False

    def change_indent(self, indent):
        """
        Generates indent and dedent tokens to change indentation level.
        """
        if not isinstance(indent, int):
            self.error = Error("ValueError: Positive integer expected", self.position)
            return
        if indent < 0:
            self.error = Error("ValueError: Positive integer expected", self.position)
            return

        while self.position.indent < indent: # increases indentation
            self.position.indent += 1
            self.tokens.append(Token(tt._INDENT, "    ", self.position, self.position))

        while self.position.indent > indent: # Decreases indentation
            self.position.indent -= 1
            self.tokens.append(Token(tt._DEDENT, "    ", self.position, self.position))
        
    def is_operator(self):
        first_char_in_op = ['=', '+', '-', '*', '/', '%', '&', '|', '^', 
            '<', '>', '(', ')', '[', ']', '{', '}', '.', ',', ':',]
        if self.get_cur_char() in first_char_in_op:
            return True
        return False
    
    def make_operator(self):
        n = self.look_ahead(2)
        if not n:
            n = self.look_ahead()

        possible_op = self.get_cur_char()
        if possible_op is None:
            self.error = Error("LexicalError: No characters in buffer", self.position)
            return Token(tt._INVALID, None, None, None)
        if n:
            possible_op += n 
        start = self.position.copy()

        values = {
            '=':tt._ASSIGN,
            '==':tt._BITWISE_EQ,
            '+':tt._PLUS,
            '++':tt._INCR,
            '+=':tt._PLUS_ASSIGN,
            '-':tt._MINUS,
            '--':tt._DECR,
            '-=':tt._MINUS_ASSIGN,
            '*':tt._MULT,
            '*=':tt._MULT_ASSIGN,
            '**':tt._EXP,
            '**=':tt._POWER_ASSIGN,
            '/':tt._DIV,
            '/=':tt._DIV_ASSIGN,
            '//':tt._FLOOR,
            '//=':tt._FLOOR_ASSIGN,
            '%':tt._MOD,
            '%=':tt._MOD_ASSIGN,
            '&=':tt._AND_ASSIGN,
            '&':tt._BITWISE_AND,
            '|=':tt._OR_ASSIGN,
            '|':tt._BITWISE_OR,
            '^':tt._BITWISE_XOR,
            '^=':tt._XOR_ASSIGN,
            '<':tt._BITWISE_LT,
            '<=':tt._BITWISE_LTE,
            '<<':tt._BITWISE_LSHIFT,
            '<<=':tt._LSHIFT_ASSIGN,
            '>':tt._BITWISE_GT,
            '>=':tt._BITWISE_GTE,
            '>>':tt._BITWISE_RSHIFT,
            '>>=':tt._RSHIFT_ASSIGN,
            '(':tt._LPARAN,
            ')':tt._RPARAN,
            '[' :tt._LSQBRACK,
            ']' :tt._RSQBRACK,
            '{' :tt._LCURLBRACK,
            '}' :tt._RCURLBRACK,
            '.':tt._DOT,
            ',':tt._COMMA,
            ':':tt._COLON,
        }
        
        if possible_op.__len__() == 3 and values.get(possible_op):
            self.advance(3)
            end = self.position.copy()
            return Token(values.get(possible_op), possible_op, start, end)
        elif possible_op.__len__() >= 2 and values.get(possible_op[:2]):
            self.advance(2)
            end = self.position.copy()
            return Token(values.get(possible_op[:2]), possible_op[:2], start, end)
        elif values.get(self.get_cur_char()):
            char = self.get_cur_char()
            self.advance()
            end = self.position.copy()
            return Token(values.get(char), char, start, end)
        else:
            self.error = Error("ValueError: Token not a operator", start)
            char = self.get_cur_char()
            self.advance()
            end = self.position.copy()
            return Token(tt._INVALID, char, start, end)
    
    def make_comment(self):
        start = self.position.copy()
        single_line = self.allowed_character('#')

        comment = str()

        if single_line:
            while not self.allowed_character('\n'):
                comment += self.get_cur_char()
                if not self.advance():
                    self.error = Error("LexicalError: Line break '\\n' expected to terminate comment", self.position)
                    return Token(tt._INVALID, comment, start, self.position.copy())
        else:
            count = 0

            for i in range(3):
                if self.allowed_character('"'):
                    comment += self.get_cur_char()
                    self.advance()
                else:
                    self.error = Error("LexicalError: Invalid Comment", start)
                    return Token(tt._INVALID, comment, start, self.position.copy())

            while count < 3:
                comment += self.get_cur_char()
                if not self.advance():
                    self.error = Error("LexicalError: ' \"\"\" ' expected to terminate comment", self.position)
                    return Token(tt._INVALID, comment, start, self.position.copy())
                if self.get_cur_char() == '"':
                    count += 1
                else:
                    count = 0
            
            comment += '"'

        return Token(tt._COMMENT if single_line else tt._MULTI_COMMENT, comment, start, self.position.copy())