// file: helloworld.i
%module helloworld

%{
#define SWIG_FILE_WITH_INIT
#include <mpi.h>
#include "helloworld.h"
%}

%include mpi4py/mpi4py.i
%mpi4py_typemap(Comm, MPI_Comm);
void sayhello(MPI_Comm comm);
