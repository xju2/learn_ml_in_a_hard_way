PROJECT = MyLib


CPPFLAGS = -c -fPIC -O3 -mavx2 -pedantic -Wall -Wno-unused-result -Wno-deprecated-declarations  $(shell root-config --cflags) -I/Users/xju/Generic/software/HighFive/include
ROOTLIBS = $(shell root-config --libs)
#LDFLAGS = -lhdf5

CPP = $(shell root-config --cxx)
LD = $(shell root-config --ld)

cpplib = lib$(PROJECT).so

cppheaders =
cppobjects =


### all: $(cpplib) merge_BDT
all: event_selection event_selection_off_branch  event_selection_cache event_selection_RNtuple

%.o:%.cxx $(cppheaders)
	    $(CPP) $(CPPFLAGS) -o $@ $^

event_selection: event_selection.o
	    $(CPP) $< $(LDFLAGS) $(ROOTLIBS) -o $@

event_selection_RNtuple: event_selection_RNtuple.o
	    $(CPP) $< $(LDFLAGS) $(ROOTLIBS) -o $@

event_selection_off_branch: event_selection_off_branch.o
	    $(CPP) $< $(LDFLAGS) $(ROOTLIBS) -o $@

event_selection_cache: event_selection_cache.o
	    $(CPP) $< $(LDFLAGS) $(ROOTLIBS) -o $@


$(cpplib): $(cppobjects)
	    $(LD) $(cppobjects) -shared $(LDFLAGS) -o $(cpplib)

clean:
	    rm -f $(cppobjects) $(cpplib) *.o event_selection_off_branch event_selection_off_branch event_selection_cache event_selection_RNtuple
