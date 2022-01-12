class Error:
    def __init__(self, msg, pos_start=None, pos_end=None):
        self.pos_start = pos_start
        self.pos_end = pos_end
        self.message = msg

    def __eq__(self, o: object) -> bool:
        if isinstance(o, Error):
            return self.message == o.message
        return False