#include "stdafx.h"
#include "SettingsState.h"

SettingsState::SettingsState(StateData* state_data) :
										State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initfonts();
	this->initKeybinds();
	this->initGui();
	this->initText();
}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

	auto it2 = this->dropdownLists.begin();
	for (it2 = this->dropdownLists.begin(); it2 != this->dropdownLists.end(); ++it2)
	{
		delete it2->second;
	}
}

void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
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

void SettingsState::initfonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void SettingsState::initKeybinds()
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

void SettingsState::initGui()
{
	this->buttons["BACK"] = new gui::Button(1500.f, 880.f, 150.f, 50.f,
		&this->font, "Back", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0));

	this->buttons["APPLY"] = new gui::Button(1300.f, 880.f, 150.f, 50.f,
		&this->font, "Apply", 25,
		sf::Color(255, 255, 255, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200),
		sf::Color(25, 25, 25, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 0));

	std::vector<std::string> modes_str;
	//std::string li[] = { "1920x1080", "800x600", "640x480"};
	for (auto &i : this->modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}

	this->dropdownLists["Resolution"] = new gui::DropDownList(800, 450, 200, 50, font, modes_str.data(), modes_str.size());
}

void SettingsState::initText()
{
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(100.f, 100.f));
	this->optionsText.setCharacterSize(30);
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));

	this->optionsText.setString(
		"Resolution \n\nFullscreen \n\nVsync \n\nAntialiasing \n\n "
	);
}

void SettingsState::updateInput(const float& dt)
{

}

void SettingsState::updateGui(const float& dt)
{
	//Buttons start
	for (auto& it : this->buttons)
	{
		it.second->updatePressButton(this->mousePosWindow);
	}

	if (this->buttons["BACK"]->isPressed())
	{
		this->endState();
	}

	if (this->buttons["APPLY"]->isPressed())
	{
		//This need to be corrected, is just a test for implementation.
		this->stateData->gfxSettings->resolution = this->modes[dropdownLists["Resolution"]->getActiveElementId()];
		this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Default);
	}
	//Buttons ends

	//DropDownList start
	for (auto& it : this->dropdownLists)
	{
		it.second->updatePressButton(this->mousePosWindow, dt);
	}
	//DropDownList ends
}

void SettingsState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);

	this->updateGui(dt);

	//This is a tracker of the mouse pos for HUD desing purposes, you should comment of deleted when finish.
	system("cls");
	std::cout << this->mousePosView.x << " " << this->mousePosView.y << "\n";

}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}

	for (auto& it : this->dropdownLists)
	{
		it.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->renderGui(*target);

	target->draw(this->optionsText);

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