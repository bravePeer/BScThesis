#pragma once
#include <iostream>
#include "gui.h"
#include "board.h"

//class State
//{
//public:
//	State() {}
//	virtual ~State() {}
//	virtual void AddLetter(wchar_t s) {}
//	virtual State* IsStateChanged()
//	{
//		return nullptr;
//	}
//	virtual void Update(RenderWindow* window, Time* elapsed)
//	{}
//	virtual void Render(RenderTarget* target)
//	{}
//private:
//
//};

class StartMenu : public State
{
public:
	StartMenu();
	StartMenu(Resources* _res);
	~StartMenu();

	//Który przycisk wciœniêty
	short GetButtonPressed();
	State* IsStateChanged();

	Board* board;

	void Update(RenderWindow* window, Time* elapsed);
	void Render(RenderTarget* target);
private:

	/*Button** buttons;
	Resources* res;*/
	short buttonPressed;

	enum BUTTONSID
	{
		BUTTONLOGIN, BUTTONREGISTER, BUTTONSKIP, BUTTONEXIT, NUMBUTTONS
	};
};

StartMenu::StartMenu()
{
	//buttons = nullptr;
	//res = nullptr;
	buttonPressed = -1;
	board = new Board(10, 10, 1);
}
StartMenu::StartMenu(Resources* _res)
{
	//res = _res;
	//buttons = new Button * [NUMBUTTONS];//new Button({ 200,200 }, { 200,100 }, res, "butt1", Color(255, 0, 0, 255), Color(249, 0, 0, 255), Color(150, 0, 0, 255));
	//buttons[BUTTONLOGIN] = new Button({ 200,50 }, { 200,240 }, res->GetFont(), L"Zaloguj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	//buttons[BUTTONREGISTER] = new Button({ 200,50 }, { 200,300 }, res->GetFont(), L"Zarejestruj", Color(255, 0, 0, 255), Color(249, 110, 0, 255), Color(150, 0, 0, 255));
	//buttons[BUTTONSKIP] = new Button({ 200,50 }, { 200,360 }, res->GetFont(), L"Graj bez logowania", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	//buttons[BUTTONEXIT] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Wyjœcie", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttonPressed = -1;
}
StartMenu::~StartMenu()
{
	/*for (short i = 0; i < NUMBUTTONS; i++)
	{
		delete buttons[i];
	}
	delete[] buttons;*/
	delete board;
}

State* StartMenu::IsStateChanged()
{
	switch (buttonPressed)
	{
	case BUTTONLOGIN:
		//return new LoginMenu(res);
		break;
	case BUTTONREGISTER:
		//return new RegisterMenu(res);
		break;
	case BUTTONSKIP:
		//return new MainMenu(res);
		break;
	case BUTTONEXIT:
		//return new CloseMenu;
		break;
	}
	return nullptr;
}
short StartMenu::GetButtonPressed()
{
	switch (buttonPressed)
	{
	case BUTTONLOGIN:
		return LOGINMENU;
		break;
	case BUTTONREGISTER:
		return REGISTEMENU;
		break;
	case BUTTONSKIP:
		return MAINMENU;
		break;
	case BUTTONEXIT:
		return EXITGAME;
		break;
	}
	return STARTMENU;
}
void StartMenu::Update(RenderWindow* window, Time* elapsed)
{
	board->Update(window, elapsed);
	/*for (short i = 0; i < NUMBUTTONS; i++)
	{
		buttons[i]->Update(static_cast<Vector2f>(Mouse::getPosition(*window)));

		if (buttons[i]->GetButtonState() == PRESSED)
			buttonPressed = i;
	}*/
}
void StartMenu::Render(RenderTarget* target)
{
	board->Render(target);
	/*for (short i = 0; i < NUMBUTTONS; i++)
	{
		buttons[i]->Render(target);
	}*/
}

