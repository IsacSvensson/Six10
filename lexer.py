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
        Inits class with source code and filename, sets all values to 0. 
        """
        self.index = 0
        self.row = 0
        self.column = 0
        self.filename = filename
        self.source_code = source_code
        self.current_indent_lvl = 0
        self.tokens = []
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