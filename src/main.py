from error import Error
import readline
from sys import argv
import platform
from lexer import Lexer
from six10_parser import Parser
from interpreter import Evaluator
import sys
import time

def run_file(filename):
    source = str()

    with open(filename, encoding='utf8') as fh:
        source = fh.read()
    lexer = Lexer(source, filename)
    start = time.time()
    lexer.make_tokens()
    if lexer.error:
        print("Error:", lexer.error)
        return
    parser = Parser(lexer.tokens)

    res = parser.parse()
    if res.error:
        print("Error:", res.error)
        print("Traceback:")
        print(res.error.get_traceback())
        return
    evaluator = Evaluator()
    result = evaluator.eval(res.node)
    end = time.time()
    print(result)
    print(f"Elapsed: {end - start:.6f} s")

def run_shell():
    evaluator = Evaluator()
    print("Six10 v.0\nRun on [{}]\n-------------".format(platform.system()))
    while True:
        try:
            line = input(">>> ")
        except KeyboardInterrupt:
            print()
            break
        if not line:
            continue
        lexer = Lexer(source_code=line, filename="Shell")
        start = time.time()
        lexer.make_tokens()
        if lexer.error:
            print("Error:", lexer.error.message)
            print(lexer.tokens[-1])
            continue
        parser = Parser(lexer.tokens)
        res = parser.parse()
        if res.error:
            print("Error:", res.error)
            print("Traceback:")
            print(res.error.get_traceback())
            continue
        try:
            result = evaluator.eval(res.node)
            end = time.time()
            print(result)
            print(f"Elapsed: {end - start:.6f} s")
        except Exception as e:
            print("Traceback:")
            print(e)

def main():
    """
    Main method for running the language.
    """
    argv = sys.argv
    if len(argv) > 1:
        run_file(argv[1])
    else:
        run_shell()


if __name__ == "__main__":
    main()