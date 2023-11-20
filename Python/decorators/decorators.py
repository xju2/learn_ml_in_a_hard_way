import functools
import time


def repeat_me(times):
    def decorator_repeat(func):
        @functools.wraps(func)
        def wrapper_repeat(*args, **kwargs):
            for itime in range(times):
                print("[{}]".format(itime))
                func(*args, **kwargs)
        return wrapper_repeat
    return decorator_repeat


def repeat(func):
    @functools.wraps(func)
    def wrapper_repeat(*args, **kwargs):
        if hasattr(func, 'times'):
            times = func.times
        else:
            times = 3
        for itime in range(times):
            print("[{}]".format(itime))
            func(*args, **kwargs)
    return wrapper_repeat


def do_twice(func):
    @functools.wraps(func)
    def wrapper_do_twice(*args, **kwargs):
        x1 = func(*args, **kwargs)
        x2 = func(*args, **kwargs)
        if x1 is not None:
            return "\n".join([x1, x2])
    return wrapper_do_twice


def timer(func):
    """Print the runtime of the decorated function"""
    @functools.wraps(func)
    def wrapper_timer(*args, **kwargs):
        start_time = time.time()
        value = func(*args, **kwargs)
        end_time = time.time()
        run_time = end_time - start_time
        print("Finsihed {} in {:.2f} secs".format(func.__name__, run_time))
        return value
    return wrapper_timer
