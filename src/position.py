
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