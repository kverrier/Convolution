CC=g++
CFLAGS=-c -Wall -Wextra
LDFLAGS=-lm -lfftw3 -lstdc++
SRCS= src/main.cpp \
			src/FFTFrame.cpp \
			src/conv.cpp \
			src/ReverbAccumulationBuffer.cpp \
			src/UniformConvolver.cpp
OBJS= $(SRCS:.cpp=.o)
TARGET=convolution

.PHONY: clean lint

all: $(SRCS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

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

