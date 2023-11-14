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


class RegisterMenu : public State
{
public:
	RegisterMenu() = delete;
	RegisterMenu(Resources* res);
	~RegisterMenu();

	void Update(RenderWindow* window, Time* elapsed);
	void Render(RenderTarget* target);
	void AddLetter(wchar_t s);
private:
	Resources* res;
	Button** buttons;
	InputBox** loginInputBox;
	InputBoxPassword** passwordInputBox;

	enum ButtonId
	{
		Register,
		Back,
		Count
	};

	enum InputBoxId
	{
		Login,
		Email,
		CountI
	};

	enum PasswordInputBoxId
	{
		First,
		Second,
		CountP
	};
};
