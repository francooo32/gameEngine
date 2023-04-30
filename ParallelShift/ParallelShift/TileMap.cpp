#include "stdafx.h"
#include "TileMap.h"



TileMap::TileMap(float gridSize, unsigned width, unsigned height, std::string texture_file)
{
	//Carefull with the size, if you run a large size tile map, lets say 3000 x and y, you gonna have problems if you are in debug mode, use
	//release mode in that case cause is hard to allocate all that memory in debug.
	this->gridSizeF = gridSize;
	this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
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
	
	this->map.resize(this->maxSizeWorldGrid.x, std::vector< std::vector<Tile*> >());
	for (size_t x = 0; x < this->maxSizeWorldGrid.x; x++)
	{
			for (size_t y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<Tile*>());
					for (size_t z = 0; z < this->layers; z++)
					{
						this->map[x][y].resize(this->layers, NULL);
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
	for (size_t x = 0; x < this->maxSizeWorldGrid.x; x++)
	{
		for (size_t y = 0; y < this->maxSizeWorldGrid.y; y++)
		{
			for (size_t z = 0; z < this->layers; z++)
			{
				delete this->map[x][y][z];
				this->map[x][y][z] = NULL;
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

void TileMap::addTile(const unsigned x, const unsigned y, const unsigned z, const sf::IntRect& texture_rect,
							const bool collision, const short type)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0
		&& y < this->maxSizeWorldGrid.y && y >= 0
		&& z < this->layers && z >= 0)
	{
		if (this->map[x][y][z] == NULL)
		{
			this->map[x][y][z] = new Tile(x, y, this->gridSizeF, this->tileSheet, texture_rect, collision, type);
		}
	}
}

void TileMap::removeTile(const unsigned x, const unsigned y, const unsigned z)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0
		&& y < this->maxSizeWorldGrid.y && y >= 0
		&& z < this->layers && z >= 0)
	{
		if (this->map[x][y][z] != NULL)
		{
			delete this->map[x][y][z];
			this->map[x][y][z] = NULL;
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
					<< this->gridSizeU << "\n"
					<< this->layers << "\n"
					<< this->textureFile << "\n";

		for (size_t x = 0; x < this->maxSizeWorldGrid.x; x++)
		{
			for (size_t y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				for (size_t z = 0; z < this->layers; z++)
				{
					if(this->map[x][y][z])
					out_file << x << " " << y << " " << z << " " << this->map[x][y][z]->getAsString() << " ";
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
		sf::Vector2u size;
		unsigned gridSize = 0;
		unsigned layers = 0;
		std::string texture_file = "";
		unsigned x = 0;
		unsigned y = 0;
		unsigned z = 0;
		unsigned trX = 0;
		unsigned trY = 0;
		bool collision = false;
		short type = 0;

		in_file >> size.x >> size.y >> gridSize >> layers >> texture_file;
		
		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeU = gridSize;
		this->maxSizeWorldGrid.x = size.x;
		this->maxSizeWorldGrid.y = size.y;
		this->layers = layers;
		this->textureFile = texture_file;

		this->clearMemory();

		this->map.resize(this->maxSizeWorldGrid.x, std::vector< std::vector<Tile*> >());
		for (size_t x = 0; x < this->maxSizeWorldGrid.x; x++)
		{
			for (size_t y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<Tile*>());
				for (size_t z = 0; z < this->layers; z++)
				{
					this->map[x][y].resize(this->layers, NULL);
				}
			}
		}

		if (!this->tileSheet.loadFromFile(texture_file))
		{
			std::cout << "Failed to load the texture sheet named:" << texture_file << "\n";
		}

		while(in_file >> x >> y >> z >> trX >> trY >> collision >> type)
		{
			this->map[x][y][z] = new Tile(x, y, this->gridSizeF, this->tileSheet, sf::IntRect(trX, trY, this->gridSizeU, this->gridSizeU),
											collision, type);
		}

	}
	else
	{
		std::cout << "Error while trying to load the tile map" << file_name << "\n";

	}

	in_file.close();
}

void TileMap::updateCollision(Entity* entity)
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
	this->fromX = entity->getGridPosition(this->gridSizeU).x - 2;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX >= this->maxSizeWorldGrid.x)
		this->fromX = this->maxSizeWorldGrid.x - 1;

	this->toX = entity->getGridPosition(this->gridSizeU).x + 1;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX >= this->maxSizeWorldGrid.x)
		this->toX = this->maxSizeWorldGrid.x - 1;

	this->fromY = entity->getGridPosition(this->gridSizeU).y - 2;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY >= this->maxSizeWorldGrid.y)
		this->fromY = this->maxSizeWorldGrid.y - 1;

	this->toY = entity->getGridPosition(this->gridSizeU).y + 1;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY >= this->maxSizeWorldGrid.y)
		this->toY = this->maxSizeWorldGrid.y - 1;

	for (size_t x = this->fromX; x < this->toX; x++)
	{
		for (size_t y = this->fromY; y < this->toY; y++)
		{
	
		}
	}
}

void TileMap::update()
{

}

void TileMap::render(sf::RenderTarget& target, Entity* entity)
{
	for (auto& x : this->map)
	{
		for (auto& y : x)
		{
			for (auto* z : y)
			{
				if (z != NULL)
				{
					z->render(target);
					if (z->getCollision())
					{
						this->collisionBox.setPosition(z->getPosition());
						target.draw(this->collisionBox);
					}
					
				}
			}
		}
	}
}