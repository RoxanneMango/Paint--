#include "paint.h"

Paint::Paint(std::string savePATH, std::string loadPATH, unsigned int maxCommands, std::string texturePATH) :
	window(sf::VideoMode(winSizeX, winSizeY), "Paint--", sf::Style::Close), 
	view(sf::Vector2f(0.0f,0.0f), sf::Vector2f(winSizeX, winSizeY)),
	savePATH(savePATH), loadPATH(loadPATH), maxCommands(maxCommands),
	commands(maxCommands)
{
	try
	{
		window.setFramerateLimit(60);
		window.setMouseCursorVisible(false);
		
		if(!(texturePack.loadFromFile(&texturePATH[0])))
		{
			throw "Could not load icons";
		}
		
		addActions();

		menus.push_back(std::make_shared<Menu>(sf::Vector2f(0, 1), sf::Vector2f(winSizeX, 32), sf::Color(211,211,211)));	// lightgray
		menus.push_back(std::make_shared<Menu>(sf::Vector2f(0, 32), sf::Vector2f(winSizeX, 45), sf::Color(128,128,128)));	// gray
		for(std::shared_ptr<Menu> menu : menus)
		{
			windowOffsetY += menu->getSize().y;
			paintShapes.push_back(menu);
		}
		
		iconNames.push_back( std::vector<std::string> { "open_file","save_file","undo","redo","divider","help" } );
		iconNames.push_back( std::vector<std::string> { 
			"create_rectangle","create_circle","create_triangle","divider","move","lower_z_index","up_z_index",
			"resize_add","resize_sub","divider","delete","divider","color_outline","color_fill","divider",
			"color_red","color_green","color_blue","color_yellow","color_magenta","color_black","color_white" } );

		if((menus.size() < 2) || iconNames.size() < 2)
		{
			throw "Something went wrong while building the menus";
		}
		
		// menu, names, offsetX, offsetY, divPosX, divPosY)
		fillMenu(menus[0], iconNames[0], 8, -2, 5, 3);
		fillMenu(menus[1], iconNames[1], 2, 6, 6, 3);

		cursor = std::make_shared<Cursor>(sf::Vector2f(30,30));
		sf::IntRect cursorTexture = sf::IntRect(tileSize * 7, tileSize * 1, tileSize, tileSize);		
		cursor->setTexture(&texturePack);
		cursor->setTextureRect(cursorTexture);
		paintShapes.push_back(cursor);
	}
	catch(const char * error)
	{
		printf("Error : %s\n", error);
		exit(-1);
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
		exit(-1);
	}
	catch( ... )
	{
		printf("Error : Could not construct Paint instance\n");
		exit(-1);
	}
}

Paint::~Paint()
{
	try
	{
		// deallocate all shapes that was were created by the user
		for(std::reverse_iterator shapeIterator = userShapes.rbegin(); shapeIterator != userShapes.rend(); ++shapeIterator)
		{
			(*shapeIterator).reset();
			userShapes.erase(std::next(shapeIterator).base());
		}
		// Erase all elements in menus and menus->icons; allocated pointers are
		// also saved in paintShapes and will be deleted through there
		for(std::reverse_iterator menuIterator = menus.rbegin(); menuIterator != menus.rend(); ++menuIterator)
		{
			for(std::reverse_iterator iconIterator = (*menuIterator)->icons.rbegin(); iconIterator != (*menuIterator)->icons.rend(); ++iconIterator)
			{
				(*menuIterator)->icons.erase(std::next(iconIterator).base());
			}
			menus.erase(std::next(menuIterator).base());
		}
		// deallocate all Paint shapes
		for(std::reverse_iterator shapeIterator = paintShapes.rbegin(); shapeIterator != paintShapes.rend(); ++shapeIterator)
		{
			(*shapeIterator).reset();
			paintShapes.erase(std::next(shapeIterator).base());
		}
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
	}
	catch( ... )
	{
		printf("Something went wrong while destructing Paint instance\n");
	}
}

void
Paint::handleEvents()
{
	while (window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		if(event.type == sf::Event::MouseButtonReleased)
			isPressed = false;
	}
	isRunning = window.isOpen();
}

