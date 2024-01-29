#pragma once
#include <SFML/Graphics.hpp>

class ComponentGraphics
{
public:
	ComponentGraphics()
		:loaded(false), path("")
	{ }
	ComponentGraphics(std::string path)
		:loaded(false), path(path)
	{ }

	bool loadComponent()
	{
		return componentTexture.loadFromFile(path);
	}
	const sf::Texture& getTexture()
	{
		return componentTexture;
	}
	const std::string getPath()
	{
		return path;
	}

private:
	sf::Texture componentTexture;
	bool loaded;
	std::string path;
};