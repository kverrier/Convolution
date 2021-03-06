CC=g++ -m64
ISPC=ispc
ISPCFLAGS=-O2 --target=sse4-x2 --arch=x86-64
CFLAGS=-c -Wall -Wextra -fopenmp -O3 -msse3
LDFLAGS=-lm -lfftw3 -lstdc++ -fopenmp -lpthread
SRCS= bench/main.cpp \
			src/FFTFrame.cpp \
			src/conv.cpp \
			src/ReverbAccumulationBuffer.cpp \
			src/UniformConvolver.cpp
OBJS= $(SRCS:.cpp=.o)
TARGET=convolution

.PHONY: clean lint

all: $(SRCS) $(TARGET)

run: $(TARGET)
	OMP_NUM_THREADS=2 ./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
		
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *o $(TARGET)

lint : cpplint.py $(SRCS)
	python cpplint.py $(SRCS)

cpplint.py:
	wget http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py
	chmod +x cpplint.py

