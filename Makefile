CPP = g++
CFLAGS = -O3 -std=c++17 #-Wall

default: conj-prior


OPENCV = `pkg-config opencv --cflags --libs`
LIBS = #$(OPENCV)

OBJECTFILES=main.o

model: $(OBJECTFILES)
	$(CPP) $(LFLAGS) $(OBJECTFILES) -o $@ $(LIBS)

$(OBJECTFILES):
	$(CPP) $(CFLAGS) $*.cpp -c

main.o: main.cpp metropolis-hastings.h

conj-prior: conjugate-prior.o
	$(CPP) $(LFLAGS) conjugate-prior.o -o $@

conjugate-prior.o:
	$(CPP) $(CFLAGS) $*.cpp -c

#clearing files
clean:
	@echo Cleaning object files...
	rm *.o
