#include "stdafx.h"
#include "TileMap.h"



TileMap::TileMap(float gridSize, int width, int height, std::string texture_file)
{
	//Carefull with the size, if you run a large size tile map, lets say 3000 x and y, you gonna have problems if you are in debug mode, use
	//release mode in that case cause is hard to allocate all that memory in debug.
	this->gridSizeF = gridSize;
	this->gridSizeI = static_cast<int>(this->gridSizeF);
	this->maxSizeWorldGrid.x = width;
	this->maxSizeWorldGrid.y = height;
	this->maxSizeWorldF.x = static_cast<float>(width) * gridSize;
	this->maxSizeWorldF.y = static_cast<float>(height) * gridSize;
	this->layers = 1;
	this->textureFile = texture_file;

	this->fromX = 0;
	this->toX = 0;
	this->fromY = 0;
	this->toY = 0;
	this->layer = 0;
	
	this->map.resize(this->maxSizeWorldGrid.x, std::vector< std::vector< std::vector<Tile*> > >());
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
	{
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector< std::vector<Tile*> >());
					for (int z = 0; z < this->layers; z++)
					{
						this->map[x][y].resize(this->layers, std::vector<Tile*>());
					}
			}
	}
	
	if (!this->tileSheet.loadFromFile(texture_file))
	{
		std::cout << "Failed to load the texture sheet named:" << texture_file << "\n";
	}

	//Just for debbugin porpuses
	this->collisionBox.setSize(sf::Vector2f(gridSize, gridSize));
	this->collisionBox.setFillColor(sf::Color(255, 0, 0, 50));
	this->collisionBox.setOutlineColor(sf::Color::Red);
	this->collisionBox.setOutlineThickness(1.f);
}

TileMap::~TileMap()
{
	this->clearMemory();
}

void TileMap::clearMemory()
{
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
	{
		for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
		{
			for (int z = 0; z < this->layers; z++)
			{
				for (size_t k = 0; k < this->map[x][y][z].size(); k++)
				{
					delete this->map[x][y][z][k];
					this->map[x][y][z][k] = NULL;
				}
				this->map[x][y][z].clear();
			}
			this->map[x][y].clear();
		}
		this->map[x].clear();
	}
	this->map.clear(); 
}

const sf::Texture* TileMap::getTileSheet() const
{
	return &this->tileSheet;
}

const int TileMap::getLayerSize(const int x, const int y, const int layer) const
{
	if (x >= 0 && x < this->map.size())
	{
		if (y >= 0 && y < this->map[x].size())
		{
			if (layer >= 0 && layer < this->map[x][y].size())
			{
				return this->map[x][y][layer].size();
			}
		}
	}
	return -1;
}

void TileMap::addTile(const unsigned x, const unsigned y, const unsigned z, const sf::IntRect& texture_rect,
							const bool collision, const short type)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0
		&& y < this->maxSizeWorldGrid.y && y >= 0
		&& z < this->layers && z >= 0)
	{
		/*if (this->map[x][y][z] == NULL)
		{*/
			this->map[x][y][z].push_back(new Tile(x, y, this->gridSizeF, this->tileSheet, texture_rect, collision, type));
		//}
	}
}

void TileMap::removeTile(const int x, const int y, const int z)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0
		&& y < this->maxSizeWorldGrid.y && y >= 0
		&& z < this->layers && z >= 0)
	{
		if (!this->map[x][y][z].empty())
		{
			delete this->map[x][y][z][this->map[x][y][z].size() - 1];
			this->map[x][y][z].pop_back();
		}
	}
}

void TileMap::saveToFile(const std::string file_name)
{
	//Save the tile map into a extension file.
	//The file is saved with the given name on the solution main folder.

	std::ofstream out_file;

	out_file.open(file_name);

	if (out_file.is_open())
	{
		out_file << this->maxSizeWorldGrid.x << " " << this->maxSizeWorldGrid.y << "\n"
					<< this->gridSizeI << "\n"
					<< this->layers << "\n"
					<< this->textureFile << "\n";

		for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
		{
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				for (int z = 0; z < this->layers; z++)
				{
					if (!this->map[x][y][z].empty())
					{
						for (size_t k = 0; k < this->map[x][y][z].size(); k++)
						{
							out_file << x << " " << y << " " << z << " " << this->map[x][y][z][k]->getAsString() << " ";
						}
					}
				}
			}
		}

	}
	else
	{
		std::cout << "Error while trying to save the tile map" << file_name << "\n";

	}

	out_file.close();
}

