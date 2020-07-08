#!/bin/bash

FILE=merge_BDT
g++ -O3 -pipe -Wall -W -Woverloaded-virtual -fPIC -pthread \
	-I $ROOTSYS/include -o $FILE.o -c $FILE.cxx

g++ -O2 -m32 $FILE.o -L$ROOTSYS/lib -lCore -lCint -lRIO -lNet -lHist -lGraf \
	-lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore \
	-lThread -pthread -lm -ldl -rdynamic  -o $FILE
