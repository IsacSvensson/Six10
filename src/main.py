from error import Error
import readline
from sys import argv
import platform
from lexer import Lexer
from six10_parser import Parser

def run_file(filename):
    source = str()

    with open(filename, encoding='utf8') as fh:
        source = fh.read()
    
    lexer = Lexer(source, filename)
    lexer.make_tokens()
    if lexer.error:
        print(lexer.error)
        return
    for token in lexer.tokens:
        print(token)
    parser = Parser(lexer.tokens)

    res = parser.parse()
    if res.error:
        print(res.error)
        return
    res.node.print_tree()

def run_shell():
    console_input = None
    print("Six10 v.0\nRun on [{}]\n-------------".format(platform.system()))
    while console_input != "":
        print(">>> ", end="")
        console_input = input()
        if console_input:
            lexer = Lexer(console_input, "Shell")
            lexer.make_tokens()
            if lexer.error:
                print(lexer.error.message)
                print(lexer.tokens[-1])
                continue
            parser = Parser(lexer.tokens)
            try:
                res = parser.parse()
                if res.error:
                    print(res.error)
                elif res.node:
                    res.node.print_tree()
                else:
                    print("No node, and no error")
            except NotImplementedError:
                print("Feature not implemented")

def main():
    """
    Main method for running the language.
    """
    if len(argv) > 1:
        run_file(argv[1])
    else:
        run_shell()


if __name__ == "__main__":
    main()