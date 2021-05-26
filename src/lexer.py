import terminal_tokens as tt
from position import Position
from lex_token import Token

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
    def __init__(self, source_code, filename):
        """
        Inits class with source code and filename, init a Position object and gets first character.
        """
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
        next_char = self.position.index + 1
        return self.source_code[next_char : next_char + count]

    def allowed_character(self, allowed_characters):
        if self.current_character is None:
            return False
        if allowed_characters is None or allowed_characters == "":
            return False
        elif self.current_character.lower() in allowed_characters:
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
            self.advance()

    def make_number(self):
        """
        Reads characters from source code and returns a number token. 
        The method can parse integers, floats, octadecimal, hexdecimal and binary numbers. 
        """
        next_character = self.look_ahead().lower()

        if self.current_character == '0' and next_character == 'b':
            return self.make_binary()
        elif self.current_character == '0' and next_character == 'o':
            return self.make_octodecimal()
        elif self.current_character == '0' and next_character == 'x':
            return self.make_hexadecimal()
        else:
            return self.make_decimal()
        

    def make_binary(self):
        """
        Reads binary characters until not allowed character appers.
        Returns a binary token
        """
        binary_string = "0b"
        allowed_chars = "01"

        start_position = self.position.copy()
        end_position = None

        self.advance()
        self.advance()
        
        while self.allowed_character(allowed_chars):
            binary_string += self.current_character

            self.advance()

        end_position = self.position.copy()
        
        if len(binary_string) < 3:
            return Token(tt._INVALID, binary_string, start_position, end_position)

        return Token(tt._BIN, int(binary_string, base=2), start_position, end_position)
        
    def make_octodecimal(self):
        """
        Reads octodecimal characters until not allowed character appers.
        Returns a octodecimal token
        """
        oct_string = "0o"
        allowed_chars = "01234567"

        start_position = self.position.copy()
        end_position = None

        self.advance()
        self.advance()
        
        while self.allowed_character(allowed_chars):
            oct_string += self.current_character

            self.advance()
        
        end_position = self.position.copy()
        
        if len(oct_string) < 3:
            return Token(tt._INVALID, oct_string, start_position, end_position)

        return Token(tt._OCT, int(oct_string, base=8), start_position, end_position)

    def make_hexadecimal(self):
        """
        Reads hexadecimal characters until not allowed character appers.
        Returns a hexdecimal token
        """
        hex_string = "0x"
        allowed_chars = "0123456789abcdef"

        self.advance()
        self.advance()

        start_position = self.position.copy()
        end_position = None
        
        while self.allowed_character(allowed_chars):
            hex_string += self.current_character

            self.advance()
        
        end_position = self.position.copy()
        
        if len(hex_string) < 3:
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
            
    def check_indent(self):
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
        while self.position.indent < indent:
            self.position.indent += 1
            self.tokens.append(Token(tt._INDENT, "    ", self.position, self.position))

        while self.position.indent > indent:
            self.position.indent -= 1
            self.tokens.append(Token(tt._DEDENT, "    ", self.position, self.position))
        