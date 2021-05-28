chi: Main.cpp Hash.o Digest.o Image.o
	g++ -std=c++20 -lgcrypt -o chi Main.cpp Hash.o Digest.o
	rm *.o

Hash.o: archive/Hash.cpp
	g++ -std=c++20 -lgcrypt -c archive/Hash.cpp

Digest.o: archive/Digest.cpp
	g++ -std=c++20 -c archive/Digest.cpp

Image.o:
	g++ -std=c++20 -c image/Image.cpp
