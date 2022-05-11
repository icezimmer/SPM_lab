ifndef FF_ROOT
FF_ROOT		= $(HOME)/fastflow
endif

CXX		= g++-11 -std=c++20  #-DNO_DEFAULT_MAPPING
INCLUDES	= -I $(FF_ROOT) 
CXXFLAGS  	= -Wall

LDFLAGS 	= -pthread -fopenmp
OPTFLAGS	= -O3 -finline-functions -DNDEBUG

TARGETS		=  mainpar / mainomp / mainalt


.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)

clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~