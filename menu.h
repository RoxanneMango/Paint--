#ifndef MENU_H
#define MENU_H

#include <memory>
#include <vector>

#include "rectangle.h"
#include "icon.h"

class Menu : public Rectangle
{
public:
	std::vector<std::shared_ptr<Icon>> icons;

	Menu(sf::Vector2f position, sf::Vector2f size, sf::Color color) : 
		Rectangle(position, size, color, sf::Color::Black, sf::Color::Black)
	{
		body.setOutlineThickness(2);
		body.setOrigin(0, 0);
		this->size = size;
	}
	virtual ~Menu()
	{}
	
	void addIcon(Option * optionPointer, Option option, std::string name, sf::Vector2f position, sf::Vector2f size, sf::Texture * texture, sf::IntRect textureRect, sf::Color outlineColor, sf::Color selectColor, bool isOutlined = false)
	{
		icons.push_back(std::make_shared<Icon>(optionPointer, option, name, position, size, texture, textureRect, outlineColor, selectColor, isOutlined));
	}
};

#endif // MENU_H