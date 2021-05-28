from error import error
import readline
from sys import argv
import platform
from lexer import Lexer

def run_file(filename):
    source = str()

    with open() as fh:
        source = fh.read()
    
    lexer = Lexer(source, filename)
    lexer.make_tokens()
    for token in lexer.tokens:
        print(token)

def run_shell():
    console_input = None
    print("Six10 v.0\nRun on [{}]\n-------------".format(platform.system()))
    while console_input != "":
        print(">>> ", end="")
        console_input = input()
        lexer = Lexer(console_input, "Shell")
        lexer.make_tokens()
        if lexer.error:
            print(lexer.error.message)
            print(lexer.tokens[-1])
            continue
        for token in lexer.tokens:
            print(token)

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