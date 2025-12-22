class Parse_result:
    """
    Class for holding parse results
    """
    def __init__(self):
        """
        Inits with none values
        """
        self.error = None
        self.node = None

    def register(self, res):
        """
        Registers if anything went wrong
        """
        if isinstance(res, Parse_result):
            if res.error: self.error = res.error
            return res.node
        
        return res

    def success(self, node):
        """
        Registers successfully created nodes
        """
        self.node = node
        return self

    def failure(self, error):
        """
        Registers failures
        """
        self.error = error
        return self
