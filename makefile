###########################################################################
#
# 5/Aug/04: The /usr/lib/gcc-lib/i386-redhat-linux/2.96/libg2c.a library
#           is needed for the Fortran Interface. If this is used the 
#           mainAnalysis.o during linking must be BEFORE the -lg2c    --SP
#
# 3/Apr/05: Defined some static rules to simplify handling and output --LF
#
###########################################################################

# Root variables
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -lMinuit  -lRooFit -lRooStats -lRooFitCore -lFoam -lHistFactory -lXMLParser 

# Programs
CXX          = g++
CXXFLAGS     = -g -Wall -fPIC -Wno-deprecated 
LDFLAGS      = -g 
SOFLAGS      = -shared 

INCDIR      += -Isrc

# Direct to make the directories with the sources:
VPATH  = ./lib / ./bin ./src ./aux

# Assign or Add variables
CXXFLAGS    += $(ROOTCFLAGS)
CXXFLAGS    += $(INCDIR)
LIBS        += $(ROOTLIBS)

GARBAGE      = *~ lib/*.so lib/*.o ./*/*.o bin/*

############################################################################
# Build main.c compiling only the first prereq: ($<) means main.c
############################################################################
getWeights : getWeights.o
SRStudies : SRStudies.o miniTree.o

kinematicMaker : kinematicMaker.o miniTree.o
kinematicMaker_exclTempl : kinematicMaker_exclTempl.o miniTree.o
kinematicMaker_softJetTempl : kinematicMaker_softJetTempl.o miniTree.o 
kinematicMaker_subjetTempl : kinematicMaker_subjetTempl.o miniTree.o

dresser : dresser.o miniTree.o
dresser_exclTempl : dresser_exclTempl.o miniTree.o
dresser_softJetTempl : dresser_softJetTempl.o miniTree.o
dresser_subjetTempl : dresser_subjetTempl.o miniTree.o

eventSelection : eventSelection.o nominalMC.o nominalData.o

############################################################################
# General rules. The first two need not be specified due to implicit rules, 
# but redefined for the "echo"
############################################################################
%.o : %.cc %.hh
	@echo "compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< 

%.o : %.cc 
	@echo "compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< 

%    : %.o
	@echo "compiling and linking $@"
	@$(CXX) $(LDFLAGS) $^ \
	        $(FORTRANLIBS) $(ROOTLIBS)  -o $@
	@if [ `ls | grep "\.so"$ | wc -l` != 0 ]; then mv *.so lib/; fi
	@if [ `ls | grep "\.o"$  | wc -l` != 0 ]; then mv *.o  lib/; fi
	@mv $@ bin/
############################################################################
# Phony rules (no prerequisites)
############################################################################
.PHONY: clean cln

kin : kinematicMaker kinematicMaker_exclTempl kinematicMaker_softJetTempl kinematicMaker_subjetTempl
dress : dresser dresser_exclTempl dresser_softJetTempl dresser_subjetTempl
select : eventSelection getWeights

all : kin dress select

clean :
	@rm -f $(GARBAGE)

print :
	@echo compiler  : $(CXX)
	@echo c++ flags : $(CXXFLAGS)
	@echo libs      : $(LIBS)
	@echo so flags  : $(SOFLAGS)
	@echo rootlibs  : $(ROOTLIBS)
