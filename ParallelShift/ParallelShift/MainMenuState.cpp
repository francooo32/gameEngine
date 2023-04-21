#include "stdafx.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateData* state_data)
								: State(state_data)
{	
	//Make this local string cause the constructor was having problem trying to recibe the direct string reference
	//std::string gametext = "New Game";

	this->initVariables();
	this->initBackground();
	this->initfonts();
	this->initKeybinds();
	this->initButtons();

	/*this->background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->background.setFillColor(sf::Color::Magenta);*/
}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void MainMenuState::initVariables()
{
}

void MainMenuState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);

	if (!this->backgroundTexture.loadFromFile("Resources/Images/Background/landascape1920x1080.png"))
	{
		throw"Failed loading background on main menu";
	}

	this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initfonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenu_keybinds.ini");

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

void MainMenuState::initButtons()
{
	this->buttons["NEW_GAME"] = new gui::Button(300.f, 480.f, 150.f, 40.f,
		&this->font, "New Game", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 220), sf::Color(150, 150, 150, 220), sf::Color(20, 20, 20, 220));

	this->buttons["SETTINGS"] = new gui::Button(300.f, 580.f, 150.f, 40.f,
		&this->font, "Settings", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 220), sf::Color(150, 150, 150, 220), sf::Color(20, 20, 20, 220));

	this->buttons["EDITOR"] = new gui::Button(300.f, 680.f, 150.f, 40.f,
		&this->font, "Editor", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 220), sf::Color(150, 150, 150, 220), sf::Color(20, 20, 20, 220));

	this->buttons["EXIT_GAME"] = new gui::Button(300.f, 880.f, 150.f, 40.f,
		&this->font, "Quit Game", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 220), sf::Color(150, 150, 150, 220), sf::Color(20, 20, 20, 220));
}

void MainMenuState::updateInput(const float& dt)
{

}

void MainMenuState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->updatePressButton(this->mousePosWindow);
	}

	if (this->buttons["NEW_GAME"]->isPressed())
	{
		this->states->push(new GameState(this->stateData));
	}

	if (this->buttons["SETTINGS"]->isPressed())
	{
		this->states->push(new SettingsState(this->stateData));
	}

	if (this->buttons["EDITOR"]->isPressed())
	{
		this->states->push(new EditorState(this->stateData));
	}

	if (this->buttons["EXIT_GAME"]->isPressed())
	{
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);

	this->updateButtons();

	system("cls");
	std::cout << this->mousePosView.x << " " << this->mousePosView.y << "\n";
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);
	this->renderButtons(*target);

	//Method to get position on mouse so you can see where you want to put the menu elements.
	/*sf::Text mouseText;
	mouseText.setPosition(this->mousePosView);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText);*/
}