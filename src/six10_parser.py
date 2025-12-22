from error import Error
from lex_token import Token
import terminal_tokens as tt
from ast_node import *
from parse_result import Parse_result
from statement_parser import StatementParserMixin
from expression_parser import ExpressionParserMixin
from atom_parser import AtomParserMixin

class Parser(StatementParserMixin, ExpressionParserMixin, AtomParserMixin):
    """
    Parser class, takes a set of tokens and parses to create an ast
    """
    def __init__(self, tokens) -> None:
        """
        Inits with a list of tokens.
        """
        self.tokens = tokens
        self.token_index = -1
        self.current_token = None
        # Sets up current token
        self.advance()
    
    def advance(self):
        """
        Moves selects next token
        """
        self.token_index += 1
        self.update_current_token()
    
    def update_current_token(self):
        """
        Updates current_token to next token
        """
        if self.token_index >= 0 and self.token_index < len(self.tokens):
            self.current_token = self.tokens[self.token_index]
        else:
            self.current_token = None

    def parse(self):
        """
        Entrypoint for tokenization.
        """
        res = self.program()
    
        return res

    def program(self):
        res = self.statements()
        if res.error: return res

        return res.success(Program_node(self.tokens[0].start.filename, [res.node]))

    def bin_op(self, func, ops):
        """
        Handles binary operations
        """
        res = Parse_result()
        lhs = res.register(func())
        if res.error: return res

        while self.current_token.datatype in ops:
            op_tok = self.current_token
            res.register(self.advance())
            rhs = res.register(func())
            if res.error: return res
            lhs = Bin_op_node(lhs, op_tok, rhs)

        return res.success(lhs)
    
    def un_op(self, func, ops):
        """
        Handles pre and post unary operations
        """
        res = Parse_result()
        if self.current_token.datatype in ops:
            # If pre operation
            while self.current_token.datatype in ops:
                op_tok = self.current_token
                res.register(self.advance())
                value = res.register(func())
                if res.error: return res
                node = Pre_unary_op_node(op_tok, value)
        else:
            # If post operation
            node = res.register(func())
            if res.error: return res

            while self.current_token.datatype in ops:
                op_tok = self.current_token
                res.register(self.advance())
                node = res.register(func())
                if res.error: return res
                node = Post_unary_op_node(op_tok, node)

        return res.success(node)