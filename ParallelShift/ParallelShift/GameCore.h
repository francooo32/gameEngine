#ifndef GAMECORE_H
#define GAMECORE_H

#include "MainMenuState.h"

class GameCore
{
	private: 

		//Variables
		sf::RenderWindow *window;
		sf::Event sfEvent;
		sf::Clock dtClock;
		     //Delta time
		float dt; 

		std::vector<sf::VideoMode> videoModes;
		sf::ContextSettings windowSettings;
		bool fullscreen;

		std::stack<State*> states;

		std::map<std::string, int> supportedKeys;

		//Initializations
		void initVariables();
		void initWindow();
		void initKeys();
		void initState();
		

public:
	GameCore();
	virtual ~GameCore();

	void endApp();
	void updateDt();
	void updateSFMLEvents(); 
	void update();
	void render();
	void run();
};

#endif 