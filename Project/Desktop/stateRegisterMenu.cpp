#include "stateRegisterMenu.h"

RegisterMenu::RegisterMenu(Resources* res)
	:State(res)
{
	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Register] = new Button({ 200,50 }, { 200,460 }, res->GetFont(), L"Zarejestruj", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,520 }, res->GetFont(), L"Powrót", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));

	loginInputBox = new InputBox * [InputBoxId::CountI];
	loginInputBox[InputBoxId::Login] = new InputBox({ 200,50 }, { 200, 110 }, res->GetFont(), L"...", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));
	loginInputBox[InputBoxId::Email] = new InputBox({ 200,50 }, { 200, 170 }, res->GetFont(), L"...", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));

	passwordInputBox = new InputBoxPassword * [PasswordInputBoxId::CountP];
	passwordInputBox[PasswordInputBoxId::First] = new InputBoxPassword({ 200,50 }, { 200, 230 }, res->GetFont(), L"...", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));
	passwordInputBox[PasswordInputBoxId::Second] = new InputBoxPassword({ 200,50 }, { 200, 290 }, res->GetFont(), L"...", Color(79, 199, 79, 255), Color(38, 173, 38, 255), Color(31, 146, 31, 255));

	textBoxes = new TextBox * [TextBoxId::CountT];
	textBoxes[TextBoxId::LoginT] = new TextBox({ 80,50 }, { 100,110 }, res->GetFont(), L"Loign:", sf::Color(255, 0, 0, 0));
	textBoxes[TextBoxId::EmailT] = new TextBox({ 80,50 }, { 100,170 }, res->GetFont(), L"Email:", sf::Color(255, 0, 0, 0));
	textBoxes[TextBoxId::PasswordT] = new TextBox({ 80,50 }, { 100,230 }, res->GetFont(), L"Has³o:", sf::Color(255, 0, 0, 0));
	textBoxes[TextBoxId::RePasswordT] = new TextBox({ 100,50 }, { 70,290 }, res->GetFont(), L"Powtórz Has³o:", sf::Color(255, 0, 0, 0));

	showRegisterInfo = false;
	registerInfo = new TextBox({ 300,100 }, { 100,350 }, res->GetFont(), "", Color(79, 199, 79, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255), 20);
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

	for (int i = 0; i < TextBoxId::CountT; i++)
		delete textBoxes[i];
	delete[] textBoxes;
}

void RegisterMenu::Update(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = Vector2f(sf::Mouse::getPosition(*window));

	passwordInputBox[PasswordInputBoxId::First]->Update(mousePos);
	passwordInputBox[PasswordInputBoxId::Second]->Update(mousePos);

	loginInputBox[InputBoxId::Login]->Update(mousePos);
	loginInputBox[InputBoxId::Email]->Update(mousePos);

	buttons[ButtonId::Back]->Update(mousePos);
	if (buttons[ButtonId::Back]->isButtonPressed())
		nextState = new StartMenu(res);

	buttons[ButtonId::Register]->Update(mousePos);
	if (buttons[ButtonId::Register]->isButtonPressed())
	{
		if (loginInputBox[InputBoxId::Login]->GetTypedString().getSize() == 0)
		{
			registerInfo->SetString(L"Login musi sk³adaæ siê\n z co najmniej 8 znaków");
			showRegisterInfo = true;
			return;
		}

		//TODO change min length of login and pass to 8
		if (passwordInputBox[PasswordInputBoxId::First]->GetTypedString().getSize() == 0)
		{
			registerInfo->SetString(L"Has³o musi sk³adaæ siê\n z co najmniej 8 znaków");
			showRegisterInfo = true;
			return;
		}

		if (passwordInputBox[PasswordInputBoxId::First]->GetTypedString() != passwordInputBox[PasswordInputBoxId::Second]->GetTypedString())
		{
			registerInfo->SetString(L"Podane has³a nie s¹ zgodne!");
			showRegisterInfo = true;
			return;
		}

		if (User::getInstance().createNewUser(
			loginInputBox[InputBoxId::Login]->GetTypedString().toAnsiString(),
			passwordInputBox[PasswordInputBoxId::First]->GetTypedString().toAnsiString()))
		{
			registerInfo->SetString(L"Konto zosta³o utworzone,\nzaloguj siê");
			showRegisterInfo = true;
		}
		else
		{
			registerInfo->SetString(L"Loign jest zajêty");
			showRegisterInfo = true;
		}
	}
	;//TODO add register encryption
}

void RegisterMenu::Render(RenderTarget* target)
{
	target->draw(res->GetDemonstrationSprite()); 
	
	for (int i = 0; i < ButtonId::Count; i++)
		buttons[i]->Render(target);

	for (int i = 0; i < InputBoxId::CountI; i++)
		loginInputBox[i]->Render(target);

	for (int i = 0; i < PasswordInputBoxId::CountP; i++)
		passwordInputBox[i]->Render(target);

	for (int i = 0; i < TextBoxId::CountT; i++)
		textBoxes[i]->Render(target);

	if (showRegisterInfo)
		registerInfo->Render(target);
}

void RegisterMenu::AddLetter(wchar_t s)
{
	for (int i = 0; i < InputBoxId::CountI; i++)
		loginInputBox[i]->AddLetter(s);

	for (int i = 0; i < PasswordInputBoxId::CountP; i++)
		passwordInputBox[i]->AddLetter(s);
}