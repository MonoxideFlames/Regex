CC=clang++
CFLAGS=-O2 -std=c++17

build: main.o RegexToNFA.o RegexScanner.o ShuntingYard.o NFA.o
	$(CC) $(CFLAGS) -o test $^

debug: main.cpp RegexToNFA.hpp RegexToNFA.cpp RegexScanner.hpp RegexScanner.cpp ShuntingYard.hpp ShuntingYard.cpp NFA.hpp NFA.cpp
	$(CC) -g -std=c++17  $^

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.gch *.o test

wipe:
	rm *.gch a.out
	rm -r a.out.dSYM

finalize:
	rm *.gch *.o
