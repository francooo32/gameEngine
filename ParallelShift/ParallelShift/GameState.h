#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Player.h"

class GameState :
	public State
{
private:
	Player* player;

	void initKeybinds();
	void initTextures();
	void initPlayers();

public:
	GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~GameState();

	//functions
	void updateInput(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};


#endif 
