#!/usr/bin/env python
from __future__ import print_function

import colorama
colorama.init(autoreset=True)

from colorama import Fore, Back, Style
import os

def info_msg(msg):
    print(Back.GREEN+"[{}]:[INFO]".format(os.getpid()), Fore.GREEN+msg)

def warning_msg(msg):
    print(Back.YELLOW+"[{}]:[WARNING]".format(os.getpid()), Fore.YELLOW+msg)

def error_msg(msg):
    print(Back.RED+"[{}]:[ERROR]".format(os.getpid()), Fore.RED+msg)

if __name__ == "__main__":
    info_msg("keep driving")
    warning_msg("sharp turn ahead, slow down")
    error_msg("did not stop at stop sign")
    info_msg("your destination is behind you")

    print('\033[31m' + 'some red text')
    print('\033[30m') # and reset to default color

    from termcolor import colored
    print(colored('Hello, World!', 'green', 'on_red'))
