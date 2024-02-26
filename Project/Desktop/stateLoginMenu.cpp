#include "stateLoginMenu.h"

//-------------------------
//-------LoginMenu---------
//-------------------------
LoginMenu::LoginMenu(Resources* res)
	:State(res)
{
	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Login] = new Button({ 200,50 }, { 200,450 }, res->GetFont(), L"Zaloguj", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,510 }, res->GetFont(), L"Powr�t", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));

	loginInputBox = new InputBox({ 200,50 }, { 200, 230 }, res->GetFont(), L"...", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));
	passwordInputBox = new InputBoxPassword({ 200,50 }, { 200, 290 }, res->GetFont(), L"...", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));

	textBoxes = new TextBox*[TextBoxId::CountT];
	textBoxes[TextBoxId::LoginT] = new TextBox({ 80,50 }, { 100,230 }, res->GetFont(), L"Loign:", Color(79, 199, 79, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	textBoxes[TextBoxId::PasswordT] = new TextBox({ 80,50 }, { 100,290 }, res->GetFont(), L"Has�o:", Color(79, 199, 79, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));


	showLoginInfo = false;
	loginInfo = new TextBox({ 300,50 }, { 100,350 }, res->GetFont(), "", Color(79, 199, 79, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255), 25);
}

LoginMenu::~LoginMenu()
{
	for (int i = 0; i < ButtonId::Count; i++)
		delete buttons[i];
	delete[] buttons;

	delete loginInputBox;
	delete passwordInputBox;

	for (int i = 0; i < TextBoxId::CountT; i++)
		delete textBoxes[i];
	delete[] textBoxes;

	delete loginInfo;
}

void LoginMenu::Update(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));

	loginInputBox->Update(mousePos);
	passwordInputBox->Update(mousePos);

	buttons[ButtonId::Back]->Update(mousePos);
	if (buttons[ButtonId::Back]->isButtonPressed())
		nextState = new StartMenu(res);

	buttons[ButtonId::Login]->Update(mousePos);
	if (buttons[ButtonId::Login]->isButtonPressed())
	{
		//TODO change min length of login and pass to 8
		if (loginInputBox->GetTypedString().getSize() == 0)
		{
			loginInfo->SetString(L"Wprowad� login");
			showLoginInfo = true;
			return;
		}
		
		if (passwordInputBox->GetTypedString().getSize() == 0)
		{
			loginInfo->SetString(L"Wprowad� has�o");
			showLoginInfo = true;
			return;
		}

		if (User::getInstance().loginExistingUser(
			loginInputBox->GetTypedString(),
			passwordInputBox->GetTypedString()))
		{
			nextState = new StartMenu(res);
		}
		else
		{
			loginInfo->SetString(L"B��dne dane logowania");
			showLoginInfo = true;
		}
	}
}

void LoginMenu::Render(RenderTarget* target)
{
	target->draw(res->GetDemonstrationSprite()); 
	
	Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition());
	for (int i = 0; i < ButtonId::Count; i++)
		buttons[i]->Render(target);

	loginInputBox->Render(target);
	passwordInputBox->Render(target);

	if (showLoginInfo)
		loginInfo->Render(target);

	for (int i = 0; i < TextBoxId::CountT; i++)
		textBoxes[i]->Render(target);
}

void LoginMenu::AddLetter(wchar_t s)
{
	loginInputBox->AddLetter(s);
	passwordInputBox->AddLetter(s);
}