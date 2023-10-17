#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "component.h"

class Level
{
public:
	Level();
	Level(sf::String id, sf::String name, sf::String desc, bool realized);
	~Level();
	
	sf::String& getId();
	sf::String& getName();
	sf::String& getDesc();
	
	bool isRealized();
	bool canRealize();

	//void setNextLevels(Level** levels); //moze nie potrzebne
	void setPrevLevels(Level** levels, int levelsCount);
	void setGenerateComponents(std::function<Component**(int*)> genComponents);
	Component** getComponents();

	void load(); //load graphics
private:
	sf::String id;
	sf::String name;
	sf::String desc;

	bool realized;

	Level** nextLevels;
	int nextLevelsCount;

	Level** prevLevels;
	int prevLevelsCount;

	std::function<Component** (int*)> genComponents;
	Component** components;
	int componentCount;
};