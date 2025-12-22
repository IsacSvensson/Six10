from error import Error
import terminal_tokens as tt
from parse_result import Parse_result
from ast_node import *

class StatementParserMixin:
    def statements(self):
        res = Parse_result()
        stmts = []
        while self.current_token.datatype not in [tt._EOF, tt._DEDENT]:
            if self.current_token.datatype in tt._CMP_STMT:
                node = res.register(self.compound_statment())
            else:
                node = res.register(self.small_statment())
            if res.error: return res
            if node:
                stmts.append(node)
            while self.current_token.datatype == tt._NEWLINE:
                self.advance()
        res.success(Block_node(stmts))
        return res

    def compound_statment(self):
        res = Parse_result()
        tok = self.current_token
        if tok.datatype == tt._DEF:
            self.advance()
            if self.current_token.datatype != tt._IDENTIFIER:
                return res.failure(Error("Expected Identifier", self.current_token.start, self.current_token.end))
            res_atom = self.atom()
            if res_atom.error: return res_atom
            id = res_atom.node
            if self.current_token.datatype != tt._LPARAN:
                return res.failure(Error("Expected '('", self.current_token.start, self.current_token.end))
            self.advance()
            if self.current_token.datatype == tt._RPARAN:
                parameters = Expression_list_node([])
                self.advance()
            else:
                res_params = self.expression_list(tt._RPARAN)
                if res_params.error: return res_params
                parameters = res_params.node
            if self.current_token.datatype != tt._COLON:
                return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
            self.advance()
            res_block = self.scope_block()
            if res_block.error: return res_block
            block = res_block.node
            return Func_decl_node(id, parameters, block)
        elif tok.datatype == tt._IF:
            return self.if_stmt()
        elif tok.datatype == tt._FOR:
            return self.for_stmt()
        elif tok.datatype == tt._WHILE:
            return self.while_stmt()
        elif tok.datatype == tt._CLASS:
            return self.class_definition()
        elif tok.datatype == tt._SWITCH:
            return self.switch_stmt()
        else:
            return res

    def scope_block(self):
        res = Parse_result()
        if self.current_token.datatype != tt._NEWLINE:
            stmts = []
            # Parse first statement on the same line
            if self.current_token.datatype in tt._CMP_STMT:
                node = res.register(self.compound_statment())
            else:
                node = res.register(self.small_statment())
            if res.error: return res
            if node:
                stmts.append(node)
            # Consume additional statements separated by semicolons (';'), which are tokenized as NEWLINE with value ';'
            while self.current_token.datatype == tt._NEWLINE and getattr(self.current_token, 'value', '\n') == ';':
                self.advance()
                if self.current_token.datatype in tt._CMP_STMT:
                    node = res.register(self.compound_statment())
                else:
                    node = res.register(self.small_statment())
                if res.error: return res
                if node:
                    stmts.append(node)
            # Do not consume real line breaks here; caller handles '\n'
            return res.success(Block_node(stmts))
        self.advance()
        if self.current_token.datatype != tt._INDENT:
            return res.failure(Error("Expected INDENT", self.current_token.start, self.current_token.end))
        self.advance()
        res_stmts = self.statements()
        if res_stmts.error: return res_stmts
        if self.current_token.datatype != tt._DEDENT:
            return res.failure(Error("Expected DEDENT", self.current_token.start, self.current_token.end))
        self.advance()
        return res.success(res_stmts.node)

    def small_statment(self):
        res = Parse_result()
        tok = self.current_token
        if tok.datatype == tt._RETURN:
            self.advance()
            tok2 = self.current_token
            if tok2.datatype in [tt._NEWLINE, tt._EOF]:
                return res.success(Return_node(None_node(tok2)))
            else:
                res_expr = self.expr()
                if res_expr.error: return res_expr
                return res.success(Return_node(res_expr.node))
        elif tok.datatype == tt._IMPORT:
            self.advance()
            if self.current_token.datatype in [tt._NEWLINE, tt._EOF]:
                return res.failure(Error("Expected module", self.current_token.start, self.current_token.end))
            res_expr = self.expr()
            if res_expr.error: return res_expr
            return res.success(Import_node(tok, res_expr.node))
        elif tok.datatype == tt._RAISE:
            self.advance()
            if self.current_token.datatype in [tt._NEWLINE, tt._EOF]:
                return res.failure(Error("Expected Exception", self.current_token.start, self.current_token.end))
            res_expr = self.expr()
            if res_expr.error: return res_expr
            return res.success(Raise_node(tok, res_expr.node))
        elif tok.datatype == tt._PASS:
            self.advance()
            return res.success(Pass_node(tok))
        elif tok.datatype == tt._DELETE:
            self.advance()
            if self.current_token.datatype in [tt._NEWLINE, tt._EOF]:
                return res.failure(Error("Expected Expression", self.current_token.start, self.current_token.end))
            res_expr = self.expr()
            if res_expr.error: return res_expr
            return res.success(Delete_node(tok, res_expr.node))
        elif tok.datatype == tt._BREAK:
            self.advance()
            return res.success(Break_node(tok))
        elif tok.datatype == tt._CONTINUE:
            self.advance()
            return res.success(Continue_node(tok))
        elif tok.datatype == tt._COMMENT:
            self.advance()
        elif tok.datatype == tt._MULTI_COMMENT:
            self.advance()
            return res.success(Multi_line_comment_node(tok))
        else:
            res_expr = self.expr()
            if res_expr.error: return res_expr
            is_tuple = False
            tuple_entries = []
            while self.current_token.datatype not in [tt._NEWLINE, tt._EOF]:
                tok = self.current_token
                if tok.datatype in tt._ASSIGN_OPS:
                    if is_tuple:
                        tuple_entries.append(res_expr.node)
                        id = Tuple_node(Expression_list_node(tuple_entries))
                    else:
                        id = res_expr.node
                    assign_op = tok
                    self.advance()
                    # Parse RHS first expression
                    rhs_first = self.expr()
                    if rhs_first.error: return rhs_first
                    # Collect tuple if comma-separated
                    rhs_nodes = [rhs_first.node]
                    while self.current_token.datatype == tt._COMMA:
                        self.advance()
                        next_expr = self.expr()
                        if next_expr.error: return next_expr
                        rhs_nodes.append(next_expr.node)
                    rhs_node = Tuple_node(Expression_list_node(rhs_nodes)) if len(rhs_nodes) > 1 else rhs_first.node
                    return res.success(Assign_node(assign_op, id, rhs_node))
                elif tok.datatype == tt._COMMA:
                    tuple_entries.append(res_expr.node)
                    is_tuple = True
                    self.advance()
                    res_expr = self.expr()
                    if res_expr.error: return res_expr
                else:
                    break
            if is_tuple:
                tuple_entries.append(res_expr.node)
                return res.success(Tuple_node(Expression_list_node(tuple_entries)))
            return res.success(res_expr.node)
        return res

    def if_stmt(self):
        res = Parse_result()
        self.advance()
        cond_res = self.expr()
        if cond_res.error: return cond_res
        condition = cond_res.node
        if self.current_token.datatype != tt._COLON:
            return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
        self.advance()
        then_block_res = self.scope_block()
        if then_block_res.error: return then_block_res
        then_block = then_block_res.node

        elif_nodes = []
        while self.current_token and self.current_token.datatype == tt._ELIF:
            self.advance()
            elif_cond_res = self.expr()
            if elif_cond_res.error: return elif_cond_res
            elif_condition = elif_cond_res.node
            if self.current_token.datatype != tt._COLON:
                return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
            self.advance()
            elif_block_res = self.scope_block()
            if elif_block_res.error: return elif_block_res
            elif_block = elif_block_res.node
            elif_nodes.append(Elif_node(elif_condition, elif_block))

        else_node = None
        if self.current_token and self.current_token.datatype == tt._ELSE:
            self.advance()
            if self.current_token.datatype != tt._COLON:
                return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
            self.advance()
            else_block_res = self.scope_block()
            if else_block_res.error: return else_block_res
            else_node = Else_node(else_block_res.node)

        return res.success(If_node(condition, then_block, elif_nodes if elif_nodes else None, else_node))

    def for_stmt(self):
        res = Parse_result()
        self.advance()
        ids = []
        if self.current_token.datatype != tt._IDENTIFIER:
            return res.failure(Error("Expected Identifier", self.current_token.start, self.current_token.end))
        while True:
            atom_res = self.atom()
            if atom_res.error: return atom_res
            ids.append(atom_res.node)
            if self.current_token.datatype == tt._COMMA:
                self.advance()
                if self.current_token.datatype != tt._IDENTIFIER:
                    return res.failure(Error("Expected Identifier", self.current_token.start, self.current_token.end))
                continue
            break
        targets = Targets_node(ids)
        if self.current_token.datatype != tt._IN:
            return res.failure(Error("Expected 'in'", self.current_token.start, self.current_token.end))
        self.advance()
        expr_res = self.expr()
        if expr_res.error: return expr_res
        iterable = expr_res.node
        if self.current_token.datatype != tt._COLON:
            return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
        self.advance()
        block_res = self.scope_block()
        if block_res.error: return block_res
        block = block_res.node

        else_node = None
        if self.current_token and self.current_token.datatype == tt._ELSE:
            self.advance()
            if self.current_token.datatype != tt._COLON:
                return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
            self.advance()
            else_block_res = self.scope_block()
            if else_block_res.error: return else_block_res
            else_node = Else_node(else_block_res.node)

        return res.success(For_node(targets, iterable, block, else_node))

    def while_stmt(self):
        res = Parse_result()
        self.advance()
        cond_res = self.expr()
        if cond_res.error: return cond_res
        condition = cond_res.node
        if self.current_token.datatype != tt._COLON:
            return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
        self.advance()
        block_res = self.scope_block()
        if block_res.error: return block_res
        block = block_res.node

        else_node = None
        if self.current_token and self.current_token.datatype == tt._ELSE:
            self.advance()
            if self.current_token.datatype != tt._COLON:
                return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
            self.advance()
            else_block_res = self.scope_block()
            if else_block_res.error: return else_block_res
            else_node = Else_node(else_block_res.node)

        return res.success(While_node(condition, block, else_node))

    def class_definition(self):
        res = Parse_result()
        self.advance()
        if self.current_token.datatype != tt._IDENTIFIER:
            return res.failure(Error("Expected Identifier", self.current_token.start, self.current_token.end))
        class_id_res = self.atom()
        if class_id_res.error: return class_id_res
        class_id = class_id_res.node
        base_args = None
        if self.current_token.datatype == tt._LPARAN:
            self.advance()
            if self.current_token.datatype == tt._RPARAN:
                base_args = Expression_list_node([])
                self.advance()
            else:
                args_res = self.expression_list(tt._RPARAN)
                if args_res.error: return args_res
                base_args = args_res.node
        if self.current_token.datatype != tt._COLON:
            return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
        self.advance()
        block_res = self.scope_block()
        if block_res.error: return block_res
        block = block_res.node
        return res.success(Class_decl_node(class_id, [base_args, block] if base_args else [block]))

    def switch_stmt(self):
        res = Parse_result()
        self.advance()
        expr_res = self.expr()
        if expr_res.error: return expr_res
        switch_expr = expr_res.node
        if self.current_token.datatype != tt._COLON:
            return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
        self.advance()
        if self.current_token.datatype != tt._NEWLINE:
            return res.failure(Error("Expected NEWLINE", self.current_token.start, self.current_token.end))
        self.advance()
        if self.current_token.datatype != tt._INDENT:
            return res.failure(Error("Expected INDENT", self.current_token.start, self.current_token.end))
        self.advance()
        cases = []
        while True:
            if self.current_token.datatype != tt._CASE:
                return res.failure(Error("Expected 'case'", self.current_token.start, self.current_token.end))
            self.advance()
            expr_case_res = self.expr()
            if expr_case_res.error: return expr_case_res
            case_expr = expr_case_res.node
            if self.current_token.datatype != tt._COLON:
                return res.failure(Error("Expected ':'", self.current_token.start, self.current_token.end))
            self.advance()
            block_res = self.scope_block()
            if block_res.error: return block_res
            cases.append(Case_node(case_expr, block_res.node))
            if self.current_token.datatype == tt._DEDENT:
                break
            if self.current_token.datatype == tt._NEWLINE:
                self.advance()
            if self.current_token.datatype == tt._DEDENT:
                break
        if self.current_token.datatype != tt._DEDENT:
            return res.failure(Error("Expected DEDENT", self.current_token.start, self.current_token.end))
        self.advance()
        return res.success(Switch_node(switch_expr, cases))
