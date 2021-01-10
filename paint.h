#ifndef PAINT_H
#define PAINT_H

// std_C includes
#include <stdio.h>

// std::C++ includes
#include <memory>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <algorithm>

// C++ includes
#include <SFML/Graphics.hpp>

// Local includes
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
//
#include "menu.h"
#include "icon.h"
#include "cursor.h"
#include "action.h"
//
#include "factory.h"
#include "option.h"
#include "command.h"
#include "command_list.h"

class Paint
{
private:
	// Private member variables
	sf::RenderWindow window;
	sf::View view;
	sf::Event event;
	
	std::string savePATH;
	std::string loadPATH;
	sf::Texture texturePack;
	
	static constexpr unsigned int winSizeX = 720;
	static constexpr unsigned int winSizeY = 480;
	unsigned int windowOffsetY = 0;
	//
	unsigned int tileSize = 32;
	unsigned int maxTileX = 8;
	unsigned int maxTileY = 4;

	double minObjectSize = 20;
	double maxObjectSize = 200;
	double objectSize = 50;
	double objectSizeStep = 10;

	bool isMovingShape = false;
	sf::Vector2f oldPos;
	
	bool isPressed = false;
	
	unsigned int maxCommands = 10;
	
	std::vector<Action> actions;	
	std::vector<std::vector<std::string>> iconNames;

	std::vector<std::shared_ptr<Shape>> userShapes;
	std::vector<std::shared_ptr<Shape>> paintShapes;
	std::vector<std::shared_ptr<Menu>> menus;

	std::shared_ptr<Cursor> cursor;
	std::shared_ptr<Shape> selectedShape;

	Option option = Option::COLOR_FILL;
	Option previousOption = Option::COLOR_FILL;
	sf::Color color = sf::Color::White;

	CommandList commands;

	// Private member functions
	void addActions();
	void fillMenu(std::shared_ptr<Menu> menu, std::vector<std::string> names, int offsetX, int offsetY, unsigned int divPosX, unsigned int divPosY);
	sf::Vector2i getTextureCoords(Option option);

public:
	// Public member variables
	bool isRunning = true;

	// Public member functions
	Paint(std::string savePATH, std::string loadPATH, unsigned int maxCommands, std::string texturePATH);
	~Paint();
	
	void handleEvents();
	void update();
	void render();

	void load();
	void save();
};

#endif // PAINT_H