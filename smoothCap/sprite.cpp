#include <iostream>
#include "sprite.h"


SDL_Texture* mySprite::getTexture(SDL_Renderer* rend, const std::string& filename) {
  try {
    SDL_Texture *texture = IMG_LoadTexture(rend, filename.c_str());
    if ( texture == NULL ) {
      throw std::string("Couldn't load ") + filename;
    }
    return texture;
  }
  catch( const std::string& msg ) { 
    std::cout << msg << std::endl; 
    std::terminate();
  }
}
inline float mySprite::clamp(const float val, const float lo, const float hi){
  return val <= lo ? lo : ( val >= hi ? hi : val);
}

void mySprite::update(mySprite& glasses, FrameGenerator& frameGen, bool makeVideo){
  static float x = glasses.X_POS;
  static float y = glasses.Y_POS;
  const unsigned int DT = 17u; // ***
  const float X_VEL = 0.0;
	const float Y_VEL = 105.0;

	//updateCircle();
	
  if(y > 160) return;
  static unsigned int remainder = 0u; // ***
  static unsigned int prevTicks = SDL_GetTicks();
  unsigned int currentTicks = SDL_GetTicks();
  unsigned int elapsedTicks = currentTicks - prevTicks + remainder; // ***

  //if( elapsedTicks < DT ) return;

  // Generate a frame:
  if ( makeVideo ) frameGen.makeFrame();

  float dx = X_VEL * DT * 0.001;
  x += dx;
  float dy = Y_VEL * DT * 0.001;
  y += dy;
  x = clamp(x,0.f,700-glasses.spriteRect.w);
  y = clamp(y,0.f,394-glasses.spriteRect.h);

  prevTicks = currentTicks;
  remainder = elapsedTicks - DT; // ***

  glasses.spriteRect.x = x;
  glasses.spriteRect.y = y;
}