void
Paint::update()
{
	if(isRunning)
	{
		if(sf::Mouse::getPosition(window).y < windowOffsetY || (sf::Mouse::getPosition(window).y < 0))
		{
			window.setMouseCursorVisible(true);
			cursor->isVisible = false;
		}
		else
		{
			window.setMouseCursorVisible(false);
			cursor->isVisible = true;
		}
		for( Action & action : actions )
		{
			action();
		}

		if((int)option >= (int)Option::SET_COLOR_RED && (int)option < (int)Option::SIZE)
		{
			color =
				(option == Option::SET_COLOR_RED) ? 
					sf::Color::Red :
				(option == Option::SET_COLOR_GREEN) ?
					sf::Color::Green :
				(option == Option::SET_COLOR_BLUE) ? 
					sf::Color::Blue :
				(option == Option::SET_COLOR_YELLOW) ?
					sf::Color::Yellow :
				(option == Option::SET_COLOR_MAGENTA) ? 
					sf::Color::Magenta :
				(option == Option::SET_COLOR_BLACK) ?
					sf::Color::Black :
				sf::Color::White;
		}
		if((int)option < (int)Option::CREATE_RECTANGLE || (int)option > (int)Option::COLOR_FILL)
		{
			option = previousOption;
			sf::Vector2i coords = getTextureCoords(option);
			cursor->setTextureRect(coords.x * tileSize, coords.y * tileSize, tileSize, tileSize);
		}
	}
}

void
Paint::render()
{
	if(isRunning)
	{
		window.clear(sf::Color(24, 24, 24));
		for(std::shared_ptr<Shape> shape : userShapes)
		{
			shape->draw(window);
		}
		for(std::shared_ptr<Shape> shape : paintShapes)
		{
			if(shape->isVisible)
			{
				shape->draw(window);
			}
		}
		window.display();
	}
}

void
Paint::load()
{
	try
	{
		std::vector<std::vector<std::string>> orderList;
		std::ifstream ifs;
		
		ifs.open(loadPATH, std::ifstream::in);

		std::string buffer;
		unsigned int index = 0;
		orderList.push_back(std::vector<std::string>());

		while(ifs.good())
		{
			ifs >> buffer;
			if(!buffer.size())
			{
				throw "File is empty!";
			}
			if(!strcmp(&buffer[0], ";"))
			{
				orderList.push_back(std::vector<std::string>());
				index += 1;
				continue;			
			}
			orderList[index].push_back(buffer);
			buffer.clear();
		}
		orderList.pop_back();	// pop back extra element
		orderList.pop_back();	// pop back extra empty element which is caused by a newline
		ifs.close();
		
		// Delete current userShapes, if there are any
		for(std::reverse_iterator shapeIterator = userShapes.rbegin(); shapeIterator != userShapes.rend(); ++shapeIterator)
		{
			(*shapeIterator).reset();
			userShapes.erase(std::next(shapeIterator).base());
		}
		
		for(std::vector<std::string> & order : orderList)
		{
			userShapes.push_back(Factory::create(order));
		}
	}
	catch( const char * message )
	{
		printf("Error : %s\n", message);
		return;
	}
	catch(std::ios_base::failure & exception)
	{
		printf("Could not open file! (%s)\n", loadPATH);
		return;
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
		return;
	}
	catch( ... )
	{
		printf("Something went wrong while loading contents from file in Paint instance\n");
		return;
	}
}

void
Paint::save()
{
	try
	{
		std::ofstream ofs;
		ofs.open(savePATH, std::ifstream::out);
		for(std::shared_ptr<Shape> shape : userShapes)
		{
			if((int)shape->id >= 0)
			{
				ofs << shape->getReceipt();
			}
		}
		ofs.close();
	}
	catch(std::ios_base::failure & exception)
	{
		printf("Could not open file! (%s)\n", savePATH);
		return;
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
		return;
	}
	catch( ... )
	{
		printf("Something went wrong while saving contents to file in Paint instance\n");
		return;
	}
}

void
Paint::fillMenu(std::shared_ptr<Menu> menu, std::vector<std::string> names, int offsetX, int offsetY, unsigned int divPosX, unsigned int divPosY)
{
	try
	{
		sf::IntRect divider(divPosX * tileSize, divPosY * tileSize, tileSize, tileSize);

		int posX = menu->body.getPosition().x;
		int posY = menu->body.getPosition().y;

		unsigned int index = 0;

		static unsigned int row = 0;
		static unsigned int column = 0;

		double divdiv = 2;
		int numOfDividers = 0;

		for(std::string name : names)
		{
			if(column >= maxTileX)
			{
				row += 1;
				column = 0;
			}
			if(row >= maxTileY)
			{
				throw "Row out of bounds!\n";
			}
			if(!strcmp(&name[0], "divider"))
			{
				menu->addIcon(&option, Option::SIZE, name, sf::Vector2f((tileSize * index) + (offsetX * index) + offsetX - ((tileSize / divdiv) * numOfDividers), posY + offsetY), sf::Vector2f(tileSize/divdiv, tileSize), &texturePack, divider, sf::Color::Black, sf::Color::Green);
				numOfDividers += 1;
			}
			else
			{
				int sum = row * maxTileX;
				Option iconOption = static_cast<Option>((sum + column) < (int)Option::SIZE ? (sum + column) : (int)Option::MOVE);
				menu->addIcon(&option, iconOption, name, sf::Vector2f((tileSize * index) + (offsetX * index) + offsetX - ((tileSize / divdiv) * numOfDividers), posY + offsetY), sf::Vector2f(tileSize, tileSize), &texturePack, sf::IntRect((tileSize * column), (tileSize * row), tileSize, tileSize), sf::Color::Black, sf::Color::Green);				
				column += 1;
			}
			paintShapes.push_back(menu->icons[menu->icons.size()-1]);
			index += 1;
		}
	}
	catch(const char * error)
	{
		printf("Error : %s\n", error);
		exit(-1);
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
		exit(-1);
	}
	catch( ... )
	{
		printf("Something went wrong while constructing menus in Paint instance\n");
		exit(-1);
	}
}

