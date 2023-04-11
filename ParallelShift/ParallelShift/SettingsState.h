#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H

#include "State.h"
#include "Gui.h"

class SettingsState :
    public State
{

private:
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, gui::DropDownList*> dropdownLists;

	sf::Text optionsText;
	std::vector<sf::VideoMode> modes;

	void initVariables();
	void initBackground();
	void initfonts();
	void initKeybinds();
	void initGui();
	void initText();

public :
    SettingsState(StateData* state_data);
    virtual ~SettingsState();

	void updateInput(const float& dt);
	void updateGui(const float& dt);
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

#endif

