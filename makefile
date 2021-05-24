chi: Main.cpp Hash.o Record.o
	g++ -std=c++20 -lgcrypt -o chi Main.cpp Hash.o Record.o
	rm *.o

Hash.o: archive/Hash.cpp
	g++ -std=c++20 -lgcrypt -c archive/Hash.cpp

Record.o: archive/Record.cpp
	g++ -std=c++20 -c archive/Record.cpp
