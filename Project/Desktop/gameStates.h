#pragma once
#include <iostream>
#include "gui.h"
#include "board.h"
#include "state.h"


class StartMenu : public State
{
public:
	StartMenu() = delete;
	StartMenu(Resources* _res, bool loged = false);
	~StartMenu();

	//Który przycisk wciœniêty
	//short GetButtonPressed();
	//State* IsStateChanged();


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
	Resources* res;

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

class CloseMenu : public State
{
public:
	void Render(RenderTarget* target);
private:
};

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
	Resources* res;
	Button** buttons;
	InputBox* loginInputBox;
	InputBoxPassword* passwordInputBox;

	enum ButtonId 
	{
		Login,
		Back,
		Count
	};
};

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