void TileMap::loadFromFile(const std::string file_name)
{
	//Load the tile map from a extension file.
	//The file is loeaded with the given name from the solution main folder.

	std::ifstream in_file;

	in_file.open(file_name);

	if (in_file.is_open())
	{
		sf::Vector2i size;
		int gridSize = 0;
		int layers = 0;
		std::string texture_file = "";
		int x = 0;
		int y = 0;
		int z = 0;
		int trX = 0;
		int trY = 0;
		bool collision = false;
		short type = 0;

		in_file >> size.x >> size.y >> gridSize >> layers >> texture_file;
		
		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeI = gridSize;
		this->maxSizeWorldGrid.x = size.x;
		this->maxSizeWorldGrid.y = size.y;
		this->layers = layers;
		this->textureFile = texture_file;

		this->clearMemory();

		this->map.resize(this->maxSizeWorldGrid.x, std::vector< std::vector< std::vector<Tile*> > >());
		for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
		{
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector <std::vector<Tile*> >());
				for (int z = 0; z < this->layers; z++)
				{
					this->map[x][y].resize(this->layers, std::vector<Tile*>());
				}
			}
		}

		if (!this->tileSheet.loadFromFile(texture_file))
		{
			std::cout << "Failed to load the texture sheet named:" << texture_file << "\n";
		}

		while(in_file >> x >> y >> z >> trX >> trY >> collision >> type)
		{
			this->map[x][y][z].push_back(new Tile(x, y, this->gridSizeF, this->tileSheet, sf::IntRect(trX, trY, this->gridSizeI, this->gridSizeI),
											collision, type));
		}

	}
	else
	{
		std::cout << "Error while trying to load the tile map" << file_name << "\n";

	}

	in_file.close();
}

void TileMap::updateCollision(Entity* entity, const float& dt)
{
	//Logic for the collision on windows.
	if (entity->getPosition().x < 0.f)
	{
		entity->setPosition(0.f, entity->getPosition().y);
		entity->stopVelocityX();
	}
	else if (entity->getPosition().x + entity->getGlobalBounds().width > this->maxSizeWorldF.x)
	{
		entity->setPosition(this->maxSizeWorldF.x - entity->getGlobalBounds().width, entity->getPosition().y);
		entity->stopVelocityX();
	}
	if (entity->getPosition().y < 0.f)
	{
		entity->setPosition(entity->getPosition().x, 0.f);
		entity->stopVelocityY();
	}
	else if (entity->getPosition().y + entity->getGlobalBounds().height > this->maxSizeWorldF.y)
	{
		entity->setPosition(entity->getPosition().x, this->maxSizeWorldF.y - entity->getGlobalBounds().height);
		entity->stopVelocityY();
	}

	//Logic for the collision on tiles.
	this->layer = 0;

	this->fromX = entity->getGridPosition(this->gridSizeI).x - 5;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSizeWorldGrid.x)
		this->fromX = this->maxSizeWorldGrid.x;

	this->toX = entity->getGridPosition(this->gridSizeI).x + 8;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSizeWorldGrid.x)
		this->toX = this->maxSizeWorldGrid.x;

	this->fromY = entity->getGridPosition(this->gridSizeI).y - 5;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSizeWorldGrid.y)
		this->fromY = this->maxSizeWorldGrid.y;

	this->toY = entity->getGridPosition(this->gridSizeI).y + 8;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSizeWorldGrid.y)
		this->toY = this->maxSizeWorldGrid.y;


	for (int x = this->fromX; x < this->toX; x++)
	{
		for (int y = this->fromY; y < this->toY; y++)
		{
			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				//This check, at the moment, is important, you need to check that the leyer position exists, or the sprite will be null
				//and the game will crash, this is wrong way for performance, a better way would be to not load null spaces.
				if (this->map[x][y][this->layer][k]) {

					sf::FloatRect playerBounds = entity->getGlobalBounds();
					sf::FloatRect wallBounds = this->map[x][y][this->layer][k]->getGlobalBounds();
					sf::FloatRect nextPositionBounds = entity->getNextPositionBounds(dt);

					if (this->map[x][y][this->layer][k]->getCollision() &&
						this->map[x][y][this->layer][k]->intersects(nextPositionBounds))
					{
						//Bottom collision
						if (playerBounds.top < wallBounds.top
							&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
							&& playerBounds.left < wallBounds.left + wallBounds.height
							&& playerBounds.left + playerBounds.width > wallBounds.left)
						{
							entity->stopVelocityY();
							entity->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
						}
						//Top collision
						else if (playerBounds.top > wallBounds.top
							&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
							&& playerBounds.left < wallBounds.left + wallBounds.height
							&& playerBounds.left + playerBounds.width > wallBounds.left)
						{
							entity->stopVelocityY();
							entity->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
						}

						//Right collision
						if (playerBounds.left < wallBounds.left
							&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
							&& playerBounds.top < wallBounds.top + wallBounds.height
							&& playerBounds.top + playerBounds.height > wallBounds.top)
						{
							entity->stopVelocityX();
							entity->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
						}

						//Left collision
						if (playerBounds.left > wallBounds.left
							&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
							&& playerBounds.top < wallBounds.top + wallBounds.height
							&& playerBounds.top + playerBounds.height > wallBounds.top)
						{
							entity->stopVelocityX();
							entity->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
						}

					}
				}
			}
		}
	}
}

