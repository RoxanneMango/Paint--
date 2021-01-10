#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape
{
public:
	sf::CircleShape body;

	Triangle(sf::Vector2f position, int radius, sf::Color fillColor, sf::Color outlineColor, sf::Color selectColor) : 
		body(radius, 3), Shape(body)
	{
		this->id = ID::TRIANGLE;
		this->fillColor = fillColor;
		this->outlineColor = outlineColor;
		this->selectColor = selectColor;
		
		this->size = sf::Vector2f(radius, 0);
		
		body.setOrigin(radius, radius);
		body.setPosition(position);
		
		body.setOutlineThickness(2);
		body.setFillColor(fillColor);
		body.setOutlineColor(outlineColor);
	}
	virtual ~Triangle()
	{}
	
	void draw(sf::RenderWindow & window) const override
	{
		window.draw(body);
	}
	
	virtual void setSize(double size) override
	{
		this->size.x = size;
		body.setRadius(size);
		body.setOrigin(size, size);
	}
	
	std::string getReceipt() override
	{
		std::string s;
		
		s += getShapeID(id) + " ";
		s += std::to_string((int)body.getPosition().x) + " ";
		s += std::to_string((int)body.getPosition().y) + " ";
		s += std::to_string((int)body.getRadius()) + " ";
		s += getColor(fillColor) + " ";
		s += getColor(outlineColor) + " ";
		s += getColor(selectColor) + " ";
		s += ";\n";
		
		return s;
	}
	std::vector<std::string> getOrder() override
	{
		return std::vector<std::string>
		{
			getShapeID(id),
			std::to_string((int)body.getPosition().x),
			std::to_string((int)body.getPosition().y),
			std::to_string((int)body.getRadius()),
			getColor(fillColor),
			getColor(outlineColor),
			getColor(selectColor)
		};
	}
};

#endif // TRIANGLE_H