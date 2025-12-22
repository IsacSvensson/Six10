from ast_node import *
import terminal_tokens as tt

class Environment:
    def __init__(self, parent=None):
        self.parent = parent
        self.values = {}

    def get(self, name):
        if name in self.values:
            return self.values[name]
        if self.parent:
            return self.parent.get(name)
        raise NameError(f"Undefined name: {name}")

    def set(self, name, value):
        self.values[name] = value

class BreakSignal(Exception):
    pass

class ContinueSignal(Exception):
    pass

class ReturnSignal(Exception):
    def __init__(self, value):
        self.value = value

class Function:
    def __init__(self, name, param_nodes, body_node, closure_env, evaluator):
        self.name = name
        self.param_nodes = param_nodes
        self.body_node = body_node
        self.closure_env = closure_env
        self.evaluator = evaluator

    def __call__(self, *args):
        call_env = Environment(self.closure_env)
        for i, p in enumerate(self.param_nodes):
            if i < len(args):
                call_env.set(p.value.value, args[i])
        try:
            return self.evaluator.eval(self.body_node, call_env)
        except ReturnSignal as rs:
            return rs.value

class ClassObject:
    def __init__(self, name, class_env, evaluator):
        self.name = name
        self.env = class_env
        self.evaluator = evaluator

    def __call__(self, *args):
        instance = Instance(self, {})
        try:
            init = self.env.get('__init__')
            if isinstance(init, Function):
                init(instance, *args)
                return instance
            elif callable(init):
                init(instance, *args)
        except NameError:
            pass
        return instance

class Instance:
    def __init__(self, klass, attrs):
        self.klass = klass
        self.attrs = attrs or {}

    def get(self, name):
        if name in self.attrs:
            return self.attrs[name]
        try:
            val = self.klass.env.get(name)
            if isinstance(val, Function):
                def bound_method(*args):
                    return val(self, *args)
                return bound_method
            return val
        except NameError:
            raise AttributeError(f"Attribute '{name}' not found")

    def set(self, name, value):
        self.attrs[name] = value

