CC=g++
CFLAGS=-c -Wall -Wextra
LDFLAGS=-lm -lfftw3 #-lstdc++
SRCS=main.cpp


all: audio

run: audio
	./audio

audio: conv.o main.o
	$(CC) conv.o main.o -o audio $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

conv.o: conv.cpp
	$(CC) $(CFLAGS) conv.cpp
	
clean:
	rm -rf *o audio

lint : cpplint.py $(SRCS)
	python cpplint.py $(SRCS)

cpplint.py:
	wget http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py
	chmod +x cpplint.py
