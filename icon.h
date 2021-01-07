#ifndef ICON_H
#define ICON_H

#include "shape.h"
#include "option.h"

class Icon : public Shape
{
	sf::Texture * texture;
	sf::IntRect textureRect;
	Option * optionPointer;
	Option option;
public:
	std::string name;
	sf::RectangleShape body;
	
	Icon(Option * optionPointer, Option option, std::string name, sf::Vector2f position, sf::Vector2f size, sf::Texture * texture, sf::IntRect textureRect, sf::Color outlineColor, sf::Color selectColor, bool isOutlined) :
		texture(texture), optionPointer(optionPointer), option(option), name(name), textureRect(textureRect), body(size), Shape(body)
	{
		body.setTexture(texture);
		body.setTextureRect(textureRect);
		
		this->id = ID::ICON;
		this->fillColor = sf::Color::Transparent;
		this->outlineColor = outlineColor;
		this->selectColor = selectColor;
		
		body.setPosition(sf::Vector2f(position.x + (size.x/2), position.y + (size.y/2)));
		body.setOrigin(size.x/2, size.y/2);

		if(isOutlined)
		{
			body.setOutlineThickness(1);
			body.setOutlineColor(outlineColor);
		}
	}
	
	virtual ~Icon()
	{}
	
	virtual bool interact(sf::Shape & body) override
	{
		isSelected = false;
		
		//if(&this->body == &body) return false; // if it is the same body return by default
		
		float deltaX = this->body.getPosition().x - body.getPosition().x;
		float deltaY = this->body.getPosition().y - body.getPosition().y;
		//
		float intersectX = abs(deltaX * 2) - (this->body.getOrigin().x + body.getOrigin().x);
		float intersectY = abs(deltaY * 2) - (this->body.getOrigin().y + body.getOrigin().y);
		//	
		if(intersectX < 0.0f && intersectY < 0.0f)
		{
			if((int)option < (int)Option::SIZE)
			{
				*optionPointer = option;
				body.setTextureRect(textureRect);
			}
			isSelected = true;
			return true;
		}
		return false;
	}
	
	void update() override
	{
		if(isSelected)
		{
			
		}
	}
	
	void draw(sf::RenderWindow & window) const override
	{
		window.draw(body);
	}
	
	std::string getReceipt() override
	{
		return NULL;
	}
};

#endif // ICON_H