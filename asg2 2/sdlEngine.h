
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "frameGenerator.h"
//SDL_RENDERER_PRESENTVSYNC
class SDLEngine{
public:
	SDLEngine(int posX, int posY, const std::string& filename) : 
		winPosX(posX),
		windPosY(posY),
		window(SDL_CreateWindow("My Animation", posX, posY, WIDTH, HEIGHT, 0)),
		renderer(SDL_CreateRenderer(window, -1, 0)),
		background(getTextureB(renderer, filename)) {}

	SDL_Window* getWindow() { return window; }
	SDL_Renderer* getRenderer() { return renderer; }
	SDL_Texture* getBackground() { return background; }
	SDL_Texture* getTextureB(SDL_Renderer* rend, const std::string& filename);
	
  void draw(SDL_Renderer* rend, SDL_Texture* back, SDL_Texture* star, SDL_Texture* circle,
          const SDL_Rect& dstrect, const SDL_Rect& circlerect, const SDL_Rect& otherrect);
	~SDLEngine() { SDL_DestroyTexture(background); SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); }
private:
	int winPosX;
	int windPosY;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *background;
	SDLEngine(const SDLEngine&);
	SDLEngine& operator=(const SDLEngine&);

	SDLEngine();
};
