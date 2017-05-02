#include "player.h"
#include "gamedata.h"
#include "renderContext.h"
Player::Player(const std::string& name) :
  TwoWaySprite(name), health(100.0), directionX(false),
  runSpeed(Gamedata::getInstance().getXmlInt(name+"/runSpeed")),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bullets(bulletName),
  standFrames(RenderContext::getInstance()->getFrames("guyStand")) {}

Player::Player(const Player& rhs) :
  TwoWaySprite(rhs), health(rhs.health), directionX(rhs.directionX), runSpeed(rhs.runSpeed),
  bulletName(rhs.bulletName), bullets(rhs.bullets),
  standFrames(rhs.standFrames){}

void Player::draw() const{
  if(getVelocityX() == 0){
    if(direction)
      standFrames[2]->draw(getX(), getY());
    else
    standFrames[1]->draw(getX(), getY());
  }
  else
    MultiSprite::draw();
  bullets.draw();
}
bool Player::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}
void Player::advanceFrame(Uint32 ticks){
	timeSinceLastFrame += ticks;
	if(timeSinceLastFrame > frameInterval){
		if(direction == 0){
      if(getVelocityX() == 0) currentFrame = currentFrame;
      else{
        currentFrame = (currentFrame+1) % (numberOfFrames/2);
      }
		}
		else{
      if(getVelocityX() == 0) currentFrame = currentFrame;
      else{
			  currentFrame = ((currentFrame-1) % (numberOfFrames/2) )+ 8;
      }
		}
		timeSinceLastFrame = 0;
	}
}
void Player::update(Uint32 ticks){
  advanceFrame(ticks);
	//accelerate faster on falling
	if(getVelocityY() > 0) setVelocityY(getVelocityY() + gravity*1.5);
	else setVelocityY(getVelocityY() + gravity);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-(frameHeight*2)) {
    setVelocityY(0);
		setY(worldHeight-(frameHeight*2));
		onGround = true;
  }

  if ( getX() < 0) {
    setX(0);
  }
  if ( getX() > worldWidth-frameWidth) {
    setX(worldWidth-frameWidth);
  }

  directionX=false;

  bullets.update(ticks);
}

void Player::stop(){
  if(!directionX) setVelocityX(0);
  guyStand();
}
void Player::left(){
  directionX = true;
  dirLeft();
  setVelocityX( -runSpeed );
}
void Player::right(){
  directionX = true;
  dirRight();
  setVelocityX( runSpeed );
}
void Player::guyStand(){
//  frames = guyStanding;
}
void Player::shoot() {
  float x = getX()+getFrame()->getWidth();
  float y = getY()+getFrame()->getHeight()/2;
  // I'm not adding minSpeed to y velocity:
  if(getVelocityX() == 0){
    if(direction){
      bullets.shoot( Vector2f(x-getFrame()->getWidth(), y),
        Vector2f(-runSpeed, 0)
      );
    }
    else{
      bullets.shoot( Vector2f(x, y),
        Vector2f(runSpeed, 0)
      );
    }
  }
  else{
    if(direction){
      bullets.shoot( Vector2f(x-getFrame()->getWidth(), y),
        Vector2f(2*getVelocityX(), 0)
      );
    }
    else{
      bullets.shoot( Vector2f(x, y),
        Vector2f(2*getVelocityX(), 0)
      );
    }
  }

}
