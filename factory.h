#ifndef FACTORY_H
#define FACTORY_H

#include <vector>
#include <string>
#include <memory>

#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include "icon.h"

class Factory
{
private:
	static sf::Color
	getColor(std::string name)
	{
		for(char & letter : name)
			letter = tolower(letter);
		if(!strcmp(&name[0], "black"))
			return sf::Color::Black;
		else if(!strcmp(&name[0], "white"))
			return sf::Color::White;
		else if(!strcmp(&name[0], "red"))
			return sf::Color::Red;
		else if(!strcmp(&name[0], "green"))
			return sf::Color::Green;
		else if(!strcmp(&name[0], "blue"))
			return sf::Color::Blue;
		else if(!strcmp(&name[0], "yellow"))
			return sf::Color::Yellow;
		else if(!strcmp(&name[0], "magenta"))
			return sf::Color::Magenta;
		else if(!strcmp(&name[0], "transparent"))
			return sf::Color::Transparent;
		printf("Color name not found! (%s)\n", &name[0]);
		return sf::Color::White;
	}

	static Shape::ID
	getShapeID(std::string shape)
	{
		for(char & letter : shape)
			letter = tolower(letter);
		if(!strcmp(&shape[0], "circle"))
			return Shape::ID::CIRCLE;
		else if(!strcmp(&shape[0], "rectangle"))
			return Shape::ID::RECTANGLE;
		else if(!strcmp(&shape[0], "triangle"))
			return Shape::ID::TRIANGLE;
		if(!strcmp(&shape[0], "icon"))
			return Shape::ID::ICON;
		printf("Shape not found! (%s)\n", &shape[0]);
		return Shape::ID::UNINITIALIZED;
	}
public:
	Factory()
	{}
	~Factory()
	{}
	static std::shared_ptr<Shape> create(std::vector<std::string> s)
	{
		try
		{
			if(s.size() == 0)
			{
				throw std::runtime_error("Error : Empty order ...");
			}
		
			int val[s.size()];
			for(int i = 0; i < s.size(); ++i)
			{
				val[i] = strtol(&s[i][0], NULL, 10);
			}

			switch(getShapeID(s[0]))		// id
			{
				case Shape::ID::CIRCLE: 	// Circle
				{
					if(s.size() < 7)
					{
						throw std::runtime_error("Error : Not enough parameters for Circle shape ...");
					}

					return std::make_shared<Circle>(
						sf::Vector2f(val[1], val[2]), 					// position(x,y)
						val[3], 										// radius
						getColor(s[4]),									// fillcolor(r,g,b,a)
						getColor(s[5]),								 	// outlineColor(r,g,b,a)
						getColor(s[6])								 	// selectColor(r,g,b,a)
					);

				}
				case Shape::ID::RECTANGLE: // Rectangle
				{
					if(s.size() < 8)
					{
						throw std::runtime_error("Error : Not enough parameters for Rectangle shape ...");
					}

					return std::make_shared<Rectangle>(
						sf::Vector2f(val[1], val[2]), 					// position(x,y)
						sf::Vector2f(val[3], val[4]),					// size(x,y)
						getColor(s[5]),									// fillcolor(r,g,b,a)
						getColor(s[6]),								 	// outlineColor(r,g,b,a)
						getColor(s[7])								 	// selectColor(r,g,b,a)
					);

				}
				case Shape::ID::TRIANGLE: // Triangle
				{
					if(s.size() < 7)
					{
						throw std::runtime_error("Error : Not enough parameters for Triangle shape ...");
					}

					return std::make_shared<Triangle>(
						sf::Vector2f(val[1], val[2]), 					// position(x,y)
						val[3], 										// radius
						getColor(s[4]),									// fillcolor(r,g,b,a)
						getColor(s[5]),								 	// outlineColor(r,g,b,a)
						getColor(s[6])								 	// selectColor(r,g,b,a)
					);

				}
				default:
				{
					throw std::runtime_error("Error : Invalid shape ID ...");
				}
			}
		}
		
		catch (const std::runtime_error & error)
		{
			printf("Error: %s\n", error.what()); fflush(stdout);
			exit(-1);
		}
	}
};


#endif // FACTORY_H