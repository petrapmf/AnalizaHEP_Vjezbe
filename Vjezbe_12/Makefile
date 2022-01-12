CC=g++
CFLAGS=-I. $(shell root-config --cflags) -lTMVA
LDFLAGS = $(shell root-config --libs) -lTMVA
DEPS = Analyzer.h

%.o: %.cpp $(DEPS)
	$(CC) $(LDFLAGS) -c -o $@ $< $(CFLAGS)

analyze: analyze.o Analyzer.o
	$(CC) $(LDFLAGS) -o analyze analyze.o Analyzer.o  

clean:
	rm *.o $(objects) analyze