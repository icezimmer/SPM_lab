CXX		= g++-11 -std=c++17  #-DNO_DEFAULT_MAPPING
CXXFLAGS = -Wall

LDFLAGS 	= -pthread -fopenmp
OPTFLAGS	= -O3 -finline-functions -DNDEBUG

TARGETS =   main \ 
			main2

.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)

clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~
