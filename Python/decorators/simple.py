#!/usr/bin/env python
from datetime import datetime
from decorators import do_twice
import decorators

@decorators.repeat
def say_whee():
    print("Whee!")


@do_twice
def greet(name):
    print("Hello {}".format(name))


@do_twice
def return_greeting(name):
    print("Creating greeting")
    return "Hi {}".format(name)

@decorators.timer
def waste_some_time(num_times):
    for _ in range(num_times):
        sum([i**2 for i in range(10000)])

if __name__ == "__main__":
    say_whee()
    # now repeat 6 times
    say_whee.times = 6
    say_whee()
    print(say_whee.times)
    greet("xiangyang")
    greetings = return_greeting("Ju")
    print(greetings)

    waste_some_time(10)
