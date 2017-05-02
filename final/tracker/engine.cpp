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
#include "smartSprite.h"
#include "birdsprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

//http://more-sky.com/group/2d-wallpaper/
//http://carbon-fighters.wikia.com/wiki/User_blog:Ladsworld/Doge_Sprite_Sheet
Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;

  for (auto& each : sprites){
    delete each;
  }
  for (auto& each : backSprites){
    delete each;
  }
  delete guyExplode;
  delete colStrategy;
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
  guyExplode(),
  currentSprite(-1),
  frameGen(),
  makeVideo( false ),
  hudToggle( false ),
  godFlag(false),
  blowUptime( false ),
  numLives(3),
  numSprites(Gamedata::getInstance().getXmlInt("flap/numSprites")),
  enemiesLeft(Gamedata::getInstance().getXmlInt("doge/numEnemy")),
  numRounds(1),
  blowUpCounter(0),
  hud(guy),
  sound(),
  colStrategy(new RectangularCollisionStrategy )
{
  constexpr float u = 1.0f; //Mean size
  constexpr float d = 0.5f; //Std deviation

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);

  const Sprite s(*guy, (guy)->getFrame());
  guyExplode = new ExplodingSprite(s);

  sprites.push_back(guy);
  for(unsigned int i =0; i<enemiesLeft;i++){
    sprites.push_back(new SmartSprite("doge", guy));
  }

  for ( unsigned int i = 0; i < numSprites/2; ++i ) {
    auto* s = new BirdSprite("flap");
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
  if(guy->getHealth() > 0 && !blowUptime) sprites[0]->draw();
  else guyExplode->draw();

  if(hudToggle || clock.getTicks() < 3850) hud.updateHUD();
  io.writeText(Gamedata::getInstance().getXmlStr("screenTitle"),
    10, viewport.getviewHeight()-35);

  TTF_SetFontStyle(io.getFont(), TTF_STYLE_BOLD);//set font to bold
  if(godFlag) io.writeText("God Mode ON", 10, viewport.getviewHeight()-70);
  io.writeText("Doge(s): " + std::to_string(enemiesLeft)
   + "  Round: " + std::to_string(numRounds)
   + "  Lives: " + std::to_string(numLives),
   10, viewport.getviewHeight()-55);
  TTF_SetFontStyle(io.getFont(), TTF_STYLE_NORMAL);//return font to normal
  if(blowUptime){
    io.writeText("DEAD", (viewport.getviewWidth()/2)-10, viewport.getviewHeight()/2);
  }
  if(numLives == 0){
    io.writeText("OUT OF LIVES - press 'R' to start over",
     (viewport.getviewWidth()/2)-10, (viewport.getviewHeight()/2)+30);
    clock.pause();
  }
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  if(!blowUptime) for(auto* s : sprites) s->update(ticks);
  guyExplode->update(ticks);
  for(auto* s : backSprites) s->update(ticks);
  for(auto* s : frontSprites) s->update(ticks);
  sky.update();
  grass.update();
  viewport.update(); // always update viewport last

  if( makeVideo) frameGen.makeFrame();
  if(blowUptime) blowUpCounter++;
  if(blowUpCounter>200){
    blowUptime = false;
    blowUpCounter = 0;
  }
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}
void Engine::resetPlayer(){
  delete guyExplode;
  sound[2];
  const Sprite tmp(*guy, (guy)->getFrame());
  guyExplode = new ExplodingSprite(tmp);
  blowUptime = true;
  numLives--;
  guy->setY(320);
  guy->setHealth(100.0);
}
void Engine::resetAI(){
  std::vector<Drawable*>::const_iterator it = sprites.begin() + 1;
  std::vector<Drawable*>::iterator it_f = frontSprites.begin();
  while(it_f != frontSprites.end()){
    it_f = frontSprites.erase(it_f);
    frontSprites.insert(it_f, *it);

    if(rand()%2) (*it)->setX(0.0);//randomize respawn
    else (*it)->setX(viewport.getworldWidth());

    (*it)->setVelocityX((*it)->getVelocityX()*(1+(numRounds/10)));

    it_f++;
    it++;
  }
  enemiesLeft = frontSprites.size();
  numRounds++;
}
void Engine::checkCollision(){
  std::vector<Drawable*>::iterator it = frontSprites.begin();
  Drawable* player = sprites[0];
  while(it != frontSprites.end()){
    if( colStrategy->execute(*player, **it) ){
      ExplodingSprite* p = dynamic_cast<ExplodingSprite*>(*it);
      if(!p){
        if(!godFlag) guy->setHealth(guy->getHealth()-1.0);
        if( enemiesLeft == 0) resetAI();
        if(guy->getHealth() < 0) resetPlayer();
      }
    }
    if(guy->collidedWith(*it)){
      ExplodingSprite* p = dynamic_cast<ExplodingSprite*>(*it);
      if(!p){
        const Sprite s(**it, (*it)->getFrame());
        Drawable* boom = new ExplodingSprite(s);
        it = frontSprites.erase(it);
        it = frontSprites.insert(it,boom);
        sound[0];
        enemiesLeft--;
        if( enemiesLeft == 0) resetAI();
      }

    }
    ++it;
  }
}
void Engine::resetGame() {
  if ( clock.isPaused() ) clock.unpause();
  guy->setHealth(100.0);
  resetAI();
  numLives = 3;
  numRounds = 1;
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
        if(guy->getGround()) sound[1];
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
          //switchSprite();
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          godFlag = !godFlag;
        }
        if( keystate[SDL_SCANCODE_R] ){
          resetGame();
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
