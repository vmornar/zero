CC = g++
CFLAGS=-c -g -fpermissive -D emu -Wno-deprecated
SRCS=$(wildcard *.cpp)
INCLUDES=$(wildcard *.h)
OBJS=$(SRCS:.cpp=.o)
MAKEFLAGS=--ignore-errors

all:	prebuild main

prebuild:
	rm main

main: $(OBJS)
	$(CC) -o $@ $(OBJS) -lws2_32

$(OBJS) : %.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o main mainPi

