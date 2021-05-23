chi: Main.cpp Hash.o
	g++ -std=c++20 -lgcrypt -o chi Main.cpp Hash.o
	rm *.o

Hash.o: archive/Hash.cpp
	g++ -std=c++20 -lgcrypt -c archive/Hash.cpp
