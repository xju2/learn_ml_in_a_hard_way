// file: helloworld.i
%module helloworld

%{
#define SWIG_FILE_WITH_INIT
#include <mpi.h>
#include "helloworld.h"
%}

%include /Users/xju/miniconda3/envs/py2.7.13/lib/python2.7/site-packages/mpi4py/include/mpi4py/mpi4py.i
%mpi4py_typemap(Comm, MPI_Comm);
void sayhello(MPI_Comm comm);
