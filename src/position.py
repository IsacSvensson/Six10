
class Position:
    """
    Keeps track on position in code. 
    Saves index in code, row, column and indentation level.

    Attributes:
        index: int - The index of the source code string where the position-object starts.
        row: int - What row the position object starts at.
        column: int - What column the position object starts at.
        indent: int - The indent level of the object specified by the position-object.
        filename: str - Name of the file.
        code_snippet: str - The code snippet which the position-object concerns 
    """
    def __init__(self, index, row, column, indent, filename = None, source_code=None):
        """
        Inits class with given parameters
        """
        self.index = index
        self.row = row
        self.column = column
        self.indent = indent
        self.filename = filename
        self.source_code = source_code

    def advance(self, current_character=None):
        """
        Advances the position-object to next index in code.

        PARAMS:
            current_character: str - If this is a '\n'-characted the position moves to the start of the next row.
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
        return Position(
            self.index, 
            self.row, 
            self.column, 
            self.indent, 
            self.filename, 
            self.source_code
            )

    def get_line_str(self):
        return self.source_code.split('\n')[self.row]

    def __str__(self):
        return "Row/col: {}/{}, Indent: {}".format(self.row, self.column, self.indent)