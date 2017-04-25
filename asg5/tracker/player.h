#ifndef PLAYER_H
#define PLAYER_H
#include "twowaysprite.h"
#include "bulletPool.h"

class Player : public TwoWaySprite{
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  virtual bool collidedWith(const Drawable*) const;
  void shoot();

  void stop();
  void left();
  void right();
  void guyStand();
  float getHealth() const {return health;}
  void setHealth(const float h) { health = h;}
  //BulletPool* getBullets() const { return *bullets; }

protected:
  float health;
  bool directionX;
  int runSpeed;
  std::string bulletName;
  BulletPool bullets;
  const std::vector<Frame*>standFrames;
  void advanceFrame(Uint32);
};
#endif
