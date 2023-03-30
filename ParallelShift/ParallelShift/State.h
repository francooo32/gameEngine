#ifndef STATE_H
#define STATE_H

#include "Entity.h"

class State
{
private:

protected: 
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	std::map<std::string, sf::Texture> textures;
	bool quit;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	virtual void initKeybinds() = 0;

public:
		State(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
		virtual ~State();

		const bool& getQuit() const;
		void endState();

		virtual void updateMousePosition();
		virtual void updateInput(const float& dt) = 0;
		virtual void update(const float& dt) = 0;
		virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

#endif