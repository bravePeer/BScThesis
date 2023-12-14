#pragma once
#include "SFML/Graphics.hpp"

class State
{
public:
	State() :nextState(nullptr) {}
	virtual ~State() {}
	virtual void AddLetter(wchar_t s) {}
	State* IsStateChanged()
	{
		return nextState;
	}
	virtual void Update(sf::RenderWindow* window, sf::Time* elapsed)
	{}
	virtual void Render(sf::RenderTarget* target)
	{}
protected:
	State* nextState;
};
