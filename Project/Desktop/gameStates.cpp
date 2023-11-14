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
	buttons[ButtonId::Login] = new Button({200,50}, {200,350}, res->GetFont(), L"Zaloguj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Powrót", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	loginInputBox = new InputBox({ 200,50 }, { 200, 230}, res->GetFont(), L"Login", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
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

void LoginMenu::AddLetter(wchar_t s)
{
	loginInputBox->AddLetter(s);
	passwordInputBox->AddLetter(s);
}

RegisterMenu::RegisterMenu(Resources* res)
	:res(res)
{
	buttons = new Button * [ButtonId::Count];
	buttons[ButtonId::Register] = new Button({ 200,50 }, { 200,350 }, res->GetFont(), L"Zarejestruj", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	buttons[ButtonId::Back] = new Button({ 200,50 }, { 200,420 }, res->GetFont(), L"Powrót", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

	loginInputBox = new InputBox * [InputBoxId::CountI];
	loginInputBox[InputBoxId::Login] = new InputBox({ 200,50 }, { 200, 110 }, res->GetFont(), L"Login", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	loginInputBox[InputBoxId::Email] = new InputBox({200,50}, {200, 170}, res->GetFont(), L"Email", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	
	passwordInputBox = new InputBoxPassword * [PasswordInputBoxId::CountP];
	passwordInputBox[PasswordInputBoxId::First] = new InputBoxPassword({ 200,50 }, { 200, 230 }, res->GetFont(), L"Password", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));
	passwordInputBox[PasswordInputBoxId::Second] = new InputBoxPassword({200,50}, {200, 290}, res->GetFont(), L"Password", Color(255, 0, 0, 255), Color(249, 0, 110, 255), Color(150, 0, 0, 255));

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

LevelSelect::LevelSelect(Resources* res)
	:res(res)
{
	//if logged
	//	get data
	//Load all levels

	backButton = new Button({ 200,50 }, { 5,5 }, res->GetFont(), L"Powrót");

	loadSaveButton = new Button({});
	startNewSaveButton = new Button();

	levelsCount = 2;
	levels = new Level * [2];
	levels[0] = new Level(L"T0", L"Test0", "Testowy opis blaaaaaaaaaaaaaaaaaawdawdawd\ndawdawdawd\nawd", false);
	levels[1] = new Level(L"T2", L"Test1", "Testowy opawdawdawdawdawis", false);
	Level** tmp = new Level * [1];
	tmp[0] = levels[0];
	levels[1]->setPrevLevels(tmp, 1);

	levels[0]->setGenerateComponents([](int* componentsCount)->Component**{
		//Tu kontynuuj jutro
		Component** components = new Component * [4];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentType::SMD);
		components[1] = new Component(L"Kondensator", L"Kumuluje ³adunek elektryczny", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getCapacitorTexture(), Component::ComponentType::SMD);
		components[2] = new Component(L"Dioda", L"Pr¹d p³ynie w jedn¹ stronê", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentType::SMD);
		delete[] tmp;
		*componentsCount = 3;
		return components;
		});


	levelButtons = new Button * [levelsCount];
	levelButtons[0] = new Button({ 200,70 }, { 10,100 }, res->GetFont(), levels[0]->getName());
	levelButtons[1] = new Button({ 200,70 }, { 250,100 }, res->GetFont(), levels[1]->getName());


	const Vector2i& windowSize = Config::getInstance()->getWindowDimension();
	levelInfoState = LevelInfoState::Hidden;
	levelInfoBox = new RectangleShape({ 7.f * windowSize.x / 16.f,8.f * windowSize.y / 9.f });

	levelInfoBox->setPosition(windowSize.x - 7.f * windowSize.x / 16.f - 50.f, 50.f);

	int buttonOffset = 5;
	loadSaveButton = new Button(
		{ 2 * (levelInfoBox->getSize().x - (buttonOffset * 3)) / 5, 50 },
		{ levelInfoBox->getPosition().x + buttonOffset, levelInfoBox->getSize().y-buttonOffset},
		res->GetFont(),
		L"Wczytaj zapis"
	);
	startNewSaveButton = new Button(
		{ 3 * (levelInfoBox->getSize().x - (buttonOffset * 3)) / 5, 50 },
		{ levelInfoBox->getPosition().x + loadSaveButton->getSize().x + 2 * buttonOffset, levelInfoBox->getSize().y - buttonOffset },
		res->GetFont(),
		L"Rozpocznij od nowa"
	);
	levelTitle = new TextBox(
		{ levelInfoBox->getSize().x - 10, 70 },
		{ levelInfoBox->getPosition().x + 5, levelInfoBox->getPosition().y + 5 },
		res->GetFont(),
		L"Title"
	);
	levelDesc = new TextBox(
		{ levelInfoBox->getSize().x - 20, 8*levelInfoBox->getSize().y/ 9 - 100 },
		{ levelInfoBox->getPosition().x + 10, levelInfoBox->getPosition().y + 100 },
		res->GetFont(),
		L"Title"
	);
}

LevelSelect::~LevelSelect()
{
	for (int i = 0; i < levelsCount; i++)
	{
		if (levels[i] == startLevel)
			continue;
		delete levels[i];
	}
	delete[] levels;

	for (int i = 0; i < levelsCount; i++)
		delete levelButtons[i];
	delete[] levelButtons;

	delete backButton;

	delete loadSaveButton;
	delete startNewSaveButton;
	delete levelInfoBox;

	delete levelTitle;
	delete levelDesc;
}

void LevelSelect::Update(sf::RenderWindow* window, sf::Time* elapsed)
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
	
	backButton->Update(mousePos);
	if (backButton->isButtonPressed())
		nextState = new StartMenu(res);

	if (levelInfoState == LevelInfoState::Active)
	{
		loadSaveButton->Update(mousePos);
		startNewSaveButton->Update(mousePos);
		if (startNewSaveButton->isButtonPressed())
		{
			
			nextState = new MainGame(res, startLevel);
		}

		if (!levelInfoBox->getGlobalBounds().contains(mousePos))
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				levelInfoState = LevelInfoState::Hidden;

		return;
	}

	for (int i = 0; i < levelsCount; i++)
	{
		levelButtons[i]->Update(mousePos);
		if (!levels[i]->canRealize())
			continue;
		if (levelButtons[i]->GetButtonState() == ButtonStates::HOVER)
		{
			levelInfoState = LevelInfoState::Showed;
			levelTitle->SetString(levels[i]->getName());
			levelDesc->SetString(levels[i]->getDesc());
			break;
		}
		else if (levelButtons[i]->GetButtonState() == ButtonStates::PRESSED)
		{
			levelInfoState = LevelInfoState::Active;
			startLevel = levels[i];
			/*levelTitle->SetString(levels[i]->getName());
			levelDesc->SetString(levels[i]->getDesc());*/
		}
		else
		{
			if (levelInfoState != LevelInfoState::Active)
				levelInfoState = LevelInfoState::Hidden;
		}
	}
}

void LevelSelect::Render(sf::RenderTarget* target)
{
	for (int i = 0; i < levelsCount; i++)
		levelButtons[i]->Render(target);

	backButton->Render(target);

	if (levelInfoState >= LevelInfoState::Showed )//|| levelInfoState == LevelInfoState::Showed
	{
		target->draw(*levelInfoBox);
		loadSaveButton->Render(target);
		startNewSaveButton->Render(target);
		levelTitle->Render(target);
		levelDesc->Render(target);
	}
}
