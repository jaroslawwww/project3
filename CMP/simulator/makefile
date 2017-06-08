CC = g++ -std=c++11

CMP: main.o allinstruction.o memory.o function.o regfile.o
	g++ -o CMP main.o allinstruction.o memory.o function.o regfile.o
main.o: main.cpp allinstruction.h memory.h function.h regfile.h
	g++ -c -g main.cpp
allinstruction.o: allinstruction.cpp memory.h function.h regfile.h
	g++ -c -g allinstruction.cpp
memory.o: memory.cpp function.h regfile.h
	g++ -c -g memory.cpp
function.o: function.cpp	
	g++ -c -g function.cpp
regfile.o: regfile.cpp
	g++ -c -g regfile.cpp
clean:
	rm -f main.o allinstruction.o memory.o function.o regfile.o
