#ifndef COMMAND_H
#define COMMAND_H

#include <memory>

#include "action.h"

template<typename T>
class Command
{
private:
	std::shared_ptr<T> actor;	// shape
	Action action;				// redo
	Action reaction;			// undo
public:
	Command(std::shared_ptr<T> actor, Action action, Action reaction) : actor(actor), action(action), reaction(reaction)
	{}
	~Command()
	{}
	
	void undo()
	{
		reaction(actor);
	}
	void redo()
	{
		action(actor);
	}
};

#endif // LINKED_LIST_H