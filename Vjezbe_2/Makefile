CXX=g++
CXXFLAGS=-I.
DEPS = ElementaryParticle.h

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

analyzer: analyzer.o ElementaryParticle.o 
	$(CXX) -o analyzer analyzer.o ElementaryParticle.o  

clean:
	rm *.o $(objects) analyzer	