import terminal_tokens as tt
from parse_result import Parse_result
from ast_node import *
from error import Error
from lex_token import Token

class ExpressionParserMixin:
    def expr(self):
        res = self.disjunction()
        if res.error: return res
        if self.current_token.datatype == tt._IF:
            true_val = res.node
            self.advance()
            res = self.expr()
            if res.error: return res
            condition = res.node
            if self.current_token.datatype != tt._ELSE:
                return res.failure(Error("Expected 'else'", self.current_token.start, self.current_token.end))
            self.advance()
            res = self.expr()
            if res.error: return res
            else_val = res.node
            node = If_else_expression_node(true_val, condition, else_val)
            return res.success(node)
        return res

    def disjunction(self):
        return self.bin_op(self.conjunction, (tt._OR,))

    def conjunction(self):
        return self.bin_op(self.invertion, (tt._AND,))

    def invertion(self):
        res = Parse_result()
        if self.current_token.datatype == tt._NOT:
            tok = self.current_token
            res.register(self.advance())
            invertion = res.register(self.invertion())
            if res.error: return res
            return res.success(Pre_unary_op_node(tok, invertion))
        return self.comparison()

    def comparison(self):
        res = self.bitwise_or()
        if self.current_token.datatype in tt._COMPARISON:
            tok = self.current_token
            lhs = res.node
            if self.current_token.datatype == tt._NOT:
                self.advance()
                if self.current_token.datatype == tt._IN:
                    self.advance()
                    res = self.bin_op(self.bitwise_or, (tt._NOT_IN))
                    if res.error: return res
                else:
                    return res.failure(Error("Expected 'in'", self.current_token.start, self.current_token.end))
                rhs = res.node
            elif self.current_token.datatype == tt._BITWISE_IS:
                self.advance()
                if self.current_token.datatype == tt._NOT:
                    self.advance()
                    res = self.bin_op(self.bitwise_or, (tt._BITWISE_IS_NOT))
                    if res.error: return res
                else:
                    res = self.bin_op(self.bitwise_or, (tt._BITWISE_IS_NOT))
                    if res.error: return res
                rhs = res.node
            else:
                tok = self.current_token
                self.advance()
                res = self.bin_op(self.bitwise_or, tt._COMPARISON)
                if res.error: return res
                rhs = res.node
            return res.success(Comparison_node(tok, lhs, rhs))
        return res

    def bitwise_or(self):
        return self.bin_op(self.term, (tt._PLUS, tt._MINUS))

    def term(self):
        return self.bin_op(self.factor, (tt._MULT, tt._DIV, tt._MOD, tt._FLOOR))

    def factor(self):
        res = Parse_result()
        tok = self.current_token
        if tok.datatype in (tt._PLUS, tt._MINUS):
            res.register(self.advance())
            factor = res.register(self.factor())
            if res.error: return res
            return res.success(Pre_unary_op_node(tok, factor))
        else:
            return self.un_op(self.unary_variation, (tt._INCR, tt._DECR))

    def unary_variation(self):
        return self.bin_op(self.power, (tt._EXP,))
            
    def power(self):
        return self.bin_op(self.primary, (tt._DOT,))

    def primary(self):
        res = self.atom()
        if res.error: return res
        id = res.node
        while self.current_token.datatype in tt._PARANS or self.current_token.datatype == tt._DOT:
            if self.current_token.datatype == tt._LPARAN:
                self.advance()
                res = self.expression_list(tt._RPARAN)
                if res.error: return res
                if res.node:
                    args = res.node
                else:
                    return res.failure(Error("No ast-node was created."))
                res.success(Func_call_node(id, args))
            elif self.current_token.datatype == tt._LSQBRACK:
                self.advance()
                res = self.slices()
                if res.error: return res
                if res.node:
                    args = res.node
                else:
                    return res.failure(Error("No ast-node was created."))
                res.success(Subscriber_call_node(id, args))
            elif self.current_token.datatype == tt._DOT:
                # member access
                self.advance()
                if self.current_token.datatype != tt._IDENTIFIER:
                    return res.failure(Error("Expected member identifier", self.current_token.start, self.current_token.end))
                member_tok = self.current_token
                self.advance()
                res.success(Member_access_node(id, Identifier_call_node(member_tok)))
            id = res.node
        return res
