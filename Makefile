CC	= g++
CFLAGS = -Wall -O3 -std=c++11
SRCS = main.cpp Board.cpp

OBJS=$(SRCS:.cpp=.o)

ot: $(OBJS)
	$(CC) $(CFLAGS) -o othello $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f othello *.o *.so

.PHONY: test clean