
CC := gcc
CXX := g++
CFLAGS := -g -O2 -std=c++11 -Wno-deprecated -Wall -fPIC

SOURCES=$(wildcard *.cpp *.c)

OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OUTLIB := libbase_utils.a

all: $(OBJS) $(OUTLIB)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(OUTLIB): $(OBJS)
	$(CC) -shared $(CFLAGS) -o $(OUTLIB) $(OBJS)

.PHONY: clean install

cleanall:
	rm *.o *.so *.a




