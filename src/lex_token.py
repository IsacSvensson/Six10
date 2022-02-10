from dataclasses import dataclass
from position import Position


@dataclass
class Token:
    """
    Class that contain data for tokens.
    """
    datatype : str
    value : str
    start : Position
    end : Position

    def __str__(self):
        return f"[{self.datatype}]: {self.value}"