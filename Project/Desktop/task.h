#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"

class Task
{
public:
	Task(sf::String name, sf::String description)
		:name(name), description(description)
	{ }
	~Task()
	{ }


	Component* getComponents()
	{
		return componentes;
	}
	int getComponentCount()
	{
		return componentCount;
	}
	sf::String& getName()
	{
		return name;
	}
	sf::String& getDescription()
	{
		return description;
	}
private:
	sf::String name;
	sf::String description;

	Component* componentes;
	int componentCount;
};
