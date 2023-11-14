#include "stateRegisterMenu.h"

RegisterMenu::RegisterMenu(Resources* res)
	:res(res)
{
	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Register] = new Button({ 200,50 }, { 200,350 }, res->GetFont(), L"Zarejestruj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Powrót", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	loginInputBox = new InputBox * [InputBoxId::CountI];
	loginInputBox[InputBoxId::Login] = new InputBox({ 200,50 }, { 200, 110 }, res->GetFont(), L"Login", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	loginInputBox[InputBoxId::Email] = new InputBox({ 200,50 }, { 200, 170 }, res->GetFont(), L"Email", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	passwordInputBox = new InputBoxPassword * [PasswordInputBoxId::CountP];
	passwordInputBox[PasswordInputBoxId::First] = new InputBoxPassword({ 200,50 }, { 200, 230 }, res->GetFont(), L"Password", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	passwordInputBox[PasswordInputBoxId::Second] = new InputBoxPassword({ 200,50 }, { 200, 290 }, res->GetFont(), L"Password", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

}

RegisterMenu::~RegisterMenu()
{
	for (int i = 0; i < ButtonId::Count; i++)
		delete buttons[i];
	delete[] buttons;

	for (int i = 0; i < InputBoxId::CountI; i++)
		delete loginInputBox[i];
	delete[] loginInputBox;

	for (int i = 0; i < PasswordInputBoxId::CountP; i++)
		delete passwordInputBox[i];
	delete[] passwordInputBox;
}

void RegisterMenu::Update(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = Vector2f(sf::Mouse::getPosition(*window));

	buttons[ButtonId::Register]->Update(mousePos);
	if (buttons[ButtonId::Register]->isButtonPressed())
		;//TODO register stuff

	buttons[ButtonId::Back]->Update(mousePos);
	if (buttons[ButtonId::Back]->isButtonPressed())
		nextState = new StartMenu(res);

	passwordInputBox[PasswordInputBoxId::First]->Update(mousePos);
	passwordInputBox[PasswordInputBoxId::Second]->Update(mousePos);

	loginInputBox[InputBoxId::Login]->Update(mousePos);
	loginInputBox[InputBoxId::Email]->Update(mousePos);
}

void RegisterMenu::Render(RenderTarget* target)
{
	for (int i = 0; i < ButtonId::Count; i++)
		buttons[i]->Render(target);

	for (int i = 0; i < InputBoxId::CountI; i++)
		loginInputBox[i]->Render(target);

	for (int i = 0; i < PasswordInputBoxId::CountP; i++)
		passwordInputBox[i]->Render(target);
}

void RegisterMenu::AddLetter(wchar_t s)
{
	for (int i = 0; i < InputBoxId::CountI; i++)
		loginInputBox[i]->AddLetter(s);

	for (int i = 0; i < PasswordInputBoxId::CountP; i++)
		passwordInputBox[i]->AddLetter(s);
}