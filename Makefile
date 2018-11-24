CC = g++
CPPFLAGS = -g -Wall -std=c++11
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
PROG = platoondemo

OPENCV = `pkg-config --cflags --libs opencv`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(SRCS) $(LIBS)
