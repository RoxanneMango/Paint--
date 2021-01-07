#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>

#include "command.h"

template<typename T>
class Node
{
private:
	T data = NULL;			// Command
public:
	std::shared_ptr<Node> previous = NULL;
	std::shared_ptr<Node> next = NULL;
	
	Node(T data) : data(data)
	{}
	~Node()
	{}
	
	bool operator==(Node & other)
	{
		return ( (this->previous == other.previous) && (this->next == other.next) );
	}
	bool operator!=(Node & other)
	{
		return ( (this->previous != other.previous) || (this->next != other.next) );
	}
};

template<typename T>
class LinkedList
{
private:
	std::shared_ptr<Node<T>> head;
	std::shared_ptr<Node<T>> tail;
	
	std::shared_ptr<Node<T>> current;
	
	unsigned int minNum = 0;
	unsigned int maxNum = 0;
	unsigned int currentNum = 0;
public:
	LinkedList(unsigned int min, unsigned int max) : minNum(min), maxNum(max)
	{
		head = NULL;
		tail = NULL;
		current = NULL;
	}
	~LinkedList()
	{}
	
	template<typename D>
	void place_back(D data)			// command
	{
		if(currentNum >= maxNum)
		{
			printf("maxNum reached, cannot add more Nodes!\n");
			return;
		}
		
		std::shared_ptr<Node<T>> node = std::make_shared<Node>(data);
		
		if(head == NULL)
		{
			head = node;
			tail = node;
			node->previous = tail;
			node->next = head;
		}
		else
		{
			tail->next = tail;
			node->previous = tail;
			tail = node;
			node->next = head;
		}
	}
	
	std::shared_ptr<Node<T>> front() { return head; }
	std::shared_ptr<Node<T>> back() { return tail; }
	bool empty() { return currentNum; }
	int size() { return currentNum; }
	
	
	void clear()
	{
		while(head != tail)
		{
			pop_back();
		}
		head.reset(nullptr);
		tail.reset(nullptr);
	}
	
	std::shared_ptr<Node<T>> pop_back()
	{
		if(tail != NULL)
		{
			tail->previous = head;
			tail.reset(head->previous);
		}
	}
	
	std::shared_ptr<Node<T>> operator++()
	{		
		current->previous = current;
		current = current->next;
		current->next = current->next;
	}
	std::shared_ptr<Node<T>> operator--()
	{
		current->next = current;
		current = current->previous;
		current->previous = current->previous;
	}
};

#endif // LINKED_LIST_H