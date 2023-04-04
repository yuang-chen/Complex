# using pkg-config to find fmt
CXX:=g++
CXXFLAGS:=-O3 -std=c++20 -Wall -Wextra $(shell pkg-config --libs --cflags fmt)


#FMT=$(shell pkg-config --libs --cflags fmt)

target: louvain clique forest triangle 

all: $(target)

$(target): %: %.cpp
	$(CXX) $(CXXFLAGS)  -o $@ $< $(LDFLAGS) 

clean:
	rm -f $(target)