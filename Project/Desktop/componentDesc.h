#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"
#include "graphics.h"
#include "gui.h"

#define WIDTH 700


class ComponentDesc
{
public:
	ComponentDesc() = delete;
	ComponentDesc(Resources*res)
	{ 
		shape.setFillColor(sf::Color(0, 255, 0, 100));
		shape.setSize({ 600, 400 });
		shouldRender = false;
		name.setFont(res->getFont());
		desc.setFont(res->getFont());
		type.setFont(res->getFont());
		mountType.setFont(res->getFont());
	}

	void setComponent(Component* component)
	{
		name.setString(component->getName());
	}

	void setRender(bool shouldRender)
	{
		this->shouldRender = shouldRender;
	}

	void Render(RenderTarget* target)
	{
		if (!shouldRender)
			return;

		target->draw(shape);
		target->draw(name);
	}

private:
	sf::RectangleShape shape;
	sf::Text name;
	sf::Text desc;
	sf::Text mountType;
	sf::Text type;

	bool shouldRender;
};