CC=g++ -m64
CFLAGS=-c -Wall -Wextra -fopenmp -O3 -msse3
LDFLAGS=-lm -lfftw3 -lstdc++ -fopenmp -lpthread
SRCS= main.cpp \
			Util.cpp
OBJS= $(SRCS:.cpp=.o)
TARGET=sim

.PHONY: clean lint

run: all
	./$(TARGET)

all: $(SRCS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
		
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *o $(TARGET)

