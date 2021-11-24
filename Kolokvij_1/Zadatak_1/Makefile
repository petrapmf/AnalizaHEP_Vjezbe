CXX=g++ -std=c++11
CXXFLAGS=-I.
DEPS = Statistics.h

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

analyzer: analyzer.o Statistics.o 
	$(CXX) -o analyzer analyzer.o Statistics.o  

clean:
	rm *.o $(objects) analyzer