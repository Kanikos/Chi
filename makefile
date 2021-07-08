chi: Main.cpp Hash.o Digest.o Image.o
	g++ -std=c++20 -lgcrypt -lpng -ljpeg -o chi Main.cpp Hash.o Digest.o Image.o
	rm *.o

Hash.o: archive/Hash.cpp
	g++ -std=c++20 -lgcrypt -c archive/Hash.cpp

Digest.o: archive/Digest.cpp
	g++ -std=c++20 -c archive/Digest.cpp

Image.o: Image.cpp
	g++ -std=c++20 -lpng -ljpeg -c Image.cpp
