from error import Error
from lex_token import Token
import terminal_tokens as tt
from ast_node import Bin_op_node, If_else_expression_node, Program_node
from ast_node import Pre_unary_op_node, Post_unary_op_node, Number_node
from ast_node import Comparison_node, Func_call_node, Subscriber_call_node
from ast_node import Identifier_call_node, Bool_node, None_node

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
        res = self.statements()
    
        return res

    def statements(self):
        res = Parse_result()
        stmts = []
        while self.current_token.datatype != tt._EOF:
            if self.current_token.datatype in tt._CMP_STMT:
                node = res.register(self.compound_statment())
            else:
                node = res.register(self.small_statment())
            if res.error: return res
            stmts.append(node)
            while self.current_token.datatype == tt._NEWLINE:
                self.advance()
        res.success(Program_node(self.tokens[0].start.filename, stmts))
        return res

    def compound_statment(self):
        raise NotImplementedError

    def small_statment(self):
        res = Parse_result()
        tok = self.current_token

        if tok.datatype == tt._RETURN:
            raise NotImplementedError
        elif tok.datatype == tt._IMPORT:
            raise NotImplementedError
        elif tok.datatype == tt._RAISE:
            raise NotImplementedError
        elif tok.datatype == tt._PASS:
            raise NotImplementedError
        elif tok.datatype == tt._DELETE:
            raise NotImplementedError
        elif tok.datatype == tt._BREAK:
            raise NotImplementedError
        elif tok.datatype == tt._CONTINUE:
            raise NotImplementedError
        elif tok.datatype == tt._COMMENT:
            raise NotImplementedError
        elif tok.datatype == tt._MULTI_COMMENT:
            raise NotImplementedError
        else:
            return self.expr()


    def expr(self):
        return self.bin_op(self.term, (tt._PLUS, tt._MINUS))

    def term(self):
        return self.bin_op(self.factor, (tt._MULT, tt._DIV, tt._MOD, tt._FLOOR))

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
                return res.failure(Error("Expected ')'", tok.start, self.current_token.end))
        
        return res.failure(Error("Expected int or float", self.current_token.start, self.current_token.end))
    
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