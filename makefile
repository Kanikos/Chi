chi: Main.cpp Hash.o Digest.o Image.o PNG.o JPEG.o
	g++ -std=c++20 -lgcrypt -lpng -ljpeg -o chi Main.cpp Hash.o Digest.o Image.o PNG.o JPEG.o
	rm *.o

Hash.o: archive/Hash.cpp
	g++ -std=c++20 -lgcrypt -c archive/Hash.cpp

Digest.o: archive/Digest.cpp
	g++ -std=c++20 -c archive/Digest.cpp

Image.o: image/Image.cpp
	g++ -std=c++20 -c image/Image.cpp

PNG.o: image/PNG.cpp Image.o
	g++ -std=c++20 -lpng -c image/PNG.cpp

JPEG.o: image/JPEG.cpp
	g++ -std=c++20 -ljpeg -c image/JPEG.cpp