void
Paint::addActions()
{
	try
	{
		actions.push_back(
			Action( [&]()
			{
				cursor->body.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
			})
		);
		actions.push_back(
			Action( sf::Mouse::Left, &isPressed, [&]()
			{
				bool isInteracted = false;
				previousOption = option;
				for(std::shared_ptr<Shape> shape : paintShapes)
				{
					if(shape->interact(cursor->body))
					{
						isInteracted = true;
						if(option == Option::OPEN_FILE)
						{
							commands.reset();
							load();
						}
						else if(option == Option::SAVE_FILE)
						{
							save();
						}
						else if(option == Option::UNDO)
						{
							if(commands.get() != NULL)
							{
								if(!commands.isBegin())
								{
									commands.get()->undo();
									commands--;
								}
							}
						}
						else if(option == Option::REDO)
						{
							if(commands.get() != NULL)
							{
								if(!commands.isEnd())
								{
									commands++;
									commands.get()->redo();
								}
							}
						}
						else if(option == Option::HELP)
						{
							// just open a dialogue box saying 'I believe in you; you'll figure it out';
						}
						break;
					}
				}
				
				for(std::reverse_iterator shapeIterator = userShapes.rbegin(); shapeIterator != userShapes.rend(); ++shapeIterator)
				{
					if((*shapeIterator)->interact(cursor->body))
					{
						if(option == Option::MOVE)
						{
							if(isMovingShape)
							{
								commands.push_back( std::shared_ptr<ShapeCommand>
									(
										new MoveCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), (*shapeIterator)->body.getPosition(), oldPos)
									)
								);
							}
							isMovingShape = !isMovingShape;
							if(isMovingShape)
							{
								oldPos = (*shapeIterator)->body.getPosition();
							}
							selectedShape = (*shapeIterator);							
						}
						else if(option == Option::SUB_Z)
						{
							if((shapeIterator) != userShapes.rend()-1)
							{
								commands.push_back( std::shared_ptr<ShapeCommand>
									(
										new Z_indexCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), std::distance(shapeIterator, userShapes.rend()-1) - 1 )
									)
								);
								std::iter_swap(shapeIterator+1, shapeIterator);
							}
						}
						else if(option == Option::ADD_Z)
						{
							if((shapeIterator) != userShapes.rbegin())
							{
								commands.push_back( std::shared_ptr<ShapeCommand>
									(
										new Z_indexCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), std::distance(shapeIterator, userShapes.rend()-1) + 1 )
									)
								);
								std::iter_swap(shapeIterator-1, shapeIterator);
							}
						}
						else if(option == Option::RESIZE_SUB)
						{
							if(((*shapeIterator)->getSize().x - objectSizeStep) >= minObjectSize)
							{
								objectSize = (*shapeIterator)->getSize().x - objectSizeStep;
								(*shapeIterator)->setSize(objectSize);
								commands.push_back( std::shared_ptr<ShapeCommand>
									(
										new ResizeCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), objectSize, objectSize + objectSizeStep)
									)
								);
							}
						}
						else if(option == Option::RESIZE_ADD)
						{
							if(((*shapeIterator)->getSize().x + objectSizeStep) <= maxObjectSize)
							{
								objectSize = (*shapeIterator)->getSize().x + objectSizeStep;
								(*shapeIterator)->setSize(objectSize);
								commands.push_back( std::shared_ptr<ShapeCommand>
									(
										new ResizeCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), objectSize, objectSize - objectSizeStep)
									)
								);
							}
						}
						else if(option == Option::DELETE)
						{
							commands.push_back( std::shared_ptr<ShapeCommand>
								(
									new DeleteCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), (*shapeIterator)->getOrder() )
								)
							);
							userShapes.erase(std::next(shapeIterator).base());							
						}
						else if(option == Option::COLOR_OUTLINE)
						{
							commands.push_back( std::shared_ptr<ShapeCommand>
								(
									new ColorOutlineCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), color, (*shapeIterator)->body.getOutlineColor())
								)
							);
							(*shapeIterator)->setOutlineColor(color);
						}
						else if(option == Option::COLOR_FILL)
						{
							commands.push_back( std::shared_ptr<ShapeCommand>
								(
									new ColorFillCommand( &userShapes, std::distance(shapeIterator, userShapes.rend()-1), color, (*shapeIterator)->body.getFillColor())
								)
							);
							(*shapeIterator)->setFillColor(color);
						}
						break;
					}
				}				
				if(!isInteracted)
				{
					if( ((int)option >= (int)Option::CREATE_RECTANGLE) && ((int)option <= (int)Option::CREATE_TRIANGLE) && (sf::Mouse::getPosition(window).y > windowOffsetY))
					{
						std::vector<std::string> recipe;
						if (option == Option::CREATE_RECTANGLE)
						{
							recipe = {
								"RECTANGLE",
								std::to_string(sf::Mouse::getPosition(window).x),
								std::to_string(sf::Mouse::getPosition(window).y),
								std::to_string(objectSize),
								std::to_string(objectSize),
								Shape::getColor(color),
								Shape::getColor(color),
								Shape::getColor(color)
							};
						}
						else if(option == Option::CREATE_CIRCLE)
						{
							recipe = {
								"CIRCLE",
								std::to_string(sf::Mouse::getPosition(window).x),
								std::to_string(sf::Mouse::getPosition(window).y),
								std::to_string((int)(objectSize/2)),
								Shape::getColor(color),
								Shape::getColor(color),
								Shape::getColor(color)
							};
						}
						else if(option == Option::CREATE_TRIANGLE)
						{
							recipe = {
								"TRIANGLE",
								std::to_string(sf::Mouse::getPosition(window).x),
								std::to_string(sf::Mouse::getPosition(window).y),
								std::to_string((int)(objectSize/2)),
								Shape::getColor(color),
								Shape::getColor(color),
								Shape::getColor(color)
							};
						}
						userShapes.push_back(Factory::create(recipe));
						commands.push_back( std::shared_ptr<ShapeCommand>
							(
								new CreateCommand( &userShapes, recipe )
							)
						);
					}
				}
				else
				{
					if(option == Option::RESIZE_SUB)
					{
						if((objectSize - objectSizeStep) >= minObjectSize)
						{
							objectSize -= objectSizeStep;
						}
					}
					else if(option == Option::RESIZE_ADD)
					{
						if((objectSize + objectSizeStep) <= maxObjectSize)
						{
							objectSize += objectSizeStep;
						}
					}
				}
				
			})
		);
		actions.push_back(
			Action( [&]()
			{
				for(std::shared_ptr<Shape> shape : paintShapes)
				{
					shape->update();
				}
				for(std::shared_ptr<Shape> shape : userShapes)
				{
					shape->update();
				}
				if(isMovingShape)
				{
					selectedShape->body.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y + (selectedShape->getSize().y / 2)));
				}
			})
		);
	}
	catch(const std::exception & exception)
	{
		printf("Exception : %s\n", &exception.what()[0]);
	}
	catch( ... )
	{
		printf("Something went wrong while adding Actions\n");
	}
}

sf::Vector2i
Paint::getTextureCoords(Option option)
{
	if(option == Option::CREATE_RECTANGLE)
		return sf::Vector2i(5, 0);
	else if(option == Option::CREATE_CIRCLE)
		return sf::Vector2i(6, 0);
	else if(option == Option::CREATE_TRIANGLE)
		return sf::Vector2i(7, 0);
	else if(option == Option::MOVE)
		return sf::Vector2i(0, 1);
	else if(option == Option::SUB_Z)
		return sf::Vector2i(1, 1);
	else if(option == Option::ADD_Z)
		return sf::Vector2i(2, 1);
	else if(option == Option::RESIZE_SUB)
		return sf::Vector2i(3, 1);
	else if(option == Option::RESIZE_ADD)
		return sf::Vector2i(4, 1);
	else if(option == Option::DELETE)
		return sf::Vector2i(5, 1);
	else if(option == Option::COLOR_OUTLINE)
		return sf::Vector2i(6, 1);
	return sf::Vector2i(7, 1);
}