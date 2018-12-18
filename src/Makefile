CCFLAGS=-std=c++14
CC=g++

dtec: dtec.o model.o parser.o checker.o
	$(CC) -o dtec dtec.o model.o parser.o checker.o

dtec.o: dtec.cc model.hh
	$(CC) $(CCFLAGS) -c dtec.cc

model.o: model.cc model.hh
	$(CC) $(CCFLAGS) -c model.cc

parser.o: parser.cc model.hh
	$(CC) $(CCFLAGS) -c parser.cc

checker.o: checker.cc model.hh
	$(CC) $(CCFLAGS) -c checker.cc

.PHONY: clean

clean:
	rm -f *.o *~ dtec