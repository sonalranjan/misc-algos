
DEBUG = -g
CXXFLAGS = $(DEBUG) -Wall

SRCS = graph_utils.cpp parse_utils.cpp utils.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
TARG_SRCS = page_ranker.cpp
BINARY = $(patsubst %.cpp,%,$(TARG_SRCS))

all: $(BINARY)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

$(BINARY) : % : %.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $@.o $(LDFLAGS)

cln clean:
	$(RM) -rf $(OBJS) *.o core $(BINARY) $(patsubst %,%.dSYM,$(BINARY))
