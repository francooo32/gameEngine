#include "stdafx.h"
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
	//this->animationComponent->addAnimation("WALK_RIGHT", 1.f, 0, 1, 6, 1, 192, 192);
	this->animationComponent->addAnimation("WALK_UP", 1.f, 0, 2, 6, 2, 192, 192);
	this->animationComponent->addAnimation("WALK_DOWN", 1.f, 0, 3, 6, 3, 192, 192);
	this->animationComponent->addAnimation("ATTACK", 1.f, 0, 2, 6, 2, 192, 192);

}

Player::~Player()
{
}

void Player::updateAttack()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->attacking = true;
	}
}

void Player::updateAnimation(const float& dt)
{
	//Attack animation                     
	if (this->attacking)
	{
		//Set origin by direction (Use this code if you need to correct the position inside the hitbox)
		//if (this->sprite.getScale().x > 0.f) //Left
		//{
		//	this->sprite.setOrigin(96.f, 0.f);
		//}
		//else //Right
		//{
		//	this->sprite.setOrigin(258.f + 96.f, 0.f);
		//}
		if (this->animationComponent->play("ATTACK", dt, true))
		{
			this->attacking = false;
			//Set origin by direction (Use this code if you need to recolate the sprite after correcting the position inside the hitbox)
			//if (this->sprite.getScale().x > 0.f) //Left
			//{
			//	this->sprite.setOrigin(0.f, 0.f);
			//}
			//else //Right
			//{
			//	this->sprite.setOrigin(258.f, 0.f);
			//}
		}
	}

	if (this->movementComponent->getState(IDLE))
	{
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		//By setting the origin and scale you can flip the sprite and also you will set automatically the idle facing direction.
		if (this->sprite.getScale().x < 0.f)
		{
			this->sprite.setOrigin(0.f, 0.f);
			this->sprite.setScale(1.f, 1.f);
		}

		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		if (this->sprite.getScale().x > 0.f)
		{
			this->sprite.setOrigin(220.f, 0.f);
			this->sprite.setScale(-1.f, 1.f);
		}

		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}

}

void Player::update(const float& dt)
{
	this->movementComponent->update(dt);

	
	this->updateAttack();
	
	this->updateAnimation(dt);

	this->hitboxComponent->update();
}

void Player::initVariables()
{
	this->attacking = false;
}

void Player::initComponents()
{
	
}