class Evaluator:
    def __init__(self):
        self.global_env = Environment()
        self.global_env.set('print', print)
        self.global_env.set('len', len)
        self.global_env.set('range', range)

        # Bootstrap built-ins for self-hosting
        import time as _time
        self.global_env.set('time', _time.time)
        self.global_env.set('ord', ord)
        self.global_env.set('chr', chr)

        def read(path):
            with open(path, 'r', encoding='utf-8') as f:
                return f.read()

        def write(path, data):
            with open(path, 'w', encoding='utf-8') as f:
                f.write(str(data))
                return True

        def open_file(path, mode):
            return open(path, mode, encoding='utf-8')

        # Host bridge: expose Python lexer/parser/evaluator to Six10
        from lexer import Lexer
        from six10_parser import Parser

        def host_parse(source_text, filename="Six10Host"):
            """
            Lex and parse Six10 source and return the AST root node.
            Six10 code can traverse this via member access.
            """
            lex = Lexer(source_text, filename)
            lex.make_tokens()
            if lex.error:
                # return a Python dict with error info for inspection in Six10
                return {
                    "error": getattr(lex.error, "message", str(lex.error)),
                    "traceback": getattr(lex.error, "get_traceback", lambda: "")()
                }
            parser = Parser(lex.tokens)
            res = parser.parse()
            if res.error:
                return {
                    "error": str(res.error),
                    "traceback": res.error.get_traceback() if hasattr(res.error, "get_traceback") else ""
                }
            return res.node

        def host_eval(ast_root):
            """
            Evaluate a previously parsed AST root node using this Evaluator.
            """
            return self.eval(ast_root, self.global_env)

        def host_run(source_text, filename="Six10Host"):
            """
            Convenience: parse and evaluate a source string and return the result.
            """
            parsed = host_parse(source_text, filename)
            # If parse returned an error dict, just return it
            if isinstance(parsed, dict) and "error" in parsed:
                return parsed
            return host_eval(parsed)

        def host_run_file(path):
            """
            Convenience: read a file, then host_run it.
            """
            code = read(path)
            return host_run(code, path)

        self.global_env.set('read', read)
        self.global_env.set('write', write)
        self.global_env.set('open', open_file)

        # Expose host bridge built-ins
        self.global_env.set('host_parse', host_parse)
        self.global_env.set('host_eval', host_eval)
        self.global_env.set('host_run', host_run)
        self.global_env.set('host_run_file', host_run_file)

    def eval(self, node, env=None):
        if node is None:
            return None
        if env is None:
            env = self.global_env
        method_name = 'eval_' + node.get_type()
        method = getattr(self, method_name, self.generic_eval)
        return method(node, env)

    def generic_eval(self, node, env):
        result = None
        if node.children:
            for child in node.children:
                if child is not None:
                    result = self.eval(child, env)
        return result

    def eval_Program_node(self, node, env):
        return self.eval(node.children[0], env)

    def eval_Block_node(self, node, env):
        result = None
        for stmt in node.children:
            try:
                result = self.eval(stmt, env)
            except BreakSignal:
                raise
            except ContinueSignal:
                raise
            except ReturnSignal as rs:
                raise rs
        return result

    def eval_Number_node(self, node, env):
        tok = node.value
        return tok.value if hasattr(tok, 'value') else tok

    def eval_Bool_node(self, node, env):
        tok = node.value
        val = getattr(tok, 'value', None)
        return True if val == 'True' else False

    def eval_None_node(self, node, env):
        return None

    def eval_String_node(self, node, env):
        tok = node.value
        return getattr(tok, 'value', '')

    def eval_List_node(self, node, env):
        expr_list = node.children[0]
        return [self.eval(expr, env) for expr in expr_list.children]

    def eval_Tuple_node(self, node, env):
        expr_list = node.children[0]
        return tuple(self.eval(expr, env) for expr in expr_list.children)

    def eval_Expression_list_node(self, node, env):
        return [self.eval(expr, env) for expr in node.children]

    def eval_Identifier_call_node(self, node, env):
        name = node.value.value if hasattr(node.value, 'value') else str(node.value)
        return env.get(name)

    def set_target(self, target_node, value, env):
        if isinstance(target_node, Identifier_call_node):
            env.set(target_node.value.value, value)
        elif isinstance(target_node, Subscriber_call_node):
            container = self.eval(target_node.children[0], env)
            indices_node = target_node.children[1]
            if isinstance(indices_node, Slices_node):
                parts = indices_node.children
                start = self.eval(parts[0], env) if parts[0] else None
                stop = self.eval(parts[1], env) if parts[1] else None
                step = self.eval(parts[2], env) if parts[2] else None
                if stop is None and step is None:
                    container[start] = value
                else:
                    container[slice(start, stop, step)] = value
            else:
                idx = self.eval(indices_node.children[0], env)
                container[idx] = value
        elif isinstance(target_node, Member_access_node):
            target = self.eval(target_node.children[0], env)
            member_node = target_node.children[1]
            member_name = member_node.value.value if hasattr(member_node.value, 'value') else str(member_node.value)
            if isinstance(target, Instance):
                target.set(member_name, value)
            else:
                if isinstance(target, ClassObject):
                    target.env.set(member_name, value)
        
    def eval_Assign_node(self, node, env):
        target = node.children[0]
        value_node = node.children[1]
        value = self.eval(value_node, env)
        if isinstance(target, Tuple_node):
            id_list = target.children[0].children
            if isinstance(value, (list, tuple)) and len(id_list) == len(value):
                for i, idnode in enumerate(id_list):
                    self.set_target(idnode, value[i], env)
                return value
        else:
            self.set_target(target, value, env)
            return value
        return value

    def eval_Func_decl_node(self, node, env):
        func_name = node.value.value.value if hasattr(node.value, 'value') else str(node.value)
        params_list_node = node.children[0]
        block_node = node.children[1]
        param_nodes = []
        if isinstance(params_list_node, Expression_list_node):
            for p in params_list_node.children:
                if isinstance(p, Identifier_call_node):
                    param_nodes.append(p)
        func = Function(func_name, param_nodes, block_node, env, self)
        env.set(func_name, func)
        return func

    def eval_Class_decl_node(self, node, env):
        class_name = node.value.value.value if hasattr(node.value, 'value') else str(node.value)
        class_env = Environment(env)
        block_node = node.children[-1]
        self.eval(block_node, class_env)
        klass = ClassObject(class_name, class_env, self)
        env.set(class_name, klass)
        return klass

    def eval_Func_call_node(self, node, env):
        callee = self.eval(node.children[0], env)
        args = self.eval(node.children[1], env)
        if isinstance(args, list):
            return callee(*args)
        return callee()

    def eval_Subscriber_call_node(self, node, env):
        target = self.eval(node.children[0], env)
        slices = node.children[1]
        if isinstance(slices, Slices_node):
            parts = slices.children
            start = self.eval(parts[0], env) if parts[0] else None
            stop = self.eval(parts[1], env) if parts[1] else None
            step = self.eval(parts[2], env) if parts[2] else None
            if stop is None and step is None:
                return target[start]
            return target[slice(start, stop, step)]
        else:
            indices = [self.eval(expr, env) for expr in slices.children]
            return target[indices[0]]

    def eval_Member_access_node(self, node, env):
        target = self.eval(node.children[0], env)
        member_node = node.children[1]
        member_name = member_node.value.value if hasattr(member_node.value, 'value') else str(member_node.value)
        if isinstance(target, Instance):
            return target.get(member_name)
        if isinstance(target, ClassObject):
            try:
                return target.env.get(member_name)
            except NameError:
                raise AttributeError(f"Member '{member_name}' not found")
        if hasattr(target, member_name):
            return getattr(target, member_name)
        if isinstance(target, dict) and member_name in target:
            return target.get(member_name)
        if member_name == 'len':
            return lambda *args, **kwargs: len(target)
        raise AttributeError(f"Member '{member_name}' not found")

    def eval_Pre_unary_op_node(self, node, env):
        tok = node.value
        operand = self.eval(node.children[0], env)
        dt = getattr(tok, 'datatype', None)
        if dt == tt._PLUS:
            return +operand
        if dt == tt._MINUS:
            return -operand
        if dt == tt._NOT:
            return not operand
        if dt == tt._INCR:
            try:
                return operand + 1
            except Exception:
                return operand
        if dt == tt._DECR:
            try:
                return operand - 1
            except Exception:
                return operand
        return operand

    def eval_Bin_op_node(self, node, env):
        dt = getattr(node.value, 'datatype', None)
        left = self.eval(node.children[0], env)
        if dt == tt._AND:
            if not left:
                return left
            right = self.eval(node.children[1], env)
            return right
        if dt == tt._OR:
            if left:
                return left
            right = self.eval(node.children[1], env)
            return right
        right = self.eval(node.children[1], env)
        if dt == tt._PLUS:
            return left + right
        if dt == tt._MINUS:
            return left - right
        if dt == tt._MULT:
            return left * right
        if dt == tt._DIV:
            return left / right
        if dt == tt._FLOOR:
            return left // right
        if dt == tt._MOD:
            return left % right
        if dt == tt._EXP:
            return left ** right
        if dt == tt._BITWISE_OR:
            return left | right
        if dt == tt._BITWISE_XOR:
            return left ^ right
        if dt == tt._BITWISE_AND:
            return left & right
        if dt == tt._BITWISE_LSHIFT:
            return left << right
        if dt == tt._BITWISE_RSHIFT:
            return left >> right
        if dt == tt._BITWISE_GT:
            return left > right
        if dt == tt._BITWISE_LT:
            return left < right
        if dt == tt._BITWISE_GTE:
            return left >= right
        if dt == tt._BITWISE_LTE:
            return left <= right
        if dt == tt._BITWISE_EQ:
            return left == right
        if dt == tt._BITWISE_NOTEQ:
            return left != right
        return None

    def eval_Comparison_node(self, node, env):
        lhs = self.eval(node.children[0], env)
        rhs = self.eval(node.children[1], env)
        dt = getattr(node.value, 'datatype', None)
        if dt == tt._BITWISE_GT:
            return lhs > rhs
        if dt == tt._BITWISE_LT:
            return lhs < rhs
        if dt == tt._BITWISE_GTE:
            return lhs >= rhs
        if dt == tt._BITWISE_LTE:
            return lhs <= rhs
        if dt == tt._BITWISE_EQ:
            return lhs == rhs
        if dt == tt._BITWISE_NEQ:
            return lhs != rhs
        return None

    def eval_If_node(self, node, env):
        condition = self.eval(node.children[0], env)
        if condition:
            return self.eval(node.children[1], env)
        idx = 2
        if len(node.children) > 2 and isinstance(node.children[2], Expression_list_node):
            for elif_node in node.children[2].children:
                cond = self.eval(elif_node.children[0], env)
                if cond:
                    return self.eval(elif_node.children[1], env)
            idx = 3
        if len(node.children) > idx:
            return self.eval(node.children[idx], env)
        return None

    def eval_For_node(self, node, env):
        targets = node.children[0]
        iterable = self.eval(node.children[1], env)
        block = node.children[2]
        else_block = node.children[3] if len(node.children) > 3 else None
        broke = False
        for item in iterable:
            ids = targets.children
            if len(ids) == 1 and isinstance(ids[0], Identifier_call_node):
                env.set(ids[0].value.value, item)
            else:
                if isinstance(item, (list, tuple)) and len(item) == len(ids):
                    for i, idnode in enumerate(ids):
                        if isinstance(idnode, Identifier_call_node):
                            env.set(idnode.value.value, item[i])
            try:
                self.eval(block, env)
            except BreakSignal:
                broke = True
                break
            except ContinueSignal:
                continue
        if else_block and not broke:
            return self.eval(else_block.children[0], env)
        return None

    def eval_While_node(self, node, env):
        condition_node = node.children[0]
        block = node.children[1]
        else_block = node.children[2] if len(node.children) > 2 else None
        broke = False
        while self.eval(condition_node, env):
            try:
                self.eval(block, env)
            except BreakSignal:
                broke = True
                break
            except ContinueSignal:
                continue
        if else_block and not broke:
            return self.eval(else_block.children[0], env)
        return None

    def eval_Return_node(self, node, env):
        value = self.eval(node.children[0], env) if node.children and node.children[0] else None
        raise ReturnSignal(value)

    def eval_Pass_node(self, node, env):
        return None

    def eval_Break_node(self, node, env):
        raise BreakSignal()

    def eval_Continue_node(self, node, env):
        raise ContinueSignal()

    def eval_If_else_expression_node(self, node, env):
        true_val_node = node.children[0]
        condition_node = node.children[1]
        else_val_node = node.children[2]
        cond = self.eval(condition_node, env)
        if cond:
            return self.eval(true_val_node, env)
        else:
            return self.eval(else_val_node, env)

    def eval_Switch_node(self, node, env):
        if not node.children:
            return None
        switch_expr_node = node.children[0]
        switch_val = self.eval(switch_expr_node, env)
        cases_nodes = []
        if len(node.children) > 1:
            cases_container = node.children[1]
            if isinstance(cases_container, Expression_list_node):
                cases_nodes = cases_container.children
            else:
                cases_nodes = node.children[1:]
        result = None
        for case_node in cases_nodes:
            if not case_node.children or len(case_node.children) < 2:
                continue
            case_expr_node = case_node.children[0]
            block_node = case_node.children[1]
            case_val = self.eval(case_expr_node, env)
            if switch_val == case_val:
                result = self.eval(block_node, env)
                break
        return result
