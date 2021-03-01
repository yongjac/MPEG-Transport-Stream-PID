CXX=g++
RM=rm -f
CPPFLAGS=-pthread

SRCS=*.cpp
OBJS=$(subst .cc,.o,$(SRCS))

all: task

task: $(OBJS)
	$(CXX) $(CPPFLAGS) -o task $(OBJS)

.PHONY: clean

clean:
	$(RM) $(OBJS)
