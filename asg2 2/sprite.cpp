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

void mySprite::update(mySprite& glasses, Clock clock, float dt){
  static float x = glasses.X_POS;
  static float y = glasses.Y_POS;
  const float X_VEL = 0.0;
	const float Y_VEL = 105.0;
	
  if(y > 160) return;
  
	float dx = X_VEL * dt;
  x += dx;
  
	float dy = Y_VEL * dt;
  y += dy;
  x = clamp(x,0.f,700-glasses.spriteRect.w);
  y = clamp(y,0.f,394-glasses.spriteRect.h);


  glasses.spriteRect.x = x;
  glasses.spriteRect.y = y;
}
