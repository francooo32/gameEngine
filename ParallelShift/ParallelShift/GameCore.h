#ifndef GAMECORE_H
#define GAMECORE_H

#include <iostream>
#include <ctime>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


class GameCore
{
	private: 

		//Variables
		sf::RenderWindow *window;
		sf::Event sfEvent;
		sf::Clock dtClock;
		     //Delta time
		float dt;

		//Initializations
		void initWindow();

public:
	GameCore();
	virtual ~GameCore();

	void updateDt();
	void updateSFMLEvents(); 
	void update();
	void render();
	void run();
};

#endif 