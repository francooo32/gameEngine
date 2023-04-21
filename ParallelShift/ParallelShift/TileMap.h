#ifndef TILEMAP_H
#define TILEMAP_H 

#include "Tile.h"

class TileMap
{
private:
	float gridSizeF;
	unsigned gridSizeU;
	unsigned layers;
	sf::Vector2u maxSize;
	
	std::vector<std::vector<std::vector<Tile*> > > map;

	sf::Texture tileSheet;
	std::string textureFile;

	void clearMemory();

public:
	TileMap(float gridSize, unsigned width, unsigned height, std::string texture_file);
	virtual ~TileMap();

	const sf::Texture* getTileSheet() const;

	void addTile(const unsigned x, const unsigned y, const unsigned z, const sf::IntRect& texture_rect, 
					const bool collision, const short type);
	void removeTile(const unsigned x, const unsigned y, const unsigned z);
	void saveToFile(const std::string file_name);
	void loadFromFile(const std::string file_name);
	void update();
	void render(sf::RenderTarget& target);
};

#endif