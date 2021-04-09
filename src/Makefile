CXXFLAGS=-O1 -ggdb -Wall -std=c++2a



bitarray.test: bitarray.test.o bitarray.o bitarray.hh
	$(CXX) $(CXXFLAGS) -o $@ bitarray.o bitarray.test.o

bitarray.o: bitarray.cc bitarray.hh

bitarray.test.o: bitarray.test.cc


.PHONY clean:
	rm -f bitarray.o bitarray.test.o
