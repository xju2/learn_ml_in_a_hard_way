def green(word):
    return '\033[92m{}\033[0m'.format(word)

def blue(word):
    return '\033[94m{}\033[0m'.format(word)

def warning(word):
    return '\033[93m{}\033[0m'.format(word)

def colored(word, color):
    cc = color[0]
    if cc == 'r':
        return '\x1b[31m{}\x1b[0m'.format(word)
    elif cc == 'g':
        return '\x1b[32m{}\x1b[0m'.format(word)
    elif cc == 'y':
        return '\x1b[33m{}\x1b[0m'.format(word)
    elif cc == 'b':
        return '\x1b[34m{}\x1b[0m'.format(word)
    elif cc == 'm':
        return '\x1b[35m{}\x1b[0m'.format(word)
    elif color[:2] == 'dr':
        return '\033[95m{}\033[0m'.format(word)
    elif color[:2] == 'dg':
        return '\033[96m{}\033[0m'.format(word)
    elif color[:2] == 'dy':
        return '\033[97m{}\033[0m'.format(word)
    elif color[:2] == 'lr':
        return '\033[91m{}\033[0m'.format(word)
    else:
        return '\033[95m{}\033[0m'.format(word)
    pass

if __name__ == '__main__':
    str = "HELLO WORLD"
    print(colored(str, 'r'))
    print(colored(str, 'g'))
    print(colored(str, 'y'))
    print(colored(str, 'b'))
    print(colored(str, 'm'))
    print(colored(str, 'dr'))
    print(colored(str, 'dg'))
    print(colored(str, 'dy'))
    print(colored(str, 'lr'))