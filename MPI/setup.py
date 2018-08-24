#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


helloworld_module = Extension('_helloworld',
                              sources=['helloworld_wrap.c', 'helloworld.c'],
                              include_dirs=['/Users/xju/miniconda3/envs/py2.7.13/lib/python2.7/site-packages/mpi4py/include']
                           )

setup (name = 'helloworld',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Simple swig helloworld from docs""",
       ext_modules = [helloworld_module],
       py_modules = ["helloworld"],
       )
