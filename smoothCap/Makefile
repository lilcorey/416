# Change the following values to suit your system.

CFLAGS=`sdl2-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/
SDL_LIB=`sdl2-config --libs` -lSDL2_image
CCC=g++

OBJECTS = frameGenerator.o circle.o  sprite.o sdlEngine.o

run: main.o $(OBJECTS)
	$(CCC) $(CFLAGS) main.cpp -o run $(OBJECTS) $(SDL_LIB)

main.o: main.cpp frameGenerator.h circle.h sprite.h sdlEngine.h
	$(CCC) $(CFLAGS) -c main.cpp

frameGenerator.o: frameGenerator.cpp frameGenerator.h
	$(CCC) $(CFLAGS) -c frameGenerator.cpp
circle.o: circle.cpp circle.h
	$(CCC) $(CFLAGS) -c circle.cpp

sprite.o: sprite.cpp sprite.h
	$(CCC) $(CFLAGS) -c sprite.cpp

sdlEngine.o: sdlEngine.cpp sdlEngine.h
	$(CCC) $(CFLAGS) -c sdlEngine.cpp

clean:
	rm -f run *.o
	rm -f *~
	rm -f frames/*.bmp
