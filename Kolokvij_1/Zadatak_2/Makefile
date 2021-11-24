CC=g++
CFLAGS=-I. $(shell root-config --cflags)
LDFLAGS = $(shell root-config --libs)
DEPS = train.h

%.o: %.cpp $(DEPS)
	$(CC) $(LDFLAGS) -c -o $@ $< $(CFLAGS)

analyze: analyze.o train.o
	$(CC) $(LDFLAGS) -o analyze analyze.o train.o  

clean:
	rm *.o $(objects) analyze