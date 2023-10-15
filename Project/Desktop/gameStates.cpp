//#pragma once
#include "gameStates.h"
#include "gameEngine.h"

//------StartMenu---------
StartMenu::StartMenu(Resources* res, bool loged)
	:res(res)
{

	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Start] = new Button({ 200,50 }, { 200,240 }, res->GetFont(), L"Rozpocznij", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Login] = new Button({ 200,50 }, { 200,360 }, res->GetFont(), L"Zaloguj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Logout] = new Button({ 200,50 }, { 200,300 }, res->GetFont(), L"Wyloguj", Color(255, 0, 0, 255), Color(249, 110, 0, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Register] = new Button({ 200,50 }, { 200,300 }, res->GetFont(), L"Zarejestruj", Color(255, 0, 0, 255), Color(249, 110, 0, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Exit] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Wyjœcie", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	//if logged sombody
	updateF = &StartMenu::updateNotLogged;
	renderF = &StartMenu::renderNotLogged;
}
StartMenu::~StartMenu()
{
	for (short i = 0; i < ButtonId::Count; i++)
	{
		delete buttons[i];
	}
	delete[] buttons;
}

void StartMenu::Update(RenderWindow* window, Time* elapsed)
{
	(this->*updateF)(window, elapsed);
}

void StartMenu::Render(RenderTarget* target)
{
	(this->*renderF)(target);
}

void StartMenu::updateLogged(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(*window));

	buttons[ButtonId::Start]->Update(mousePos);
	if (buttons[ButtonId::Start]->isButtonPressed())
		;

	buttons[ButtonId::Logout]->Update(mousePos);
	if (buttons[ButtonId::Logout]->isButtonPressed())
	{
		//TODO Logout stufff

		updateF = &StartMenu::updateNotLogged;//to nie tu bedzie
		renderF = &StartMenu::renderNotLogged;//to nie tu bedzie
	}
		

	buttons[ButtonId::Exit]->Update(mousePos);
	if (buttons[ButtonId::Exit]->isButtonPressed())
		nextState = new CloseMenu();
}

void StartMenu::renderLogged(RenderTarget* window)
{
	buttons[ButtonId::Start]->Render(window);
	buttons[ButtonId::Logout]->Render(window);
	buttons[ButtonId::Exit]->Render(window);
}

void StartMenu::updateNotLogged(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(*window));

	buttons[ButtonId::Start]->Update(mousePos);
	if (buttons[ButtonId::Start]->isButtonPressed())
		;

	buttons[ButtonId::Login]->Update(mousePos);
	if (buttons[ButtonId::Login]->isButtonPressed())
	{
		//TODO Login stufff
		//updateF = &StartMenu::updateLogged;//to nie tu bedzie
		//renderF = &StartMenu::renderLogged;//to nie tu bedzie
		nextState = new LoginMenu(res);
	}

	buttons[ButtonId::Register]->Update(mousePos);
	if (buttons[ButtonId::Register]->isButtonPressed())
	{
		//TODO register stufff
	}

	buttons[ButtonId::Exit]->Update(mousePos);
	if (buttons[ButtonId::Exit]->isButtonPressed())
		nextState = new CloseMenu();
}

void StartMenu::renderNotLogged(RenderTarget* window)
{
	buttons[ButtonId::Start]->Render(window);
	buttons[ButtonId::Login]->Render(window);
	buttons[ButtonId::Register]->Render(window);
	buttons[ButtonId::Exit]->Render(window);
}


//-------------------------
//------CloaseMenu---------
//-------------------------
void CloseMenu::Render(RenderTarget* target)
{
	dynamic_cast<RenderWindow*>(target)->close();
}

//-------------------------
//-------LoginMenu---------
//-------------------------
LoginMenu::LoginMenu(Resources* res)
	:res(res)
{
	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Login] = new Button({200,50}, {200,300}, res->GetFont(), L"Zaloguj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Powrót", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	loginInputBox = new InputBox({ 200,50 }, { 300, 240}, res->GetFont(), L"Login", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	passwordInputBox = new InputBoxPassword({ 200,50 }, { 400, 170 }, res->GetFont(), L"Password", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	//loginInputBox = new InputBox();
	//passwordInputBox = new InputBoxPassword();
	//auto lam = [](wchar_t w)->void {};
	//Game::addLetter = lam;
}

LoginMenu::~LoginMenu()
{
	for (int i = 0; i < ButtonId::Count; i++)
		delete buttons[i];
	delete[] buttons;

	delete loginInputBox;
	delete passwordInputBox;
}

void LoginMenu::Update(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
	
	loginInputBox->Update(mousePos);
	passwordInputBox->Update(mousePos);

	buttons[ButtonId::Login]->Update(mousePos);
	if (buttons[ButtonId::Login]->isButtonPressed())
		;//do login stuff

	buttons[ButtonId::Back]->Update(mousePos);
	if(buttons[ButtonId::Back]->isButtonPressed())
		nextState = new StartMenu(res);
}

void LoginMenu::Render(RenderTarget* target)
{
	Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition());
	for (int i = 0; i < ButtonId::Count; i++)
		buttons[i]->Render(target);

	loginInputBox->Render(target);
	passwordInputBox->Render(target);
}
