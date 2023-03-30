#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State( window, supportedKeys, states)
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

void MainMenuState::updateInput(const float& dt)
{

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
	this->buttons["NEW_GAME"] = new Button(300, 480, 150, 50,
		&this->font, "New Game", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0));

	this->buttons["SETTINGS"] = new Button(300, 580, 150, 50,
		&this->font, "Settings", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0));

	this->buttons["EDITOR_STATE"] = new Button(300, 680, 150, 50,
		&this->font, "Editor", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0));

	this->buttons["EXIT_GAME"] = new Button(300, 880, 150, 50,
		&this->font, "Quit Game", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0));
}

void MainMenuState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->updatePressButton(this->mousePosView);
	}

	if (this->buttons["NEW_GAME"]->isPressed())
	{
		this->states->push(new GameState(this->window, this->supportedKeys, this->states));
	}

	if (this->buttons["EDITOR_STATE"]->isPressed())
	{
		this->states->push(new EditorState(this->window, this->supportedKeys, this->states));
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