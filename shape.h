#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/Graphics.hpp>

class Shape
{
protected:
	sf::Vector2f size;
	
	sf::Color fillColor;
	sf::Color outlineColor;
	sf::Color selectColor;
public:
	enum class ID { UNINITIALIZED = -1, CIRCLE = 0, RECTANGLE = 1, TRIANGLE = 2, ICON = 3};

	ID id = ID::UNINITIALIZED;
	bool isVisible = true;
	sf::Shape & body;

	Shape(sf::Shape & body) : body(body)
	{}
	virtual ~Shape()
	{}
	
	virtual bool interact(sf::Shape & body)
	{
		if(&this->body == &body) return false; // if it is the same body, return by default
		
		float deltaX = this->body.getPosition().x - body.getPosition().x;
		float deltaY = this->body.getPosition().y - body.getPosition().y;
		//
		float intersectX = abs(deltaX) - (this->body.getOrigin().x + body.getOrigin().x);
		float intersectY = abs(deltaY) - (this->body.getOrigin().y + body.getOrigin().y);
		//	
		if(intersectX < 0.0f && intersectY < 0.0f)
		{	
			return true;
		}
		return false;
		
	}
	virtual void update()
	{}

	static std::string
	getColor(sf::Color color)
	{
		if(color == sf::Color::Black)
			return "BLACK";
		else if(color == sf::Color::White)
			return "WHITE";
		else if(color == sf::Color::Red)
			return "RED";
		else if(color == sf::Color::Green)
			return "GREEN";
		else if(color == sf::Color::Blue)
			return "BLUE";
		else if(color == sf::Color::Yellow)
			return "YELLOW";
		else if(color == sf::Color::Magenta)
			return "MAGENTA";
		else if(color == sf::Color::Transparent)
			return "TRANSPARENT";
		printf("Color not found! (%d)\n", color.toInteger());
		return "WHITE";
	}

	std::string
	getShapeID(ID shape)
	{
		if(shape == Shape::ID::CIRCLE)
			return "CIRCLE";
		if(shape == Shape::ID::RECTANGLE)
			return "RECTANGLE";
		if(shape == Shape::ID::TRIANGLE)
			return "TRIANGLE";
		if(shape == Shape::ID::ICON)
			return "ICON";
		printf("ShapeID not found! (%d)\n", (int)shape);
		return "UNINITIALIZED";
	}

	void setFillColor(sf::Color color)
	{
		this->fillColor = color;
		body.setFillColor(color);
	}
	void setOutlineColor(sf::Color color)
	{
		this->outlineColor = color;
		this->selectColor = color;
		body.setOutlineColor(color);
	}

	sf::Vector2f getSize()
	{
		return size;
	}
	
	virtual void setSize(double size)
	{
		this->size.x = size;
		this->size.y = size;
	}

	virtual void draw(sf::RenderWindow & window) const = 0;
	virtual std::string getReceipt() = 0;
	virtual std::vector<std::string> getOrder() = 0;
};

#endif // SHAPE_H