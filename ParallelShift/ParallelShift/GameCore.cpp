#include "GameCore.h"

//Initializers
void GameCore::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "C++ JRPG Game");
}

GameCore::GameCore() 
{
    this->initWindow();
}

GameCore::~GameCore()
{
    delete this->window;
}

void GameCore::updateDt()
{
    this->dt = this->dtClock.getElapsedTime().asSeconds();
}

void GameCore::updateSFMLEvents()
{
    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->sfEvent.type == sf::Event::Closed)
        {
            this->window->close();
        }
    }
}

void GameCore::update() 
{
    this->updateSFMLEvents();
}

void GameCore::render()
{
    this->window->clear();

    this->window->display();
}

void GameCore::run()
{
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}

