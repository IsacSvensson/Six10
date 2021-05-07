import terminal_tokens as tt
from position import Position

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
        self.position = Position(-1, 0, -1, filename)
        self.current_character = None
        self.tokens = []
        self.advance()

    def advance(self):
        """
        Advances current_token to next character in source code. 
        Changes Position-object to new postion in code.
        """
        self.position.advance()

        if self.position.index < len(self.source_code):
            self.current_character = self.source_code[self.position.index] 
        else: 
            self.current_character = None

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

        """
        Preforms the lexical analysis on the source code and breaks it down to terminal tokens.

        RETURNS:
            list, containing token-objects 
        """
        while self.current_character:
                self.tokens.append(self.make_number())
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
        Reads 
        """
        binary_string = "0b"
        allowed_chars = "01"

        self.advance()
        self.advance()
        
        while self.allowed_character(allowed_chars):
            binary_string += self.current_character

            self.advance()
        
        if len(binary_string) < 3:
            return "invalid token"

        return int(binary_string, base=2)

    def make_octodecimal(self):
        """
        Reads 
        """
        binary_string = "0o"
        allowed_chars = "01234567"

        self.advance()
        self.advance()
        
        while self.allowed_character(allowed_chars):
            binary_string += self.current_character

            self.advance()
        
        if len(binary_string) < 3:
            return "invalid token"

        return int(binary_string, base=8)

    def make_hexadecimal(self):
        """
        Reads 
        """
        binary_string = "0x"
        allowed_chars = "0123456789abcdef"

        self.advance()
        self.advance()
        
        while self.allowed_character(allowed_chars):
            binary_string += self.current_character

            self.advance()
        
        if len(binary_string) < 3:
            return "invalid token"

        return int(binary_string, base=16)

    def make_decimal(self):
        number_string = ""
        dot_counter = 0
        allowed_chars = "1234567890."

        while self.allowed_character(allowed_chars) and dot_counter < 2:
            number_string += self.current_character

            self.advance()
            if self.current_character == ".":
                dot_counter += 1
            elif self.current_character is None:
                break

        if dot_counter:
            return float(number_string)
        else:
            return int(number_string)
            

