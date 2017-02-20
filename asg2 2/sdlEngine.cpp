#include <iostream>
#include "sdlEngine.h"


void SDLEngine::draw(SDL_Renderer* rend, SDL_Texture* back, SDL_Texture* star, SDL_Texture* circle,
          const SDL_Rect& dstrect, const SDL_Rect& circlerect, const SDL_Rect& other) {
  SDL_RenderClear(rend);
  SDL_RenderCopy(rend, back, NULL, NULL);
  SDL_RenderCopy(rend, star, NULL, &dstrect);
  SDL_RenderCopy(rend, circle, NULL, &circlerect);
  SDL_RenderCopy(rend, circle, NULL, &other);
  SDL_RenderPresent(rend);
}

SDL_Texture* SDLEngine::getTextureB(SDL_Renderer* rend, const std::string& filename) {
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
