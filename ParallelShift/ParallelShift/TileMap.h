#ifndef TILEMAP_H
#define TILEMAP_H 

#include "Tile.h"
#include "Entity.h"

class Tile;
class Entity;

class TileMap
{
private:
	float gridSizeF;
	int gridSizeI;
	int layers;
	sf::Vector2i maxSizeWorldGrid;
	sf::Vector2f maxSizeWorldF;
	std::vector<std::vector<std::vector<std::vector<Tile*> > > > map;
	std::stack<Tile*> deferredRenderStack;
	sf::Texture tileSheet;
	std::string textureFile;
	sf::RectangleShape collisionBox;	

	int fromX;
	int toX;
	int fromY;
	int toY;
	int layer;

	void clearMemory();

public:
	TileMap(float gridSize, int width, int height, std::string texture_file);
	virtual ~TileMap();

	const sf::Texture* getTileSheet() const;
	const int getLayerSize(const int x, const int y, const int layer) const;

	void addTile(const unsigned x, const unsigned y, const unsigned z, const sf::IntRect& texture_rect, 
					const bool collision, const short type);
	void removeTile(const int x, const int y, const int z);
	void saveToFile(const std::string file_name);
	void loadFromFile(const std::string file_name);
	void updateCollision(Entity* entity, const float& dt);
	void update();
	void render(sf::RenderTarget& target, sf::Vector2i gridpPosition);
	void renderDeferred(sf::RenderTarget& target);
};

#endif