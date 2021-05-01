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