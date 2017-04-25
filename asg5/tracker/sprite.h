#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const Sprite&);
  Sprite(const Drawable& d, const Frame* f);
  virtual ~Sprite() { }
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void setFrame(const Frame* f) { frame = f; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  inline float getScale()const{ return scale; }
  void  setScale(float s){ scale = s; }

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  float scale;
  int getDistance(const Sprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
