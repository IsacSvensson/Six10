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

    def look_ahead(count = 1):
        """
        Looks ahead on comming characters in the source code.

        PARAMS:
            count: int - Specifies how many characters to fetch
        RETURNS:
            str: the following characters
        """
        next_char = self.position.index + 1
        return self.source_code[next_char : next_char + count]

    def make_tokens():
        """
        Preforms the lexical analysis on the source code and breaks it down to terminal tokens.

        RETURNS:
            list, containing token-objects 
        """
        while self.current_character:
        if self.current_character in range(0,10):
                tokens.append(make_number())
            
            self.advance()

    def make_number():
        next_character = self.look_ahead().lower()
        if self.current_character == '0' and next_character == 'b':
            return make_binary()
        elif self.current_character == '0' and next_character == 'o':
            return make_octodecimal()
        elif self.current_character == '0' and next_character == 'x':
            return make_hexadecimal()
        elif self.current_character == '0' and next_character != '.':
        

    def make_binary():
        pass
    def make_octodecimal():
        pass
    def make_hexadecimal():
        pass
