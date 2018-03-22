CC = g++
CFLAGS=-c -g -fpermissive -D emu
SRCS=$(wildcard *.cpp)
INCLUDES=$(wildcard *.h)
OBJS=$(SRCS:.cpp=.o)
MAKEFLAGS=--ignore-errors

all:	prebuild main

prebuild:
	rm main

main: $(OBJS)
	$(CC) -o $@ $(OBJS) 

$(OBJS) : %.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o main mainPi

