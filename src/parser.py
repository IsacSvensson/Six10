from error import Error
from lex_token import Token
import terminal_tokens as tt
from ast_node import Bin_op_node, Unary_op_node, Number_node

class Parse_result:
    def __init__(self):
        self.error = None
        self.node = None

    def register(self, res):
        if isinstance(res, Parse_result):
            if res.error: self.error = res.error
            return res.node
        
        return res

    def success(self, node):
        self.node = node
        return self

    def failure(self, error):
        self.error = error
        return self


class Parser:
    def __init__(self, tokens) -> None:
        self.tokens = tokens
        self.token_index = -1
        self.advance()
    
    def advance(self):
        self.token_index += 1
        self.update_current_token()
    
    def update_current_token(self):
        if self.token_index >= 0 and self.token_index < len(self.tokens):
            self.current_token = self.tokens[self.token_index]

    def parse(self):
        res = self.expr()
    
        return res

    def expr(self):
        return self.bin_op(self.term, (tt._PLUS, tt._MINUS))

    def term(self):
        return self.bin_op(self.factor, (tt._MULT, tt._DIV))

    def factor(self):
        res = Parse_result()
        tok = self.current_token
        node = None 

        if tok.datatype in (tt._PLUS, tt._MINUS):
            res.register(self.advance())
            factor = res.register(self.factor())
            if res.error: return res

            return res.success(Unary_op_node(tok, factor))

        elif tok.datatype in (tt._INT, tt._FLOAT):
            factor = Number_node(tok)
            node = factor
            res.register(self.advance())

            if self.current_token.datatype in (tt._INCR, tt._DECR):
                res.register(self.advance())
                node = Unary_op_node(self.current_token, factor)
            return res.success(node)

        elif tok.datatype == tt._LPARAN:
            res.register(self.advance())
            expr = res.register(self.expr())
            if res.error: return res
            if self.current_token.datatype == tt._RPARAN:
                res.register(self.advance())
                return res.success(expr)
            else:
                return res.failure(Error("Expected ')'"))
        
        return res.failure(Error("Expected int or float"))
    
    def bin_op(self, func, ops):
        res = Parse_result()
        left = res.register(func())
        if res.error: return res

        while self.current_token.datatype in ops:
            op_tok = self.current_token
            res.register(self.advance())
            right = res.register(func())
            if res.error: return res
            left = Bin_op_node(left, op_tok, right)

        return res.success(left)