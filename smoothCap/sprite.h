#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <string>
#include "sdlEngine.h"

class mySprite{
public:
	mySprite(const SDL_Rect cord, const std::string& filepath, SDL_Renderer *r) : 
  	spriteRect(cord),
    spriteImg(getTexture(r, filepath)),
  	X_POS(spriteRect.x),
    Y_POS(spriteRect.y) {}
  mySprite(const mySprite& rhs) :
    spriteRect(rhs.spriteRect),
    spriteImg(rhs.spriteImg),
    X_POS(rhs.X_POS),
    Y_POS(rhs.Y_POS) {}
	const SDL_Rect getRect() const { return spriteRect; }
  void setRect(int x, int y) { spriteRect.x = x; spriteRect.y = y; }
  SDL_Texture* getImg() { return spriteImg; }
	void update(mySprite& glasses, FrameGenerator& frameGen, bool makeVideo);
	inline float clamp(const float val, const float lo, const float hi);
	SDL_Texture* getTexture(SDL_Renderer* rend, const std::string& filename);
	~mySprite(){ SDL_DestroyTexture(spriteImg); }

private:
	SDL_Rect spriteRect;
	SDL_Texture *spriteImg;
	int X_POS, Y_POS;
	mySprite();
};
