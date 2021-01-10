#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <memory>
#include <vector>

#include "command.h"

class CommandList
{
private:
	std::vector<std::shared_ptr<ShapeCommand>> list;
	
	unsigned int max;
	unsigned int size;

	int index;
	int num;
	
	bool isDecremented;
	
public:
	
	CommandList(unsigned int max) : list(max+1, NULL), max(max), size(0), index(0), num(0), isDecremented(false)
	{}
	~CommandList()
	{
		reset();
	}
	
	std::shared_ptr<ShapeCommand> get()
	{ 
		return list[index ? index-1 : 0];
	}
	
	void push_back(std::shared_ptr<ShapeCommand> command)
	{
		if(isDecremented)
		{
			isDecremented = false;
			size = 0;
			index = 0;
			num = 0;
		}
		
		if( (index >= max) && (index >= size) )
		{
			index = 0;
		}
		
		list[index] = command;
		
		if(size < max)
		{
			size += 1;
			index = size;
			num = size;
		}
		else
		{
			if(index < size)
			{
				index += 1;
			}
			if(num < max)
			{
				num += 1;
			}
		}
	}
	
	void reset()
	{
		for(std::shared_ptr<ShapeCommand> & command : list)
		{
			command = NULL;
		}
		isDecremented = false;
		size = 0;
		index = 0;
		num = 0;
	}
	
	bool isBegin()
	{ 
		return num <= 0; 
	}
	bool isEnd()
	{
		return num >= size;
	}
	
	int operator++(int)
	{
		index = index < size ? index+1 : 1;
		num++;
		return index;
	}
	int operator--(int)
	{
		isDecremented = true;
		index = index ? index-1 : size-1;
		num--;
		return index;
	}
};

#endif // COMMAND_LIST_H