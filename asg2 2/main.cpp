// To compile and link, need to add link flag in Makefile: -lSDL2_image
// Also need to install libsdl2-image-dev

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "circle.h"
#include "clock.h"

int main( ) {
  SDL_Init(SDL_INIT_VIDEO);
	Clock myClock;
  SDLEngine engine(100, 100, "images/background.png");

  SDL_Rect rect = {145, 0, 66, 16};
  SDL_Rect circ = {0, 0, 30, 30};

  mySprite myGlasses(rect, "images/glass.png", engine.getRenderer());

  mySprite myCircle(circ, "images/yellowstar.png", engine.getRenderer());
  Circle starCircle(myCircle, 70);
  Circle otherCircle(myCircle, 100);

  SDL_Event event;
  const Uint8* keystate;
  int nKeys=-1;
  bool makeVideo = true;
  bool done = false;
	unsigned int frameCount = 0;
  FrameGenerator frameGen(engine.getRenderer(), engine.getWindow());

  while ( !done ) {
    while ( SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        done = true;
      }
      keystate = SDL_GetKeyboardState(&nKeys);
      if (keystate[SDL_SCANCODE_ESCAPE]) { 
        done = true; 
      }
      if (keystate[SDL_SCANCODE_F4]) { 
        makeVideo = true;
      }
    }


		if(myClock.calcClock() == 0){
		  myGlasses.update(myGlasses, myClock, myClock.getDt());
		  starCircle.updateCircle(myClock);
		  otherCircle.updateCircle(myClock);
			engine.draw(engine.getRenderer(), engine.getBackground(), myGlasses.getImg(), starCircle.getImg(), 				
									myGlasses.getRect(), starCircle.getRect(), otherCircle.getRect());
			if ( makeVideo ) frameGen.makeFrame();
			myClock.updatePrev();
			frameCount++;

		}
	
	
  }//end while !done
	unsigned int totalTicks = SDL_GetTicks() - myClock.getstartTime();
	std::cout << "fps: " << frameCount/(totalTicks*0.001) << std::endl;	
	
  SDL_Quit();
}
