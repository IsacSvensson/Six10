class Ast_node:
    """
    Base class for AST
    """
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.errors = list()

    def get_type(self):
        """
        returns the name of the objects class
        """
        return type(self).__name__

    def get_value_str(self):
        """
        returns the value of the node as a string
        """
        val = str(self.value.value) if self.value.value else ""
        return val
    
    def print_tree(self, deepth = 0):
        """
        Prints the ast in terminal
        """
        print(("  "*deepth) + self.get_type() + ":", self.get_value_str())
    
        if self.children:
            for child in self.children:
                child.print_tree(deepth+1)
            

class Program_node(Ast_node):
    """
    Top node of the AST.
    """
    def __init__(self, file_name):
        super().__init__(file_name, list()) 
    def build_ast(self):
        pass

class Class_decl_node(Ast_node):
    """
    Used to define a class
    """
    def __init__(self, value, children):
        super().__init__(value, children) 
    def build_ast(self):
        pass

class Func_decl_node(Ast_node):
    """
    Used to define a method
    """
    def __init__(self, value, children):
        super().__init__(value, children)

class Bin_op_node(Ast_node):
    """
    Defines a binary operation. 
    value = the operator
    children[0] = left part of the operation
    children[1] = right part of the operation 
    """
    def __init__(self, left, operator, right):
        super().__init__(operator, list((left, right)))

class Unary_op_node(Ast_node):
    """
    Defines a unary operation. 
    value = the operator 
    children[0] = value to operate on
    children[1] = prefix (bool)
    """
    def __init__(self, operator, value):
        super().__init__(operator, list((value,)))

class Number_node(Ast_node):
    def __init__(self, value):
        super().__init__(value, None)
    