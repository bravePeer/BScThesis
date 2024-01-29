#include "stateLevelSelect.h"

LevelSelect::LevelSelect(Resources* res)
	:State(res)
{
	//if logged
	//	get data
	//Load all levels
	

	backButton = new Button({ 200,50 }, { 5,5 }, res->GetFont(), L"Powrót");

	loadSaveButton = new Button({});
	startNewSaveButton = new Button();



	//Generating levels
	levelsMap.insert(std::pair<std::string, Level*>("WEL0", emptyLevel(L"EL0", L"Opór po³¹czenie\nszeregowe")));
	levelsMap.insert(std::pair<std::string, Level*>("WEL1", emptyLevel(L"EL1", L"Pojemnoœæ po³¹czenie\nszeregowe")));
	levelsMap.insert(std::pair<std::string, Level*>("WEL6", emptyLevel(L"EL2", L"Pojemnoœæ po³¹czenie\nrównoleg³e")));
	levelsMap.insert(std::pair<std::string, Level*>("WEL2", emptyLevel(L"EL3", L"Tranzystor\nbipolarny")));
	levelsMap.insert(std::pair<std::string, Level*>("WEL3", emptyLevel(L"EL4", L"Tranzystor\nMOSFET")));
	levelsMap.insert(std::pair<std::string, Level*>("WEL4", emptyLevel(L"EL5", L"Indukcyjnoœæ")));
	levelsMap.insert(std::pair<std::string, Level*>("WEL5", emptyLevel(L"EL6", L"Obwód RLC")));

	//TODO IMPORTANT dodaj coœ jeszcze
	levelsMap.insert(std::pair<std::string, Level*>("ZEL0", emptyLevel(L"Ea0", L"Stabilizator napiêcia", 
LR"(
W tym poziomie dowiesz siê co to stabilizator napiêcia!

Otó¿ stabilizator napiêcia to uk³ad scalony, którego zadaniem
jest utrzymywanie okreœlonego sta³ego napiêcia wyjœciowego 
niezale¿nie od jego wejœcia. 

Warunki zaliczenia zadania:
Pod³¹cz stabilizator napiêcia liniowego do mikrokontrolera,
w taki sposób aby napiêcie na zasilaniu mikrokontrolera 
nie przekracza³o 5V. Pamiêtaj o dodaniu dodatkowych 
kondensatorów miêdzy wejœcie i masê oraz wyjœcie i masê, 
w celu zapewnienia stabilnej pracy uk³adu w przypadku gdy
mikrokontroler potrzebowa³by wiêcej pr¹du ni¿ jest 
w stanie stabilizator napiêcia dostarczyæ.



)")));
	levelsMap.insert(std::pair<std::string, Level*>("ZEL1", loadMicrocontrollerLevel()));

	
	//levelsMap.insert(std::pair<std::string, Level*>("TEST", loadLevelTest()));
	levelsMap.insert(std::pair<std::string, Level*>("ASTART", loadLevelSTART()));
	levelsMap.insert(std::pair<std::string, Level*>("RES0", loadLevelRES0()));
	levelsMap.insert(std::pair<std::string, Level*>("RES1", loadLevelRES1()));
	levelsMap.insert(std::pair<std::string, Level*>("CAP0", loadLevelCAP0()));
	levelsMap.insert(std::pair<std::string, Level*>("LED0", loadLevelLED0()));



	if (User::getInstance().isLoggedIn())
	{
		try
		{
			Level::loadRealizedLevels(levelsMap);
		}
		catch (const std::string& exception)
		{
			
			//cout << exception;
			throw exception;
		}
	}

	//levelButtonsMap.insert(std::pair<std::string, Button*>("START", new Bu));

	Vector2f buttonSize = { 220.,80. };
	int maxVerticalButtons = 5;
	Vector2f buttonGap = { 40., 20. };
	Vector2f buttonsStart = { 10., 100. };

	auto calcButtonPos = [buttonsStart, buttonSize, buttonGap, maxVerticalButtons](int &i)->Vector2f {
		
		float x = buttonsStart.x + static_cast<int>(i / maxVerticalButtons) * (buttonSize.x + buttonGap.x);
		float y = buttonsStart.y + static_cast<int>(i % maxVerticalButtons) * (buttonSize.y + buttonGap.y);
		return Vector2f(x, y);
		};

	int i = 0;
	for(auto &level : levelsMap)
	{
		levelButtonsMap.insert(std::pair<std::string, Button*>(level.first, new Button(buttonSize, calcButtonPos(i), res->GetFont(), level.second->getName())));
		i++;
	}

	//Generate gui
	//levelButtons = new Button * [levelsCount];
	//levelButtons[0] = new Button({ 200,70 }, { 10,100 }, res->GetFont(), levels[0]->getName());
	//levelButtons[1] = new Button({ 200,70 }, { 250,100 }, res->GetFont(), levels[1]->getName());
	//levelButtons[2] = new Button({ 200,70 }, { 400,100 }, res->GetFont(), levels[1]->getName());


	const Vector2i& windowSize = res->getConfig().getWindowDimension();
	levelInfoState = LevelInfoState::Hidden;
	levelInfoBox = new RectangleShape({ 7.f * windowSize.x / 16.f,8.f * windowSize.y / 9.f });
	levelInfoBox->setFillColor(sf::Color(90, 170, 90, 100));
	levelInfoBox->setPosition(windowSize.x - 7.f * windowSize.x / 16.f - 50.f, 50.f);

	int buttonOffset = 5;
	loadSaveButton = new Button(
		{ 2 * (levelInfoBox->getSize().x - (buttonOffset * 3)) / 5, 50 },
		{ levelInfoBox->getPosition().x + buttonOffset, levelInfoBox->getSize().y - buttonOffset },
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
		{ levelInfoBox->getSize().x - 20, 8 * levelInfoBox->getSize().y / 9 - 100 },
		{ levelInfoBox->getPosition().x + 10, levelInfoBox->getPosition().y + 100 },
		res->GetFont(),
		L"Title"
	);
}

LevelSelect::~LevelSelect()
{
	//for (int i = 0; i < levelsCount; i++)
	//{
	//	if (levels[i] == startLevel)
	//		continue;
	//	delete levels[i];
	//}
	//delete[] levels;

	//for (int i = 0; i < levelsCount; i++)
	//	delete levelButtons[i];
	//delete[] levelButtons;


	//TODO clear memory

	//for (std::map<std::string, Level*>::iterator it = levelsMap.begin(); it != levelsMap.end(); it++)
	//{
	//	delete it->second;
	//	levelsMap.erase(it);
	//}

	//for (std::map<std::string, Button*>::iterator it = levelButtonsMap.begin(); it != levelButtonsMap.end(); it++)
	//{
	//	delete it->second;
	//	levelButtonsMap.erase(it);
	//}


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
		if (loadSaveButton->isButtonPressed())
		{
			nextState = new SolveLevel(res, startLevel, true);
		}

		startNewSaveButton->Update(mousePos);
		if (startNewSaveButton->isButtonPressed())
		{
			nextState = new SolveLevel(res, startLevel);
		}

		if (!levelInfoBox->getGlobalBounds().contains(mousePos))
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				levelInfoState = LevelInfoState::Hidden;

		return;
	}

	for (auto& levelButton : levelButtonsMap)
	{
		levelButton.second->Update(mousePos);
		try
		{
			if (!levelsMap[levelButton.first]->canRealize(levelsMap))
				continue;

			if (levelButton.second->GetButtonState() == ButtonStates::HOVER)
			{
				levelInfoState = LevelInfoState::Showed;
				levelTitle->SetString(levelsMap[levelButton.first]->getName());
				levelDesc->SetString(levelsMap[levelButton.first]->getDesc());
				break;
			}
			else if (levelButton.second->GetButtonState() == ButtonStates::PRESSED)
			{
				levelInfoState = LevelInfoState::Active;
				startLevel = levelsMap[levelButton.first];
				/*levelTitle->SetString(levels[i]->getName());
				levelDesc->SetString(levels[i]->getDesc());*/
			}
			else
			{
				if (levelInfoState != LevelInfoState::Active)
					levelInfoState = LevelInfoState::Hidden;
			}
		}
		catch (const std::exception&)
		{

		}
		
	}

	//for (int i = 0; i < levelsCount; i++)
	//{
	//	levelButtons[i]->Update(mousePos);
	//	if (!levels[i]->canRealize())
	//		continue;
	//	if (levelButtons[i]->GetButtonState() == ButtonStates::HOVER)
	//	{
	//		levelInfoState = LevelInfoState::Showed;
	//		levelTitle->SetString(levels[i]->getName());
	//		levelDesc->SetString(levels[i]->getDesc());
	//		break;
	//	}
	//	else if (levelButtons[i]->GetButtonState() == ButtonStates::PRESSED)
	//	{
	//		levelInfoState = LevelInfoState::Active;
	//		startLevel = levels[i];
	//		/*levelTitle->SetString(levels[i]->getName());
	//		levelDesc->SetString(levels[i]->getDesc());*/
	//	}
	//	else
	//	{
	//		if (levelInfoState != LevelInfoState::Active)
	//			levelInfoState = LevelInfoState::Hidden;
	//	}
	//}
}

void LevelSelect::Render(sf::RenderTarget* target)
{
	//for (int i = 0; i < levelsCount; i++)
	//	levelButtons[i]->Render(target);

	for (auto &levelButton : levelButtonsMap)
	{
		levelButton.second->Render(target);
	}

	backButton->Render(target);

	if (levelInfoState >= LevelInfoState::Showed)//|| levelInfoState == LevelInfoState::Showed
	{
		target->draw(*levelInfoBox);
		loadSaveButton->Render(target);
		startNewSaveButton->Render(target);
		levelTitle->Render(target);
		levelDesc->Render(target);
	}
}