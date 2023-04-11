#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

class AnimationComponent
{
private:
	class Animation
	{
	public :
		sf::Sprite& sprite;
		sf::Texture& textureSheet;
		float animationTimer;
		float timer;
		bool done;
		int width;
		int height;
		sf::IntRect startRect;
		sf::IntRect currentRect;
		sf::IntRect endRect;

		Animation(sf::Sprite& sprite, sf::Texture& texture_sheet, float animation_timer, 
			int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height)
			: sprite(sprite), textureSheet(texture_sheet), 
			animationTimer(animation_timer), timer(0.f), done(false), 
			width(width), height(height)
		{
			/*this->width = width;
			this->height = height;*/
			this->timer = 0.f;
			this->startRect = sf::IntRect(start_frame_x * width, start_frame_y * height, width, height);
			this->currentRect = this->startRect;
			this->endRect = sf::IntRect(frames_x * width, frames_y * height, width, height);
			//this-> startRect == this->endRect;

			this->sprite.setTexture(this->textureSheet, true);
			this->sprite.setTextureRect(this->startRect);
		}

		const bool& isDone() const
		{
			return this->done;
		}

		const bool& play(const float& dt)
		{
			//Actualiza el timer
			this->done = false;
			this->timer += 10.f * dt;
			if (this->timer >= this->animationTimer)
			{
				//Resetea el timer
				this->timer = 0.f;

				if (this->currentRect != this->endRect)
				{
					//this->currentRect.left *= this->width;
					this->currentRect.left += this->width;
				}
				else
				{
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}

				this->sprite.setTextureRect(this->currentRect);
			}
			return this->done;
		}

		const bool& play(const float& dt, float mod_percetage)
		{
			//Actualiza el timer
			if (mod_percetage < 0.5f)
				mod_percetage = 0.5f;

			this->done = false;

			this->timer += mod_percetage * 10.f * dt;
			if (this->timer >= this->animationTimer)
			{
				//Resetea el timer
				this->timer = 0.f;

				if (this->currentRect != this->endRect)
				{
					//this->currentRect.left *= this->width;
					this->currentRect.left += this->width;
				}
				else
				{
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}

				this->sprite.setTextureRect(this->currentRect);
			}
			return this->done;
		}

		void reset()
		{
			this->timer = this->animationTimer;
			this->currentRect = this->startRect;
		}
	};

	sf::Sprite& sprite;
	sf::Texture& textureSheet;
	std::map<std::string, Animation*> animations;
	Animation* lastAnimation;
	Animation* priorityAnimation;

public:
	AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimationComponent();

	const bool& isDone(std::string key);
	

	void addAnimation(const std::string key, float animation_timer,
		int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height);

	//Removed const "key", pay attention in case you need to define that as a const again.
	const bool& play(std::string key, const float& dt, const bool priority = false);
	const bool& play(std::string key, const float& dt, const float& modifier, const float& modifier_max, const bool priority = false);

};

#endif 