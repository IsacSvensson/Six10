class Token:
    """
    Class that contain data for tokens.
    """
    def __init__(self, datatype, value, start, end):
        """
        Inits the class with datatype, value and start and end positions.
        """
        self.datatype = datatype
        self.value = value
        self.start = start
        self.end = end