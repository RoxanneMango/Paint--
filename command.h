#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <functional>
#include <string>
#include <vector>

#include "factory.h"

template<typename T>
class Command
{
public:
	int index;
	std::vector<T> * reactor;

	Command<T>(std::vector<T> * reactor, unsigned int index) : reactor(reactor), index(index)
	{}
	virtual ~Command()
	{}
};

class ShapeCommand : public Command<std::shared_ptr<Shape>>
{
public:
	ShapeCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index) : Command<std::shared_ptr<Shape>>(reactor, index)
	{}
	virtual ~ShapeCommand()
	{}
	
	virtual void undo() = 0;
	virtual void redo() = 0;
};

class CreateCommand : public ShapeCommand
{
private:
	std::vector<std::string> order;
public:
	CreateCommand(std::vector<std::shared_ptr<Shape>> * reactor, std::vector<std::string> order) : 
		ShapeCommand(reactor, 0), order(order)
	{}
	virtual ~CreateCommand()
	{}
	void undo() override { reactor->pop_back(); }
	void redo() override { reactor->push_back(Factory::create(order)); }
};

class DeleteCommand : public ShapeCommand
{
private:
	std::vector<std::string> order;
public:
	DeleteCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index, std::vector<std::string> order) : 
		ShapeCommand(reactor, index), order(order)
	{}
	virtual ~DeleteCommand()
	{}
	void undo() override { reactor->insert(reactor->begin() + index, Factory::create(order)); }
	void redo() override { reactor->erase(reactor->begin() + index); }
};

class MoveCommand : public ShapeCommand
{
	sf::Vector2f pos;
	sf::Vector2f previousPos;
public:
	MoveCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index, sf::Vector2f pos, sf::Vector2f previousPos) : 
		ShapeCommand(reactor, index), pos(pos), previousPos(previousPos)
	{}
	virtual ~MoveCommand()
	{}
	void undo() override { (*reactor)[index]->body.setPosition(previousPos); }
	void redo() override { (*reactor)[index]->body.setPosition(pos); }
};

class ResizeCommand : public ShapeCommand
{
	double size;
	double previousSize;
public:
	ResizeCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index, double size, double previousSize) : 
		ShapeCommand(reactor, index), size(size), previousSize(previousSize)
	{}
	virtual ~ResizeCommand()
	{}
	void undo() override { (*reactor)[index]->setSize(previousSize); }
	void redo() override { (*reactor)[index]->setSize(size); }
};

class ColorOutlineCommand : public ShapeCommand
{
	sf::Color color;
	sf::Color previousColor;
public:
	ColorOutlineCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index, sf::Color color, sf::Color previousColor) : 
		ShapeCommand(reactor, index), color(color), previousColor(previousColor)
	{}
	virtual ~ColorOutlineCommand()
	{}
	void undo() override { (*reactor)[index]->setOutlineColor(previousColor); }
	void redo() override { (*reactor)[index]->setOutlineColor(color); }
};

class ColorFillCommand : public ShapeCommand
{
	sf::Color color;
	sf::Color previousColor;
public:
	ColorFillCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index, sf::Color color, sf::Color previousColor) : 
		ShapeCommand(reactor, index), color(color), previousColor(previousColor)
	{}
	virtual ~ColorFillCommand()
	{}
	void undo() override { (*reactor)[index]->setFillColor(previousColor); }
	void redo() override { (*reactor)[index]->setFillColor(color); }
};

class Z_indexCommand : public ShapeCommand
{
private:
	unsigned int previousIndex;
public:
	Z_indexCommand(std::vector<std::shared_ptr<Shape>> * reactor, unsigned int index, unsigned int previousIndex) : 
		ShapeCommand(reactor, index), previousIndex(previousIndex)
	{}
	virtual ~Z_indexCommand()
	{}
	void undo() override { std::swap((*reactor)[previousIndex], (*reactor)[index]); }
	void redo() override { std::swap((*reactor)[index], (*reactor)[previousIndex]); }
};

#endif // LINKED_LIST_H