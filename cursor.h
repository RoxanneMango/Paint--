#ifndef CURSOR_H
#define CURSOR_H

#include "shape.h"

class Cursor : public Shape
{
	sf::Texture * texture;
	sf::IntRect textureRect;
public:
	sf::RectangleShape body;
	
	Cursor(sf::Vector2f size) : body(size), Shape(body)
	{
		body.setOrigin(size.x/2, size.y/2);
		this->size = size;
	}
	virtual ~Cursor()
	{}

	bool interact(sf::Shape & body) override
	{
		return false;
	}

	void update() override
	{}
	
	void draw(sf::RenderWindow & window) const override
	{
		window.draw(body);
	}
	
	virtual void setSize(double size) override
	{
		this->size.x = size;
		this->size.y = size;
		body.setSize(this->size);
		body.setOrigin(size/2, size/2);
	}
	
	std::string getReceipt() override
	{
		std::string s;
		
		s += std::to_string((int)id);
		
		return s;
	}

	std::vector<std::string> getOrder() override
	{
		return std::vector<std::string> { std::to_string((int)id) };
	}

	void setTexture(sf::Texture * texture)
	{
		this->texture = texture;
		body.setTexture(texture);
	}
	void setTextureRect(sf::IntRect textureRect)
	{
		this->textureRect = textureRect;
		body.setTextureRect(textureRect);
	}
	void setTextureRect(int x, int y, int w, int h)
	{
		this->textureRect = sf::IntRect(x,y,w,h);
		body.setTextureRect(textureRect);
	}
	
};

#endif // CURSOR_H