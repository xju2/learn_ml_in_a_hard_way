#!/usr/bin/env python
from __future__ import print_function

import colorama
colorama.init(autoreset=True)

from colorama import Fore, Back, Style

def warning_msg(msg):
    print(Back.YELLOW+"[WARNING]", Fore.YELLOW+msg)

def error_msg(msg):
    print(Back.RED+"[ERROR]", Fore.RED+msg)

if __name__ == "__main__":
    warning_msg("sharp turn ahead, slow down")
    error_msg("did not stop at stop sign")

    print('\033[31m' + 'some red text')
    print('\033[30m') # and reset to default color

    from termcolor import colored
    print(colored('Hello, World!', 'green', 'on_red'))
