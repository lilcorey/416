#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "frameGenerator.h"
#include "player.h"
#include "hud.h"

class Engine {
public:
  Engine ();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World grass;
  Viewport& viewport;
  HUD hud;

  std::vector<Drawable*> sprites;
  Player* guy;

  int currentSprite;
  FrameGenerator frameGen;
  bool makeVideo;
  bool hudToggle;
  const unsigned int numSprites;

  void draw() const;
  void update(Uint32);

};
