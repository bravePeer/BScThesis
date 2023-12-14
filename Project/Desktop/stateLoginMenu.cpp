#include "stateLoginMenu.h"

//-------------------------
//-------LoginMenu---------
//-------------------------
LoginMenu::LoginMenu(Resources* res)
	:res(res)
{
	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Login] = new Button({ 200,50 }, { 200,350 }, res->GetFont(), L"Zaloguj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Powrót", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	loginInputBox = new InputBox({ 200,50 }, { 200, 230 }, res->GetFont(), L"Login", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	passwordInputBox = new InputBoxPassword({ 200,50 }, { 200, 290 }, res->GetFont(), L"Password", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
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
	if (buttons[ButtonId::Back]->isButtonPressed())
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

void LoginMenu::AddLetter(wchar_t s)
{
	loginInputBox->AddLetter(s);
	passwordInputBox->AddLetter(s);
}