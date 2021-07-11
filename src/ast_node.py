class ast_node:
    def __init__(self, value, children, errors):
        self.value = value
        self.children = children
        self.errors = errors

class program_node(ast_node):
    def __init__(self, file_name):
        super().__init__(file_name, list(), list()) 
    def build_ast(self):
        pass

class class_decl_node(ast_node):
    def __init__(self, value, children, errors):
        super().__init__(value, children, errors=errors) 
    def build_ast(self):
        pass