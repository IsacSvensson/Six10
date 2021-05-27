class Error:
    def __init__(self, msg):
        self.message = msg

    def __eq__(self, o: object) -> bool:
        if isinstance(o, Error):
            return self.message == o.message
        return False