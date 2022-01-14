from error import Error
from lex_token import Token
import terminal_tokens as tt
from ast_node import Bin_op_node, If_else_expression_node, Program_node
from ast_node import Pre_unary_op_node, Post_unary_op_node, Number_node
from ast_node import Comparison_node, Func_call_node, Subscriber_call_node
from ast_node import Identifier_call_node, Bool_node, None_node

class Parse_result:
    """
    Class for holding parse results
    """
    def __init__(self):
        """
        Inits with none values
        """
        self.error = None
        self.node = None

    def register(self, res):
        """
        Registers if anything went wrong
        """
        if isinstance(res, Parse_result):
            if res.error: self.error = res.error
            return res.node
        
        return res

    def success(self, node):
        """
        Registers successfully created nodes
        """
        self.node = node
        return self

    def failure(self, error):
        """
        Registers failures
        """
        self.error = error
        return self


class Parser:
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

        return res

    def statements(self):
        res = Parse_result()
        # Holds all statements that is the program
        stmts = []

        # Runs until EOF is encountered
        while self.current_token.datatype != tt._EOF:
            if self.current_token.datatype in tt._CMP_STMT:
                # Compound statments
                node = res.register(self.compound_statment())
            else:
                # Small statements
                node = res.register(self.small_statment())
            if res.error: return res
            if node:
                # If node extracted add to list.
                stmts.append(node)
            # Skip blank lines
            while self.current_token.datatype == tt._NEWLINE:
                self.advance()
        # Entrypoint of AST.
        res.success(Program_node(self.tokens[0].start.filename, stmts))
        return res

    def compound_statment(self):
        """
        Parses definitions of functions and classes and if/while/for/switch-statements  
        """
        raise NotImplementedError

    def small_statment(self):
        """
        Parsing regular one-line-statements.
        """
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
            # If not anything above, it's a expression.
            res = self.expr()
            #TODO: Implement assignment here.
        return res


    def expr(self):
        """
        Expression parser. Either regular expression or if/else expression
        """
        res = self.disjunction()
        if res.error: return res

        # If/else expression
        if self.current_token.datatype == tt._IF:
            # Value to use if condition is true
            true_val = res.node
            self.advance()
            # Get condition
            res = self.expr()
            if res.error: return res
            condition = res.node
            if self.current_token.datatype != tt._ELSE:
                return res.failure(Error("Expected 'else'",
                    self.current_token.start, self.current_token.end))
            self.advance()
            # Value to use if condition is False
            res = self.expr()
            if res.error: return res
            else_val = res.node
            node = If_else_expression_node(true_val, condition, else_val)
            return res.success(node)
        return res

    def disjunction(self):
        """
        Parsing disjunction:
            conjunction ('or' conjunction)+
          | conjunction
        """
        return self.bin_op(self.conjunction, (tt._OR,))

    def conjunction(self):
        """
        Parsing conjunction:
            invertion ('and' invertion)+
          | invertion
        """
        return self.bin_op(self.invertion, (tt._AND,))

    def invertion(self):
        """
        Parsing invertion:
            'not' invertion
          | comparison
        """
        res = Parse_result()
        # If negation
        if self.current_token.datatype == tt._NOT:
            tok = self.current_token
            res.register(self.advance())
            invertion = res.register(self.invertion())
            if res.error: return res

            return res.success(Pre_unary_op_node(tok, invertion))
        # Else
        return self.comparison()

    def comparison(self):
        """
        Parsing comparison:
            bitwise_or compare_pair+
          | bitwise_or
        """
        res = self.bitwise_or()

        # If comparison of some kind
        if self.current_token.datatype in tt._COMPARISON:
            tok = self.current_token
            lhs = res.node
            # If 'not in'
            if self.current_token.datatype == tt._NOT:
                self.advance()
                if self.current_token.datatype == tt._IN:
                    self.advance()
                    res = self.bin_op(self.bitwise_or, (tt._NOT_IN))
                    if res.error: return res
                else:
                    return res.failure(Error("Expected 'in'", 
                        self.current_token.start, self.current_token.end))
                rhs = res.node
            elif self.current_token.datatype == tt._BITWISE_IS:
                self.advance()
                if self.current_token.datatype == tt._NOT:
                    # If is not
                    self.advance()
                    res = self.bin_op(self.bitwise_or, (tt._BITWISE_IS_NOT))
                    if res.error: return res
                else:
                    # if is
                    res = self.bin_op(self.bitwise_or, (tt._BITWISE_IS_NOT))
                    if res.error: return res
                rhs = res.node
            else:
                # all other comparisons
                res = self.bin_op(self.bitwise_or, tt._COMPARISON)
                if res.error: return res
                rhs = res.node
            return res.success(Comparison_node(tok, lhs, rhs))
        return res

    def bitwise_or(self):
        """
        Parsing + and -
        """
        return self.bin_op(self.term, (tt._PLUS, tt._MINUS))

    def term(self):
        """
        Parsing '*', '/', '%' and '//'
        """
        return self.bin_op(self.factor, (tt._MULT, tt._DIV, tt._MOD, tt._FLOOR))

    def factor(self):
        """
        Handles unary '+' and '-'
        """
        res = Parse_result()
        tok = self.current_token

        # If unary plus or minus
        if tok.datatype in (tt._PLUS, tt._MINUS):
            res.register(self.advance())
            factor = res.register(self.factor())
            if res.error: return res

            return res.success(Pre_unary_op_node(tok, factor))
        else:
            return self.un_op(self.unary_variation, (tt._INCR, tt._DECR))

    def unary_variation(self):
        """
        Handles pre/post increase/decrease
        """
        return self.bin_op(self.power, (tt._EXP,))
            
    def power(self):
        """
        Handles exponents
        """
        return self.bin_op(self.primary, (tt._DOT,))

    def primary(self):
        """
        Handles members, subscribers and function calls
        """
        res = self.atom()
        if res.error: return res
        id = res.node
        if self.current_token.datatype == tt._LPARAN:
            self.advance()
            res = self.expression_list(tt._RPARAN)
            if res.error: return res
            if res.node:
                args = res.node
            else:
                return res.failure(Error("No ast-node was created."))
            return res.success(Func_call_node(id, args))
        elif self.current_token.datatype == tt._LSQBRACK:
            self.advance()
            res = self.expression_list(tt._RSQBRACK)
            if res.error: return res
            if res.node:
                args = res.node
            else:
                return res.failure(Error("No ast-node was created."))
            return res.success(Subscriber_call_node(id, args))
        return res

    def expression_list(self, terminator):
        """
        Handles lists of expressions, such as arguments/parameters or items in 
        containers
        """
        res = Parse_result()
        exprs = []

        while self.current_token.datatype != terminator:
            res = self.expr()
            if res.error: return res
            if res.node:
                exprs.append(res.node)
            if self.current_token.datatype == tt._COMMA:
                self.advance()
            elif self.current_token.datatype != terminator:
                return res.failure(Error(f"Expected ',' or {terminator}",
                    self.current_token.start, self.current_token.end))
        self.advance()

        return res.success(Expression_list_node(exprs))

    def atom(self):
        """
        Handles identifiers, litterals, tuples, lists, dicts, sets and 
        parenthesis enclosed expresions
        """
        res = Parse_result()
        tok = self.current_token
        if self.current_token.datatype == tt._IDENTIFIER:
            self.advance()
            return res.success(Identifier_call_node(tok))
        if self.current_token.datatype == tt._LPARAN:
            #TODO: Implement parenthesis enclosed expression and tuple
            raise NotImplementedError
        if self.current_token.datatype == tt._LSQBRACK:
            #TODO: Implement list
            raise NotImplementedError
        if self.current_token.datatype == tt._LCURLBRACK:
            #TODO: Implement set and dict
            raise NotImplementedError
        return self.litteral()

    def litteral(self):
        """
        Handles Bools, None, Strings and numbers
        """
        res = Parse_result()
        tok = self.current_token
        if tok.datatype == tt._TRUE or tok.datatype == tt._FALSE:
            self.advance()
            return res.success(Bool_node(tok))
        if tok.datatype == tt._NONE:
            self.advance()
            return res.success(None_node(tok))
        if tok.datatype == tt._STRING:
            self.advance()
            return res.success(None_node(tok))
        if tok.datatype == tt._INT or tok.datatype == tt._FLOAT:
            self.advance()
            return res.success(Number_node(tok))
        return res.failure("Expected litteral ('True'/'False', 'None', string or number)")

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