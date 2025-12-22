import terminal_tokens as tt
from parse_result import Parse_result
from ast_node import *
from error import Error
from lex_token import Token

class AtomParserMixin:
    def expression_list(self, terminator):
        res = Parse_result()
        exprs = []
        list_comprehension = False
        while self.current_token.datatype != terminator:
            res_expr = self.expr()
            if res_expr.error: return res_expr
            if res_expr.node:
                exprs.append(res_expr.node)
            if self.current_token.datatype == tt._COMMA:
                self.advance()
            elif self.current_token.datatype == tt._FOR:
                list_comprehension = True
                break
            elif self.current_token.datatype != terminator:
                return res.failure(Error(f"Expected ',' or {terminator}", self.current_token.start, self.current_token.end))
        if list_comprehension:
            self.advance()
            res_ids = self.identifier_list()
            if res_ids.error: return res_ids
            expr_list = res_ids.node
            res_it = self.expr()
            if res_it.error: return res_it
            iterable = res_it.node
            self.advance()
            return res.success(List_comprehension_node(exprs, expr_list, iterable))
        self.advance()
        return res.success(Expression_list_node(exprs))

    def identifier_list(self):
        res = Parse_result()
        list_comprehension = False
        ids = []
        while self.current_token.datatype != tt._IN:
            res_atom = self.atom()
            if res_atom.error: return res_atom
            if res_atom.node:
                ids.append(res_atom.node)
            if self.current_token.datatype == tt._COMMA:
                self.advance()
            elif self.current_token.datatype == tt._FOR:
                list_comprehension = True
                break
            elif self.current_token.datatype != tt._IN:
                return res.failure(Error(f"Expected ',' or 'in'", self.current_token.start, self.current_token.end))
        if list_comprehension:
            self.advance()
            res_ids = self.identifier_list()
            if res_ids.error: return res_ids
            expr_list = res_ids.node
            res_it = self.expr()
            if res_it.error: return res_it
            iterable = res_it.node
            self.advance()
            return res.success(List_comprehension_node(ids, expr_list, iterable))
        self.advance()
        return res.success(Expression_list_node(ids))

    def slices(self):
        res = Parse_result()
        exprs = []
        slice = 0
        while self.current_token.datatype != tt._RSQBRACK:
            if slice < 3:
                if self.current_token.datatype == tt._COLON:
                    res = res.success(None_node(Token(tt._NONE, "", self.current_token.start, self.current_token.end)))
                else:
                    res = self.expr()
                if res.error: return res
                if res.node:
                    exprs.append(res.node)
                    slice += 1
                if self.current_token.datatype == tt._COLON and slice < 3:
                    self.advance()
                elif self.current_token.datatype != tt._RSQBRACK:
                    return res.failure(Error(f"Expected ']'", self.current_token.start, self.current_token.end))
            elif self.current_token.datatype != tt._RSQBRACK:
                return res.failure(Error(f"Expected ']'", self.current_token.start, self.current_token.end))
        while len(exprs) < 3:
            exprs.append(None_node(Token(tt._NONE, "", self.current_token.start, self.current_token.end)))
        self.advance()
        return res.success(Slices_node(exprs))

    def atom(self):
        res = Parse_result()
        tok = self.current_token
        if self.current_token.datatype == tt._IDENTIFIER:
            self.advance()
            return res.success(Identifier_call_node(tok))
        elif self.current_token.datatype == tt._LPARAN:
            self.advance()
            res_list = self.expression_list(tt._RPARAN)
            if res_list.error: return res_list
            # If only a single expression inside parentheses and no comma, return the expression itself
            if isinstance(res_list.node, Expression_list_node) and len(res_list.node.children) == 1:
                return res.success(res_list.node.children[0])
            return res.success(Tuple_node(res_list.node))
        elif self.current_token.datatype == tt._LSQBRACK:
            self.advance()
            res_list = self.expression_list(tt._RSQBRACK)
            if res_list.error: return res_list
            return res.success(List_node(res_list.node))
        elif self.current_token.datatype == tt._LCURLBRACK:
            self.advance()
            res_ds, is_dict = self.dict_or_set()
            if res_ds.error: return res_ds
            if is_dict:
                return res.success(Dict_node(res_ds.node))
            else:
                return res.success(Set_node(res_ds.node))
        return self.litteral()

    def litteral(self):
        res = Parse_result()
        tok = self.current_token
        dt = tok.datatype
        if dt == tt._TRUE or dt == tt._FALSE:
            self.advance()
            res.success(Bool_node(tok))
        elif dt == tt._NONE:
            self.advance()
            res.success(None_node(tok))
        elif dt == tt._STRING:
            self.advance()
            res.success(String_node(tok))
        elif dt in tt._NUMBER_TOKENS:
            self.advance()
            res.success(Number_node(tok))
        else:
            return res.failure(Error("Expected litteral ('True'/'False', 'None', string or number)", tok.start, tok.end))
        return res

    def dict_or_set(self):
        terminator = tt._RCURLBRACK
        res = Parse_result()
        exprs = []
        comprehension = False
        is_dict = False
        if self.current_token.datatype != terminator:
            res_expr = self.expr()
            if res_expr.error: return res_expr, is_dict
            if res_expr.node:
                exprs.append(res_expr.node)
            if self.current_token.datatype == tt._COMMA:
                self.advance()
            elif self.current_token.datatype == tt._FOR:
                comprehension = True
            elif self.current_token.datatype == tt._COLON:
                is_dict = True
                self.advance()
                key = res_expr.node
                res_val = self.expr()
                if res_val.error: return res_val, is_dict
                value = res_val.node
                exprs.append(Key_value_node(key, value))
                if self.current_token.datatype == tt._FOR:
                    comprehension = True
                elif self.current_token.datatype == tt._COMMA:
                    self.advance()
                elif self.current_token.datatype != terminator:
                    return res.failure(Error("Expected 'for', ',' or '}'.", self.current_token.start, self.current_token.end)), is_dict
            elif self.current_token.datatype != terminator:
                return res.failure(Error("Expected 'for', ',' or '}'.", self.current_token.start, self.current_token.end)), is_dict
        while self.current_token.datatype != terminator and not comprehension:
            res_expr = self.expr()
            if res_expr.error: return res_expr, is_dict
            if res_expr.node:
                if is_dict:
                    if self.current_token.datatype == tt._COLON:
                        self.advance()
                    else:
                        return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end)), is_dict
                    key = res_expr.node
                    res_val = self.expr()
                    if res_val.error: return res_val, is_dict
                    value = res_val.node
                    exprs.append(Key_value_node(key, value))
                else:
                    exprs.append(res_expr.node)
            if self.current_token.datatype == tt._COMMA:
                self.advance()
            elif self.current_token.datatype != terminator:
                return res.failure(Error(f"Expected ',' or {terminator}", self.current_token.start, self.current_token.end)), is_dict
        if comprehension:
            self.advance()
            res_ids = self.identifier_list()
            if res_ids.error: return res_ids, is_dict
            expr_list = res_ids.node
            res_it = self.expr()
            if res_it.error: return res_it, is_dict
            iterable = res_it.node
            self.advance()
            if is_dict:
                return res.success(Dict_comprehension_node(exprs, expr_list, iterable)), is_dict
            else:
                return res.success(Set_comprehension_node(exprs, expr_list, iterable)), is_dict
        self.advance()
        return res.success(Expression_list_node(exprs)), is_dict
