#include "stdafx.h"
#include "GameState.h"

GameState::GameState(StateData* state_data)
	: State(state_data)
{
	this->initKeybinds();
	this->initFont();
	this->initTextures();
	this->initPauseMenu();
	this->initPlayers();
	this->initTileMap();
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->player;
	delete this->tileMap;
}

void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key_reference = "";
		while (ifs >> key >> key_reference)
		{
			this->keybinds[key] = this->supportedKeys->at(key_reference);
		}
	}

	ifs.close();

}

void GameState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void GameState::initTextures()
{

	if(!this->textures["PLAYER_SHEET"].loadFromFile("Resources/Images/Sprites/Player/PlayerSheet.png"))
	{
		throw "The character wans´t loaded";
	}
}

void GameState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->font);

	this->pmenu->addButton("EXIT", 800.f, "Exit");
}

void GameState::initPlayers()
{
	this->player = new Player(0, 0, this->textures["PLAYER_SHEET"]);
}

void GameState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 10, 10, "Resources/Images/Tiles/Grass100px.png");
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime() )
	{
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void GameState::updatePlayerInput(const float& dt)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
		this->player->move(-1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
		this->player->move(1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
		this->player->move(0.f, -1.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
		this->player->move(0.f, 1.f, dt);
}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("EXIT"))
		this->endState();
}

void GameState::update(const float& dt)
{

	this->updateMousePosition();
	this->updateKeyTime(dt);
	this->updateInput(dt);

	if (!this->paused) {
		this->player->update(dt);
		this->updatePlayerInput(dt);
	}
	else
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	//this->map.render(*target);

	this->player->render(*target);

	if (this->paused)
	{
		this->pmenu->render(*target);
	}
}