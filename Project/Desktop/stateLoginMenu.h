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


class LoginMenu : public State
{
public:
	LoginMenu() = delete;
	LoginMenu(Resources* res);
	~LoginMenu();

	void Update(RenderWindow* window, Time* elapsed);
	void Render(RenderTarget* target);
	void AddLetter(wchar_t s);
private:
	Button** buttons;
	InputBox* loginInputBox;
	InputBoxPassword* passwordInputBox;

	TextBox** textBoxes;

	bool showLoginInfo;
	TextBox* loginInfo;

	enum TextBoxId
	{
		LoginT,
		PasswordT,
		CountT
	};

	enum ButtonId
	{
		Login,
		Back,
		Count
	};
};