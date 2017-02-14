// To compile and link, need to add link flag in Makefile: -lSDL2_image
// Also need to install libsdl2-image-dev

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
//#include "sdlEngine.h"
//#include "sprite.h"
#include "circle.h"

int main( ) {
  SDL_Init(SDL_INIT_VIDEO);

  SDLEngine engine(100, 100, "images/background.png");

  SDL_Rect rect = {145, 0, 66, 16};
  mySprite myGlasses(rect, "images/glass.png", engine.getRenderer());
  SDL_Rect circ = {0, 0, 30, 30};
  mySprite myCircle(circ, "images/yellowstar.png", engine.getRenderer());
  //mySprite otherC(circ, "images/yellowstar.png", engine.getRenderer());
  Circle starCircle(myCircle, 80);
  Circle otherCircle(myCircle, 120);


  SDL_Event event;
  const Uint8* keystate;
  int nKeys=-1;
  bool makeVideo = false;
  bool done = false;
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

    engine.draw(engine.getRenderer(), engine.getBackground(), myGlasses.getImg(), starCircle.getImg(), myGlasses.getRect(), starCircle.getRect(), otherCircle.getRect());
   // engine.draw(engine.getRenderer(), engine.getBackground(), myGlasses.getImg(), starCircle.getImg(), myGlasses.getRect(), v);
    myGlasses.update(myGlasses, frameGen, makeVideo);
    starCircle.updateCircle();
    otherCircle.updateCircle();
   // circle3.updateCircle();
  }

  SDL_Quit();
}
