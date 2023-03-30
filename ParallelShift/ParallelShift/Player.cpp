#include "Player.h"

Player::Player(float x, float y, sf::Texture& texture_sheet)
{

	this->initVariables();
	
	this->setPosition(x, y);

	this->createHitboxComponent(this->sprite, 55.f, 60.f, 120.f, 130.f);
	this->createMovementComponent(300.f, 15.f, 5.f);
	this->createAnimationComponent(texture_sheet);

	//Configuration for sprite animation																								
	this->animationComponent->addAnimation("IDLE", 0.1f, 0, 0, 0, 0, 192, 192);
	this->animationComponent->addAnimation("WALK_LEFT", 1.f, 0, 2, 6, 2, 192, 192);
	this->animationComponent->addAnimation("WALK_RIGHT", 1.f, 0, 1, 6, 1, 192, 192);
	this->animationComponent->addAnimation("WALK_UP", 1.f, 0, 1, 6, 1, 192, 192);
	this->animationComponent->addAnimation("WALK_DOWN", 1.f, 0, 1, 6, 1, 192, 192);

}

Player::~Player()
{
}

void Player::update(const float& dt)
{
	this->movementComponent->update(dt);

	if (this->movementComponent->getState(IDLE)) 
	{
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		this->animationComponent->play("WALK_LEFT", dt);
	}
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		this->animationComponent->play("WALK_RIGHT", dt);
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("WALK_UP", dt);
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK_DOWN", dt);
	}

	this->hitboxComponent->update();
}

void Player::initVariables()
{
}

void Player::initComponents()
{
	
}
