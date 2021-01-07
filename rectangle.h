#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape
{
public:
	sf::RectangleShape body;
	
	Rectangle(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor, sf::Color outlineColor, sf::Color selectColor) :
		body(size), Shape(body)
	{
		this->id = ID::RECTANGLE;
		this->fillColor = fillColor;
		this->outlineColor = outlineColor;
		this->selectColor = selectColor;
		
		this->size = size;
		
		body.setOrigin(size.x/2, size.y/2);
		body.setPosition(position);

		body.setOutlineThickness(2);
		body.setFillColor(fillColor);
		body.setOutlineColor(outlineColor);
	}
	virtual ~Rectangle()
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
		
		s += getShapeID(id) + " ";
		s += std::to_string((int)body.getPosition().x) + " ";
		s += std::to_string((int)body.getPosition().y) + " ";
		s += std::to_string((int)body.getSize().x) + " ";
		s += std::to_string((int)body.getSize().y) + " ";
		s += getColor(fillColor) + " ";
		s += getColor(outlineColor) + " ";
		s += getColor(selectColor) + " ";
		s += ";\n";
		
		return s;
	}
};

#endif // RECTANGLE_H