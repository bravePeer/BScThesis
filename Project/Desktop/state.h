#pragma once
#include "SFML/Graphics.hpp"

//Bazowy stan gry w gui.h póki co
class State
{
public:
	State() {}
	virtual ~State() {}
	virtual void AddLetter(wchar_t s) {}
	virtual State* IsStateChanged()
	{
		return nullptr;
	}
	virtual void Update(sf::RenderWindow* window, sf::Time* elapsed)
	{}
	virtual void Render(sf::RenderTarget* target)
	{}
private:

};

