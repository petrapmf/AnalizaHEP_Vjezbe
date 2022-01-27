CC=g++
CFLAGS=-I. $(shell root-config --cflags)
LDFLAGS = $(shell root-config --libs)
DEPS = Height.h

%.o: %.cpp $(DEPS)
	$(CC) $(LDFLAGS) -c -o $@ $< $(CFLAGS)

analyze: analyze.o Height.o
	$(CC) $(LDFLAGS) -o analyze analyze.o Height.o  

clean:
	rm *.o $(objects) analyze