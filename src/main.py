from sys import argv
import platform
from lexer import Lexer

def main():
    if len(argv) > 1:
        source = str()

        print(argv)
        with open(argv[1]) as fh:
            source = fh.read()
        
        lexer = Lexer(source, argv[1])
        lexer.make_tokens()
        for token in lexer.tokens:
            print(token)
    else:
        console_input = None
        print("Six10 v.0\nRun on [{}]\n-------------".format(platform.system()))
        while console_input != "":
            print(">>> ", end="")
            console_input = input()
            lexer = Lexer(console_input, "Shell")
            lexer.make_tokens()
            for token in lexer.tokens:
                print(token)


if __name__ == "__main__":
    main()