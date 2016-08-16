SRC = myfirstprogram.cc
OBJ = $(SRC:%.cc=%.o)
DEPENDFILE = .depend
BOOST =   /work/speech/tools/boost/boost_1_53_0

# compiler binary
CXX = g++

# compiler and linker options
CXXFLAGS = -std=c++0x -O2 -m64 -mtune=native -fopenmp \
           -I. -I$(BOOST)/include
LDFLAGS := -L$(BOOST)/lib -lboost_program_options -lboost_filesystem \
                          -lboost_system -lboost_random -lboost_iostreams -lm \
           -lpthread -lgsl -lgslcblas \
           -Wl,-rpath,$(BOOST)/lib

# default rule
all: dep mfpg

-include $(DEPENDFILE) # "-" means do not stop if file does not exist

rwthlm: $(OBJ)

.PHONY: clean dep

clean:
	rm -f *.o mfpg 

dep: $(SRC)
	$(CXX) $(CXXFLAGS) -MM $(SRC) > $(DEPENDFILE)

# implicit rules (-n show commands, -r no implicit rules)
mfpg: $(OBJ)
	$(CXX) $(CXXFLAGS) -o mfpg $(OBJ) $(LDFLAGS)
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ -c $(@:%.o=%.cc)
