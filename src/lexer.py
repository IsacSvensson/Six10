import terminal_tokens as tt

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
        self.position = Position(-1, 0, -1)
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
            None

    def make_tokens():
        """
        Preforms the lexical analysis on the source code and breaks it down to terminal tokens.

        RETURNS:
            list, containing token-objects 
        """
        pass

class Position:
    """
    Keeps track on position in code. 
    Saves index in code, row, column and indentation level.
    """
    def __init__(self, index, row, column, indent):
        """
        Inits class with given parameters
        """
        self.index = index
        self.row = row
        self.column = column
        self.indent = indent

    def advance(self, current_character=None):
        """
        Advances the position-object to next index in code.
        """
        self.index += 1
        self.column += 1

        if current_character == '\n':
            self.row += 1
            self.column = 0

        return self

    def copy(self):
        """
        Returns a copy of the object.
        """
        return Position(self.index, self.row, self.column, self.indent)