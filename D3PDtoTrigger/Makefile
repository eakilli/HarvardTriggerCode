ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

ROOTCFLAGS   += -I$(ROOFITSYS)/include
ROOTGLIBS    += -L$(ROOFITSYS)/lib


CXX           = g++ -m64
CXXFLAGS      = -g -fPIC -Wno-deprecated -O -ansi -D_GNU_SOURCE -g -O2
LD            = g++ -m64
LDFLAGS       = -g
SOFLAGS       = -shared

#PG da qui per macosx
#PG -----------------

ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit
gGLIBS          = $(filter-out -lNew -lz, $(NGLIBS))

CXXFLAGS      += $(ROOTCFLAGS)
LIBS           = $(ROOTLIBS) 

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit -lRooFit -lRooFitCore
GLIBS          = $(filter-out -lNew -lz , $(NGLIBS))

INCLUDEDIR       = ./include
SRCDIR           = ./src/
CXX	         += -I$(INCLUDEDIR) -I.
OUTLIB	         = ./lib/

.SUFFIXES: .cc, .C, .hh
.PREFIXES: ./lib/

all:  lib D3PDtoTrigger.x AddHemtoTrigger.x AddVartoTrigger.x PrescaletoTrigger.x

PrescaletoTrigger.x:  $(SRCDIR)PrescaletoTrigger.C \
	$(OUTLIB)PrescaleBase.o \
	$(OUTLIB)PrescaleAnalysis.o 
	$(CXX) $(CXXFLAGS) -o PrescaletoTrigger.x $(OUTLIB)/*.o $(GLIBS) $ $<
	touch PrescaletoTrigger.x

D3PDtoTrigger.x:  $(SRCDIR)D3PDtoTrigger.C \
	$(OUTLIB)NTUP_SUSYBase.o \
	$(OUTLIB)D3PDAnalysis.o \
	$(OUTLIB)mt2_bisect.o 
	$(CXX) $(CXXFLAGS) -o D3PDtoTrigger.x $(OUTLIB)/*.o $(GLIBS) $ $<
	touch DP3DtoTrigger.x

AddVartoTrigger.x:  $(SRCDIR)AddVartoTrigger.C \
	$(OUTLIB)TriggerBase.o \
	$(OUTLIB)TriggerAnalysis.o \
	$(OUTLIB)mt2_bisect.o 
	$(CXX) $(CXXFLAGS) -o AddVartoTrigger.x $(OUTLIB)/*.o $(GLIBS) $ $<
	touch AddVartoTrigger.x

AddHemtoTrigger.x:  $(SRCDIR)AddHemtoTrigger.C \
	$(OUTLIB)TriggerBase.o \
	$(OUTLIB)TriggerAnalysis.o \
	$(OUTLIB)mt2_bisect.o 
	$(CXX) $(CXXFLAGS) -o AddHemtoTrigger.x $(OUTLIB)/*.o $(GLIBS) $ $<
	touch AddHemtoTrigger.x

lib: 	$(OUTLIB)mt2_bisect.o \
	$(OUTLIB)D3PDAnalysis.o \
	$(OUTLIB)TriggerAnalysis.o \
	$(OUTLIB)TriggerBase.o \
	$(OUTLIB)PrescaleBase.o \
	$(OUTLIB)PrescaleAnalysis.o \
	$(OUTLIB)NTUP_SUSYBase.o 

# analysis functions

$(OUTLIB)mt2_bisect.o: $(SRCDIR)mt2_bisect.cc 
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)mt2_bisect.o $<

$(OUTLIB)TriggerAnalysis.o: $(SRCDIR)TriggerAnalysis.cc $(OUTLIB)TriggerBase.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)TriggerAnalysis.o $<

#base class with ntuple structure for Prescale trees
$(OUTLIB)PrescaleBase.o: $(SRCDIR)PrescaleBase.cc 
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)PrescaleBase.o $<

$(OUTLIB)PrescaleAnalysis.o: $(SRCDIR)PrescaleAnalysis.cc $(OUTLIB)PrescaleBase.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)PrescaleAnalysis.o $<

# base class with ntuple structure for trigger trees
$(OUTLIB)TriggerBase.o: $(SRCDIR)TriggerBase.cc 
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)TriggerBase.o $<

$(OUTLIB)D3PDAnalysis.o: $(SRCDIR)D3PDAnalysis.cc $(OUTLIB)NTUP_SUSYBase.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)D3PDAnalysis.o $<

# base class with ntuple structure for D3PDs
$(OUTLIB)NTUP_SUSYBase.o: $(SRCDIR)NTUP_SUSYBase.cc 
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)NTUP_SUSYBase.o $<

clean:
	rm -f $(OUTLIB)*.o 
	rm -f ./*.x
	rm -r *.dSYM
