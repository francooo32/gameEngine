#include "GameCore.h"
#include "MainMenuState.h"

GameCore::GameCore()
{
    this->initWindow();
    this->initKeys();
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
    this->fullscreen = false;
    this->dt = 0.f;
}

//Initializers
void GameCore::initWindow()
{

    std::ifstream ifs("Config/window.ini");
    this->videoModes = sf::VideoMode::getFullscreenModes();

    std::string title = "None";
    sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();

    //window_bounds.bitsPerPixel = 32;

    bool fullScreen = false;
    unsigned framerate_limit = 120;
    bool vertical_sync_enable = false;
    unsigned antialiasing_level = 0;

    if (ifs.is_open())
    {
        std::getline(ifs, title);
        ifs >> window_bounds.width >> window_bounds.height;
        ifs >> fullScreen;
        ifs >> framerate_limit;
        ifs >> vertical_sync_enable;
        ifs >> antialiasing_level;
    }

    ifs.close();

    this->fullscreen = fullScreen;
    this->windowSettings.antialiasingLevel = antialiasing_level;
    if (this->fullscreen) 
    {
        this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Fullscreen , windowSettings);
    }
    else
    {
        this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close, windowSettings);
    }
	this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Default, windowSettings);
    this->window->setFramerateLimit(framerate_limit);
    this->window->setVerticalSyncEnabled(vertical_sync_enable);

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
    this->states.push(new MainMenuState(this->window, &this->supportedKeys, &this->states));
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
    if (!this->states.empty())
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