void TileMap::update()
{

}

void TileMap::render(sf::RenderTarget& target, sf::Vector2i gridpPosition)
{
	this->layer = 0;

	this->fromX = gridpPosition.x - 15;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSizeWorldGrid.x)
		this->fromX = this->maxSizeWorldGrid.x;

	this->toX = gridpPosition.x + 40;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSizeWorldGrid.x)
		this->toX = this->maxSizeWorldGrid.x;

	this->fromY = gridpPosition.y - 15;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSizeWorldGrid.y)
		this->fromY = this->maxSizeWorldGrid.y;

	this->toY = gridpPosition.y + 40;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSizeWorldGrid.y)
		this->toY = this->maxSizeWorldGrid.y;

	for (int x = this->fromX; x < this->toX; x++)
	{
		for (int y = this->fromY; y < this->toY; y++)
		{
			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
			//This check, at the moment, is important, you need to check that the leyer position exists, or the sprite will be null
			//and the game will crash, this is wrong way for performance, a better way would be to not load null spaces.
				if (this->map[x][y][this->layer][k]) {
					if (this->map[x][y][this->layer][k]->getType() == TileTypes::OVERLAPTILE)
					{
						this->deferredRenderStack.push(this->map[x][y][this->layer][k]);
					}
					else
					{
						this->map[x][y][this->layer][k]->render(target);
					}
					if (this->map[x][y][this->layer][k]->getCollision())
					{
						this->collisionBox.setPosition(this->map[x][y][this->layer][k]->getPosition());
						target.draw(this->collisionBox);
					}
				}
			}
				
		}
	}
	/*else
	{
		for (auto& x : this->map)
		{
			for (auto& y : x)
			{
				for (auto& z : y)
				{
					for(auto *k : z)
					{
						if (k != NULL)
						{
							k->render(target);
							if (k->getCollision())
							{
								this->collisionBox.setPosition(k->getPosition());
								target.draw(this->collisionBox);
							}

						}
					}
					
				}
			}
		}
	}*/
}

void TileMap::renderDeferred(sf::RenderTarget& target)
{
	while (!this->deferredRenderStack.empty())
	{
		deferredRenderStack.top()->render(target);
		deferredRenderStack.pop();
	}
}
