#ifndef BIRDSPRITE__H
#define BIRDSPRITE__H
#include <string>
#include "sprite.h"

class BirdSprite : public Sprite{
public:
  BirdSprite(const std::string& n) : Sprite(n) , goingLeft(false){}
  BirdSprite(const Sprite& s) : Sprite(s) , goingLeft(false){}
  virtual ~BirdSprite() { }

  virtual void draw() const;
  virtual void update(Uint32 ticks);

private:
  bool goingLeft;
};

#endif
