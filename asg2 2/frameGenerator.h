
#include <string>
#include <SDL.h>

const int WIDTH  = 700;
const int HEIGHT = 394;
const std::string USERNAME = "cedelma";
const int unsigned MAX_FRAMES = 300;


class FrameGenerator {
public:
  FrameGenerator(SDL_Renderer* const r, SDL_Window* const win) : 
    rend(r), window(win),
    frameCount(0) 
  {}
  void makeFrame();
  unsigned int getFrameCount() const { return frameCount; }
private:
  SDL_Renderer* const rend;
  SDL_Window* const window;
  unsigned int frameCount;
  FrameGenerator(const FrameGenerator&);
  FrameGenerator& operator=(const FrameGenerator&);
};
