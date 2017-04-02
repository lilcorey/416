#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowaysprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};
//#include "hud.h"
//#include "frameGenerator.h"
//http://more-sky.com/group/2d-wallpaper/
//http://carbon-fighters.wikia.com/wiki/User_blog:Ladsworld/Doge_Sprite_Sheet
Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  for (auto& each : sprites){
    delete each;
  }
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  grass("grass", Gamedata::getInstance().getXmlInt("grass/factor") ),
  viewport( Viewport::getInstance() ),
  hud(),
  sprites(),
  guy(new Player("guy")),
  currentSprite(-1),
  frameGen(),
  makeVideo( false ),
  hudToggle( false ),
  numSprites(Gamedata::getInstance().getXmlInt("ball/numSprites"))
{
  constexpr float u = 1.0f; //Mean size
  constexpr float d = 0.5f; //Std deviation

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);

  sprites.push_back(guy);
  sprites.push_back( new TwoWaySprite("doge"));

  for ( unsigned int i = 0; i < numSprites; ++i ) {
    auto* s = new Sprite("ball");
    float scale = dist(mt);
    while(scale < 0.1f) scale = dist(mt);
    s->setScale(scale);
    s->setVelocityX( (s->getVelocityX())*(scale) );
    s->setVelocityY( (s->getVelocityY())*(scale) );
    sprites.push_back(s);
  }
  std::vector<Drawable*>::iterator ptr = sprites.begin();
  ++ptr;
  ++ptr;
  sort(ptr, sprites.end(), SpriteLess());

  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  for ( unsigned int i = 2; i < numSprites/2; ++i ) {
    sprites[i]->draw();
  }
  grass.draw();

  for ( unsigned int i = numSprites/2; i < numSprites; ++i ) {
    sprites[i]->draw();
  }
  //for(auto* s : sprites) s->draw();
  sprites[0]->draw();
  sprites[1]->draw();
  if(hudToggle || clock.getTicks() < 3850) hud.updateHUD();

  io.writeText(Gamedata::getInstance().getXmlStr("screenTitle"),
    10, viewport.getviewHeight()-35);

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  sky.update();
  grass.update();
  viewport.update(); // always update viewport last
  if( makeVideo) frameGen.makeFrame();
}

void Engine::switchSprite(){
  currentSprite = ++currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }

      if ( keystate[SDL_SCANCODE_W]){
        std::cout << "DOWN" << '\n';
        guy->startJump();
      }
      if( (keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D]) ||
          (!keystate[SDL_SCANCODE_A] && !keystate[SDL_SCANCODE_D]) ){
        guy->stop();
      }
      else if(keystate[SDL_SCANCODE_A]){
        guy->left();
      }
      else if(keystate[SDL_SCANCODE_D]){
        guy->right();
      }

      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if( keystate[SDL_SCANCODE_F1] ){
          hudToggle = !hudToggle;
        }

      }

    }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
    }
  }
}
