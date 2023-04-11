#ifndef TILE_H
#define TILE_H

class Tile
{

private :

protected :
	sf::RectangleShape shape;

public :
	Tile();
	Tile(float x, float y, float griSizeF, sf::Texture& texture);
	virtual ~Tile();

	void update();
	void render(sf::RenderTarget& target);
};

#endif 