#include "stateStartMenu.h"

//------StartMenu---------
StartMenu::StartMenu(Resources* res, bool logged)
	:res(res)
{

	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Start] = new Button({ 200,50 }, { 200,240 }, res->GetFont(), L"Rozpocznij");
	buttons[ButtonId::Login] = new Button({ 200,50 }, { 200,360 }, res->GetFont(), L"Zaloguj");
	buttons[ButtonId::Logout] = new Button({ 200,50 }, { 200,300 }, res->GetFont(), L"Wyloguj");
	buttons[ButtonId::Register] = new Button({ 200,50 }, { 200,300 }, res->GetFont(), L"Zarejestruj");
	buttons[ButtonId::Exit] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Wyjœcie");

	//if logged sombody
	if (logged)
	{
		updateF = &StartMenu::updateLogged;
		renderF = &StartMenu::renderLogged;
		User::getInstance().getSavesFile();
	}
	else
	{
		updateF = &StartMenu::updateNotLogged;
		renderF = &StartMenu::renderNotLogged;
	}
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
	target->draw(res->GetDemonstrationSprite());
	(this->*renderF)(target);
}

void StartMenu::updateLogged(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(*window));

	buttons[ButtonId::Start]->Update(mousePos);
	if (buttons[ButtonId::Start]->isButtonPressed())
		nextState = new LevelSelect(res);

	buttons[ButtonId::Logout]->Update(mousePos);
	if (buttons[ButtonId::Logout]->isButtonPressed())
	{
		User::getInstance().logout();

		updateF = &StartMenu::updateNotLogged;//to nie tu bedzie (new) czemu?
		renderF = &StartMenu::renderNotLogged;//to nie tu bedzie (new) czemu?
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
		nextState = new LevelSelect(res);

	buttons[ButtonId::Login]->Update(mousePos);
	if (buttons[ButtonId::Login]->isButtonPressed())
		nextState = new LoginMenu(res);

	buttons[ButtonId::Register]->Update(mousePos);
	if (buttons[ButtonId::Register]->isButtonPressed())
	{
		nextState = new RegisterMenu(res);
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