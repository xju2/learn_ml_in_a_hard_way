Examples are taken from [mpi4py tuotiral](https://mpi4py.readthedocs.io/en/stable/tutorial.html).

Commands to compile thw SWIG interface
```bash
MPI4_DIR=/Users/xju/miniconda3/envs/rlpy3p6/lib/python3.6/site-packages/mpi4py/include
swig -python -I$MPI4_DIR helloworld.i

cc -c `python-config --cflags` helloworld.c helloworld_wrap.c  -I$MPI4_DIR
cc -bundle `python-config --ldflags` helloworld.o helloworld_wrap.o -lmpi -o _helloworld.so
```

or directly use python distutils
```bash
python setup.py build_ext --inplace
```
