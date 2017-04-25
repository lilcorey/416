#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowaysprite.h"
#include "explodingSprite.h"
#include "shootingSprite.h"
#include "smartSprite.h"
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
  // for(auto& each : frontSprites){
  //   delete each;
  // }
  for (auto& each : sprites){
    delete each;
  }
  for (auto& each : backSprites){
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
  sprites(),
  backSprites(),
  frontSprites(),
  guy(new Player("guy")),
  currentSprite(-1),
  frameGen(),
  makeVideo( false ),
  hudToggle( false ),
  numSprites(Gamedata::getInstance().getXmlInt("ball/numSprites")),
  enemiesLeft(Gamedata::getInstance().getXmlInt("doge/numEnemy")),
  hud(guy),
  colStrategy(new RectangularCollisionStrategy )
{
  constexpr float u = 1.0f; //Mean size
  constexpr float d = 0.5f; //Std deviation

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);

  sprites.push_back(guy);
  for(unsigned int i =0; i<enemiesLeft;i++){
    sprites.push_back(new SmartSprite("doge", guy));
  }

  for ( unsigned int i = 0; i < numSprites/2; ++i ) {
    auto* s = new Sprite("ball");
    float scale = dist(mt);
    while(scale < 0.1f) scale = dist(mt);
    s->setScale(scale);
    s->setVelocityX( (s->getVelocityX())*(scale) );
    s->setVelocityY( (s->getVelocityY())*(scale) );
    backSprites.push_back(s);
  }
  std::vector<Drawable*>::iterator ptr = backSprites.begin();
  sort(ptr, backSprites.end(), SpriteLess());

  ptr = sprites.begin();
  ++ptr;
  while(ptr != sprites.end()){
    frontSprites.push_back(*ptr);
    ptr++;
  }
  std::cout << sprites.size() << '\n';
  std::cout << backSprites.size() << '\n';
  std::cout << frontSprites.size() << '\n';
  switchSprite();
  enemiesLeft = frontSprites.size();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  for ( auto* each : backSprites ) {
    each->draw();
  }
  grass.draw();
  for(auto* each : frontSprites){
    each->draw();
  }
  sprites[0]->draw();

  if(hudToggle || clock.getTicks() < 3850) hud.updateHUD();
  io.writeText(Gamedata::getInstance().getXmlStr("screenTitle"),
    10, viewport.getviewHeight()-35);
  io.writeText(std::to_string(enemiesLeft), 10, viewport.getviewHeight()-55);
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  for(auto* s : backSprites) s->update(ticks);
  for(auto* s : frontSprites) s->update(ticks);
  sky.update();
  grass.update();
  viewport.update(); // always update viewport last
  if( makeVideo) frameGen.makeFrame();
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}
void Engine::resetAI(){
  std::vector<Drawable*>::const_iterator it = sprites.begin() + 1;
  std::vector<Drawable*>::iterator it_f = frontSprites.begin();
  while(it_f != frontSprites.end()){
    it_f = frontSprites.erase(it_f);
    frontSprites.insert(it_f, *it);
    (*it)->setX(viewport.getworldWidth());
    it_f++;
    it++;
  }
  enemiesLeft = frontSprites.size();
}
void Engine::checkCollision(){
  std::vector<Drawable*>::const_iterator it = frontSprites.begin();
  Drawable* player = sprites[0];
  while(it != frontSprites.end()){
    if( colStrategy->execute(*player, **it) ){
      ExplodingSprite* p = dynamic_cast<ExplodingSprite*>(*it);
      if(!p){
        //const Sprite s(**it, (*it)->getFrame());
        //Drawable* boom = new ExplodingSprite(s);
        //it = frontSprites.erase(it);
        //Drawable* boom = new ExplodingSprite(*static_cast<SmartSprite*>(*it));
        //it = frontSprites.insert(it,boom);
        //enemiesLeft--;
        guy->setHealth(guy->getHealth()-1.0);
        if( enemiesLeft == 0) resetAI();
      }
    }
    if(guy->collidedWith(*it)){
      ExplodingSprite* p = dynamic_cast<ExplodingSprite*>(*it);
      if(!p){
        const Sprite s(**it, (*it)->getFrame());
        Drawable* boom = new ExplodingSprite(s);
        it = frontSprites.erase(it);
        it = frontSprites.insert(it,boom);
        enemiesLeft--;
        if( enemiesLeft == 0) resetAI();
      }

    }
    ++it;
  }
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
        guy->startJump();
      }
      if ( keystate[SDL_SCANCODE_SPACE]){
        guy->shoot();
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
      checkCollision();
      draw();
      update(ticks);

    }
  }
}
