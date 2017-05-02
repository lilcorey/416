#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "frameGenerator.h"
#include "player.h"
#include "sound.h"
#include "hud.h"
#include "collisionStrategy.h"

class Engine {
public:
  Engine ();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine ();
  void resetGame();
  void play();
  void switchSprite();
  void checkCollision();

  void resetPlayer();
  void resetAI();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World grass;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Drawable*> backSprites;
  std::vector<Drawable*> frontSprites;
  Player* guy;
  Drawable* guyExplode;

  int currentSprite;
  FrameGenerator frameGen;
  bool makeVideo;
  bool hudToggle;
  bool godFlag;
  bool blowUptime;
  int numLives;
  unsigned int numSprites;
  unsigned int enemiesLeft;
  unsigned int numRounds;
  unsigned int blowUpCounter;

  HUD hud;
  SDLSound sound;
  CollisionStrategy* colStrategy;

  void draw() const;
  void update(Uint32);

};
