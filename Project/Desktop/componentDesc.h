#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"
#include "graphics.h"
#include "gui.h"

#define WIDTH 700

class ComponentDesc 
{
public:
	ComponentDesc(std::string path = "")
		:path(path)
	{
		sf::FloatRect dump = FloatRect(0, 0, 1100, 700);

		shape.setFillColor(sf::Color(120, 200, 120, 200));
		shape.setSize({ dump.getSize().x - 20, dump.getSize().y - 20 });
		shape.setPosition({ 10,10 });
	}

	void setFont(sf::Font&font)
	{
		name.setFont(font);
	}
	void setName(sf::String name)
	{
		this->name.setString(name);
	}

	std::string getPath()
	{
		return path;
	}

	void setSprite(Texture& texture)
	{
		this->sprite.setTexture(texture);
		sprite.setPosition({ 10.0f,10.0f });
	}
	
	void Render(RenderTarget* target)
	{
		target->draw(sprite);
		//target->draw(name);
		//target->draw(shape);
	}
private:
	std::string path;

	sf::Text name;
	sf::RectangleShape shape;
	sf::Sprite sprite;
};