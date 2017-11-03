TARGET := test

CXX := g++
CFLAGS := -Wreturn-type -O3 -g -std=c++11 -Iinc

LDFLAGS := -pthread -fopenmp

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %cpp,%o,$(SRCS))  

all: $(OBJS) 
	$(CXX) $^ $(CFLAGS) $(LDFLAGS) -o $(TARGET)
clean:  
	rm $(TARGET) $(OBJS)
%.o:%.cpp  
	$(CXX) $< $(CFLAGS) $(LDFLAGS) -c  -o $@ 
