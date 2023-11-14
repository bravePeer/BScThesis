#include "level.h"

Level::Level()
{
	id = "";
	name = "";
	desc = "";

	realized = false;

	nextLevels = nullptr;
	nextLevelsCount = 0;

	prevLevels = nullptr;
	prevLevelsCount = 0;
}

Level::Level(sf::String id, sf::String name, sf::String desc, bool realized)
	:id(id), name(name), desc(desc), realized(realized), nextLevelsCount(0), prevLevelsCount(0)
{
	nextLevels = nullptr;
	prevLevels = nullptr;
}

Level::~Level()
{
	delete[] nextLevels;
	delete[] prevLevels;
}

sf::String& Level::getId()
{
	return id;
}

sf::String& Level::getName()
{
	return name;
}

sf::String& Level::getDesc()
{
	return desc;
}

bool Level::isRealized()
{
	return realized;
}

bool Level::canRealize()
{
	for (int i = 0; i < prevLevelsCount; i++)
		if (!prevLevels[i]->isRealized())
			return false;
	
	return true;
}

void Level::setPrevLevels(Level** levels, int levelsCount)
{
	prevLevels = levels;
	prevLevelsCount = levelsCount;
}

void Level::setGenerateComponents(std::function<Component** (int*)> genComponents)
{
	this->genComponents = genComponents;
}

Component** Level::getComponents()
{
	return components;
}

int Level::getComponentsCount()
{
	return componentCount;
}

void Level::setCheckSimulation(std::function<void(Board*)> checkSimulation)
{
	this->checkSimulation = checkSimulation;
}

void Level::load()
{
	components = genComponents(&componentCount);
}
