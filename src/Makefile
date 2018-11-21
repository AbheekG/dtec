CCFLAGS=-std=c++14
CC=g++

main: main.o model.o
	$(CC) -o main main.o model.o 

main.o: main.cc
	$(CC) $(CCFLAGS) -c main.cc

model.o: model.cc model.hh
	$(CC) $(CCFLAGS) -c model.cc

.PHONY: clean

clean:
	rm -f *.o *~ main


# CPPFLAGS=-std=c++14
# SDIR=src
# CC=g++

# main: main.o model.o
# 	$(CC) -o main main.o model.o 

# main.o: $(SDIR)/main.cc
# 	$(CC) $(CPPFLAGS) -c main.cc

# model.o: $(SDIR)/model.cc $(SDIR)/model.hh
# 	$(CC) $(CPPFLAGS) -c model.cc