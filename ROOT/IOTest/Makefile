PROJECT = MyLib


CPPFLAGS = -c -fPIC -O3 -mavx2 -pedantic -Wall -Wno-unused-result -Wno-deprecated-declarations  $(shell root-config --cflags) -I/Users/xju/Generic/software/HighFive/include
ROOTLIBS = $(shell root-config --libs)
LDFLAGS = -lhdf5

CPP = $(shell root-config --cxx)
LD = $(shell root-config --ld)

cpplib = lib$(PROJECT).so 

cppheaders = 
cppobjects = 


### all: $(cpplib) merge_BDT 
all: merge_BDT  testHDF5ROOT

%.o:%.cxx $(cppheaders)
	$(CPP) $(CPPFLAGS) -o $@ $^

merge_BDT: merge_BDT.o
	$(CPP) $(LDFLAGS) $(ROOTLIBS) -o $@ $< 

testHDF5ROOT: testHDF5ROOT.o
	$(CPP) $(LDFLAGS) $(ROOTLIBS) -o $@ $< 



$(cpplib): $(cppobjects)
	$(LD) $(cppobjects) -shared $(LDFLAGS) -o $(cpplib) 

clean:
	rm -f $(cppobjects) $(cpplib) merge_BDT merge_BDT.o

