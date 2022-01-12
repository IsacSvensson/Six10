class Error:
    def __init__(self, msg, pos_start=None, pos_end=None):
        self.pos_start = pos_start
        self.pos_end = pos_end
        self.message = msg

    def generate_traceback(self):
        start = self.pos_start
        end = self.pos_end
        if end is None:
            end = start
        same_line = True
        if start.row != end.row:
            same_line = False

        code = start.get_line_str().lstrip()
        output = ["Error: " + self.message]
        output.append(f"Traceback:\n    File \"<{start.filename}>\", line {start.row+1}, column {(-start.indent*4)+start.column+1}")
        output.append("-"*100)
        output.append(f"{code}")
        if same_line:
            pre_error = (start.column-(start.indent*4))*'-'
            e_placement = (1+end.column-(end.indent*4)-(start.column-(start.indent*4)))-1
            error_space = '^'*(e_placement if e_placement > 1 else 1)
            post_error = (100-len(pre_error+error_space))*'-'
            output.append(f"{pre_error+error_space+post_error}")
        else:
            pre_error = (start.column-(start.indent*4))*'-'
            error_space = '^'*(len(code)-len(pre_error))
            post_error = (100-len(pre_error+error_space))*'-'
            output.append(f"{pre_error+error_space+post_error}")

        return "\n".join(output)

    def __eq__(self, o: object) -> bool:
        if isinstance(o, Error):
            return self.message == o.message
        return False

    def __str__(self) -> str:
        return self.generate_traceback()