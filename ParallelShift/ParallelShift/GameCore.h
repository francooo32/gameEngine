#ifndef GAMECORE_H
#define GAMECORE_H

#include "MainMenuState.h"

class GameCore
{
	private: 

		//Variables
		GraphicsSettings gfxSettings;
		StateData stateData;
		sf::RenderWindow *window;
		sf::Event sfEvent;
		sf::Clock dtClock;
		//Delta time
		float dt; 

		std::stack<State*> states;

		std::map<std::string, int> supportedKeys;

		float gridSize;

		//Initializations
		void initVariables();
		void initGraphicsSettings();
		void initStateData();
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