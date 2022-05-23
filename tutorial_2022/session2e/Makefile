
INC_DIR := -I$(AMPTOOLS) -I$(shell root-config --incdir)
LIB_DIR := -L$(AMPTOOLS)/lib -L$(shell root-config --libdir)
LIBS := -lAmpTools $(shell root-config --glibs) -lstdc++

CXX := gcc
CXX_FLAGS := $(shell root-config --cflags)

default:  displayFitResults

displayFitResults:  displayFitResults.cc
	$(CXX) $(CXX_FLAGS) -o $@ $< $(INC_DIR) $(LIB_DIR) $(LIBS)
