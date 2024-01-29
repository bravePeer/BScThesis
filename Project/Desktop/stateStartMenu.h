#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "gui.h"
#include "board.h"
#include "level.h"

#include "state.h"
#include "stateCloseMenu.h"
#include "stateLevelSelect.h"
#include "stateLoginMenu.h"
#include "stateRegisterMenu.h"
#include "stateStartMenu.h"

class StartMenu : public State
{
public:
	StartMenu() = delete;
	StartMenu(Resources* res, bool logged = false);
	~StartMenu();

	//Który przycisk wciœniêty
	//short GetButtonPressed();
	//State* GetNextState();


	void Update(RenderWindow* window, Time* elapsed);
	void Render(RenderTarget* target);
private:

	void (StartMenu::* updateF)(RenderWindow* window, Time* elapsed);
	void (StartMenu::* renderF)(RenderTarget* window);

	void updateLogged(RenderWindow* window, Time* elapsed);
	void renderLogged(RenderTarget* window);
	void updateNotLogged(RenderWindow* window, Time* elapsed);
	void renderNotLogged(RenderTarget* window);

	Button** buttons;

	enum ButtonId
	{
		Start,
		Login,
		Register,
		Logout,
		Exit,
		//Last to count
		Count
	};
};
