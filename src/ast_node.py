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
        val = str(self.value) if self.value else ""
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
    def __init__(self, file_name, nodes=None):
        if nodes is None:
            nodes = []
        super().__init__(file_name, nodes) 
    def build_ast(self):
        raise NotImplementedError

class Class_decl_node(Ast_node):
    """
    Used to define a class
    """
    def __init__(self, value, children):
        super().__init__(value, children) 
    def build_ast(self):
        raise NotImplementedError

class Func_decl_node(Ast_node):
    """
    Used to define a method
    """
    def __init__(self, value, children):
        super().__init__(value, children)

class Func_call_node(Ast_node):
    """
    Used to define a method
    """
    def __init__(self, identifier, arguments):
        super().__init__(identifier.value, [identifier, arguments])


class Subscriber_call_node(Ast_node):
    """
    Used to define a method
    """
    def __init__(self, identifier, arguments):
        super().__init__(identifier.value, [identifier, arguments])

class Bin_op_node(Ast_node):
    """
    Defines a binary operation. 
    value = the operator
    children[0] = left part of the operation
    children[1] = right part of the operation 
    """
    def __init__(self, left, operator, right):
        super().__init__(operator, list((left, right)))

class Pre_unary_op_node(Ast_node):
    """
    Defines a unary operation. 
    value = the operator 
    children[0] = value to operate on
    children[1] = prefix (bool)
    """
    def __init__(self, operator, value):
        super().__init__(operator, list((value,)))

class Post_unary_op_node(Ast_node):
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

class Bool_node(Ast_node):
    def __init__(self, value):
        super().__init__(value, None)

class None_node(Ast_node):
    def __init__(self, value):
        super().__init__(value, None)
    
class If_else_expression_node(Ast_node):
    def __init__(self, true_val, condition, else_val):
        super().__init__(None, [true_val, condition, else_val])

class Comparison_node(Ast_node):
    def __init__(self, op, lhs, rhs):
        super().__init__(op, [lhs, rhs])
    
class Identifier_call_node(Ast_node):
    def __init__(self, token):
        super().__init__(token, [])

class List_node(Ast_node):
    def __init__(self, objects):
        super().__init__("List", [objects])

class Tuple_node(Ast_node):
    def __init__(self, objects):
        super().__init__("Tuple", [objects])

class Expression_list_node(Ast_node):
    def __init__(self, expressions):
        super().__init__("Expression list", expressions)

class List_comprehension_node(Ast_node):
    def __init__(self, expressions, expr_list, iterable):
        super().__init__("List Comprehension", [expressions[0], expr_list, iterable])

class Dict_comprehension_node(Ast_node):
    def __init__(self, kv_pair, expr_list, iterable):
        super().__init__("Dict Comprehension", [kv_pair[0], expr_list, iterable])

class Set_comprehension_node(Ast_node):
    def __init__(self, expression, expr_list, iterable):
        super().__init__("Set Comprehension", [expression[0], expr_list, iterable])

class Slices_node(Ast_node):
    def __init__(self, expressions):
        super().__init__("Slices List", expressions)

class Key_value_node(Ast_node):
    def __init__(self, key, value):
        super().__init__("Key/Value List", [key, value])

class Dict_node(Ast_node):
    def __init__(self, objects):
        super().__init__("Dict", [objects])

class Set_node(Ast_node):
    def __init__(self, objects):
        super().__init__("Set", [objects])

class Assign_node(Ast_node):
    def __init__(self, assign_op, id, expression):
        super().__init__(assign_op, [id, expression])


class Block_node(Ast_node):
    def __init__(self, stmts):
        super().__init__("Block", stmts)
