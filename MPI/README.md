```bash
swig -python helloworld.i
cc -c `python-config --cflags` helloworld.c helloworld_wrap.c  -I/Users/xju/miniconda3/envs/py2.7.13/lib/python2.7/site-packages/mpi4py/include/
cc -bundle `python-config --ldflags` helloworld.o helloworld_wrap.o -lmpi -o _helloworld.so
```

```bash
python setup.py build_ext --inplace
```
