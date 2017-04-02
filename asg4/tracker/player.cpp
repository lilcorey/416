#include "player.h"
#include "gamedata.h"

Player::Player(const std::string& name) :
  TwoWaySprite(name), health(100.0), directionX(false),
  runSpeed(Gamedata::getInstance().getXmlInt(name+"/runSpeed")) {}

Player::Player(const Player& rhs) :
  TwoWaySprite(rhs), health(rhs.health), directionX(rhs.directionX), runSpeed(rhs.runSpeed){}

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
