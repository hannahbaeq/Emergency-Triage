CXX = g++
CXXFLAGS = -Wall

proj3: pqueue.o mytest.cpp
        $(CXX) $(CXXFLAGS) pqueue.o mytest.cpp -o proj3

pqueue.o: pqueue.cpp pqueue.h
        $(CXX) $(CXXFLAGS) -c pqueue.cpp

clean:
        rm *.o*
        rm *~

val: valgrind ./proj3
