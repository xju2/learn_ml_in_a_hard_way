PROJECT = MyLib


CPPFLAGS = -c -fPIC -O3 -mavx2 -pedantic -Wall -Wno-unused-result -Wno-deprecated-declarations  -I$(shell root-config --cflags)
#LDFLAGS = $(shell root-config --libdir)
#LDFLAGS = $(shell root-config --ldflags)
ROOTLIBS = $(shell root-config --libs)
LDFLAGS = 

CPP = $(shell root-config --cxx)
LD = $(shell root-config --ld)

cpplib = lib$(PROJECT).so 

cppheaders = 
cppobjects = 


### all: $(cpplib) merge_BDT 
all: sanity_check

%.o:%.cxx $(cppheaders)
	$(CPP) $(CPPFLAGS) -o $@ $^

sanity_check: sanity_check.o
	# $(CPP) $(LDFLAGS) $(ROOTLIBS) -Wl,-rpath,./ -o $@ $< 
	$(CPP) $(LDFLAGS) $(ROOTLIBS) -o $@ $< 


$(cpplib): $(cppobjects)
	$(LD) $(cppobjects) -shared $(LDFLAGS) -o $(cpplib) 

clean:
	rm -f $(cppobjects) $(cpplib) merge_BDT merge_BDT.o

