#include "stdafx.h"
#include "GameCore.h"
#include "MainMenuState.h"

GameCore::GameCore()
{
    this->initVariables();
    this->initGraphicsSettings();
    this->initWindow();
    this->initKeys();
    this->initStateData();
    this->initState();
}

GameCore::~GameCore()
{
    delete this->window;

    while (!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }

}

void GameCore::initVariables()
{
    this->window = NULL;
    this->dt = 0.f;
    this->gridSize = 50.f;
}

void GameCore::initGraphicsSettings()
{
    this->gfxSettings.loadFromFile("Config/graphics.ini");
}

void GameCore::initStateData()
{
    this->stateData.window = this->window;
    this->stateData.gfxSettings = &this->gfxSettings;
    this->stateData.supportedKeys = &this->supportedKeys;
    this->stateData.states = &this->states;
    this->stateData.gridSize = this->gridSize;
}

//Initializers
void GameCore::initWindow()
{
    if (this->gfxSettings.fullScreen) 
    {
        this->window = new sf::RenderWindow(this->gfxSettings.resolution, 
                                                this->gfxSettings.title, 
                                                    sf::Style::Fullscreen , 
                                                        this->gfxSettings.contextSettings);
    }
    else
    {
        this->window = new sf::RenderWindow(this->gfxSettings.resolution,
                                                    this->gfxSettings.title,
                                                        sf::Style::Titlebar | sf::Style::Close, 
                                                            this->gfxSettings.contextSettings);
    }
    this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
    this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);

}

void GameCore::initKeys()
{

    std::ifstream ifs("Config/supported_keys.ini");

    if (ifs.is_open())
    {
        std::string key = "";
        int key_value = 0;
        while (ifs >> key >> key_value)
        {
            this->supportedKeys[key] = key_value;
        }
    }

    ifs.close();

}

void GameCore::initState()
{
    this->states.push(new MainMenuState(&this->stateData));
}



void GameCore::endApp()
{
    std::cout << "end app" << "\n";
}

void GameCore::updateDt()
{
    this->dt = this->dtClock.restart().asSeconds();
     
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

    //Update items
    if (!this->states.empty() && this->window->hasFocus())
    {
        this->states.top()->update(this->dt);
        if (this->states.top()->getQuit())
        {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }
    else 
    {
        this->endApp();
        this->window->close();
    }
}

void GameCore::render()
{
    this->window->clear();

    //Render items
    if (!this->states.empty())
        this->states.top()->render(this->window);

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

