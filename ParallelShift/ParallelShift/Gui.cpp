#include "stdafx.h"
#include "Gui.h"

gui::Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, unsigned character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::Color outlined_idle_color, sf::Color outlined_hover_color, sf::Color outlined_active_color,
	short unsigned id)
{
	this->buttonState = BTN_IDLE;
	this->id = id;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_color);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outlined_idle_color);

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2
	);

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;

	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;

	this->outlinedIdleColor = outlined_idle_color;
	this->outlinedHoverColor = outlined_hover_color;
	this->outlinedActiveColor = outlined_active_color;
	
}

gui::Button::~Button()
{

}

const bool gui::Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE)
		return true;

	return false;
}

const std::string gui::Button::getText() const
{
	return this->text.getString();
}

const short unsigned& gui::Button::getId() const
{
	return this->id;
}

void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}

void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}

void gui::Button::updatePressButton(const sf::Vector2f& mousePos)
{
	this->buttonState = BTN_IDLE; 
	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = BTN_HOVER;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE;
		}
	}

	switch (this->buttonState)
	{
	case BTN_IDLE :
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlinedIdleColor);
			break;
	case BTN_HOVER :
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlinedHoverColor);
		    break;
	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
		this->shape.setOutlineColor(this->outlinedActiveColor);
			break;
		default:
			this->shape.setFillColor(sf::Color::Red);
			this->text.setFillColor(sf::Color::Blue);
			this->shape.setOutlineColor(sf::Color::Green);
			break;
	}
}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);

}

//DropDown Class -------------------------------------------------------------------------------------------------------------

gui::DropDownList::DropDownList(float x, float y, float width, float height, 
	sf::Font& font, std::string list[], unsigned nrOfelements,unsigned default_index) :
		font(font), showList(false), keytimeMax(1.f), keytime(keytimeMax)
{
	//unsigned nrOfelements = sizeof(list) / sizeof(std::string)

	this->activeElement = new gui::Button(x, y, width, height,
		&this->font, list[default_index], 25,
		sf::Color(255, 255, 255, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0),
		sf::Color(255, 255, 255, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 50)
	);

	for (size_t i = 0; i < nrOfelements; i++)
	{
		this->list.push_back(
			new gui::Button(x, y + ((i + 1) * height), width, height,
				&this->font, list[i], 25,
				sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
				sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0),
				sf::Color(255, 255, 255, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0),
				i
			)
		);
	}

	
}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;

	for (size_t i = 0; i < this->list.size(); i++)
	{
		delete this->list[i];
	}
}

const unsigned short gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

const bool gui::DropDownList::getKeytime()
{
	if (this->keytime >= this->keytimeMax)
	{
		this->keytime = 0.f;
		return true;
	}
	return false;
}

void gui::DropDownList::updateKeytime(const float& dt)
{
	if (this->keytime < this->keytimeMax)
	{
		this->keytime += 10.f * dt;
	}
}

void gui::DropDownList::updatePressButton(const sf::Vector2f& mousePos, const float& dt)
{

	this->updateKeytime(dt);

	this->activeElement->updatePressButton(mousePos);

	if (this->activeElement->isPressed() && this->getKeytime())
	{
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->updatePressButton(mousePos);

			if (i->isPressed() && this->getKeytime())
			{
				this->showList = false;
				this->activeElement->setText(i->getText()); 
				this->activeElement->setId(i->getId());
			}
		}
	}

}

void gui::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->render(target);
		}
	}
}
