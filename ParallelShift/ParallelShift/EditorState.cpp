#include "stdafx.h"
#include "EditorState.h"

EditorState::EditorState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initfonts();
	this->initKeybinds();
	this->initPauseMenu();
	this->initButtons();
	this->initGui();
	this->initTileMap();
}

EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

	delete this->pmenu;

	delete this->tileMap;
}
 
void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime())
	{
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void EditorState::updateEditorInput(const float& dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime())
	{
		this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
	}
}

void EditorState::initVariables()
{
}

void EditorState::initBackground()
{
	
}

void EditorState::initfonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
	}
}

void EditorState::initKeybinds()
{
	std::ifstream ifs("Config/editorstate_keybinds.ini");

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

void EditorState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->font);

	this->pmenu->addButton("EXIT", 800.f, "Exit");
}

void EditorState::initButtons()
{
}

void EditorState::initGui()
{
	this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));
	this->selectorRect.setFillColor(sf::Color::Transparent);
	this->selectorRect.setOutlineThickness(1.f);
	this->selectorRect.setOutlineColor(sf::Color::Green);
}

void EditorState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 10, 10);
}

void EditorState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->updatePressButton(this->mousePosView);
	}
}

void EditorState::updateGui()
{
	this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
}

void EditorState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("EXIT"))
		this->endState();
}

void EditorState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateKeyTime(dt);
	this->updateInput(dt);

	if (!this->paused)
	{
		this->updateButtons();
		this->updateGui();
		this->updateEditorInput(dt);
	}
	else
	{
		this->pmenu->update(this->mousePosView);
		this->updatePauseMenuButtons();
	}

	system("cls");
	std::cout << this->mousePosView.x << " " << this->mousePosView.y << "\n";
}

void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::renderGui(sf::RenderTarget& target)
{
	target.draw(this->selectorRect);
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->renderButtons(*target);
	this->renderGui(*target);

	this->tileMap->render(*target);

	if (this->paused)
	{
		this->pmenu->render(*target);
	}

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
