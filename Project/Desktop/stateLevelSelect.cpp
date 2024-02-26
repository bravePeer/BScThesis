#include "stateLevelSelect.h"

LevelSelect::LevelSelect(Resources* res)
	:State(res)
{
	//if logged
	//	get data
	//Load all levels
	

	backButton = new Button({ 200,50 }, { 5.f,res->getConfig().getWindowDimension().y - 55.f }, res->GetFont(), L"Powrót");

	loadSaveButton = new Button({});
	startNewSaveButton = new Button();

	//Generating levels
	//levelsMap.insert(std::pair<std::string, Level*>("WEL0", emptyLevel(L"WEL0", L"Opór po³¹czenie\nszeregowe")));
	//levelsMap.insert(std::pair<std::string, Level*>("WEL2", emptyLevel(L"WEL3", L"Tranzystor\nbipolarny")));
	//levelsMap.insert(std::pair<std::string, Level*>("WEL3", emptyLevel(L"WEL4", L"Tranzystor\nMOSFET")));
	//levelsMap.insert(std::pair<std::string, Level*>("WEL4", emptyLevel(L"WEL5", L"Indukcyjnoœæ")));
	//levelsMap.insert(std::pair<std::string, Level*>("WEL5", emptyLevel(L"WEL6", L"Obwód RLC")));

	//TODO IMPORTANT dodaj coœ jeszcze
	/*levelsMap.insert(std::pair<std::string, Level*>("ZEL0", emptyLevel(L"ZEL0", L"Stabilizator napiêcia", 
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



)")));*/
	//levelsMap.insert(std::pair<std::string, Level*>("ZEL1", loadMicrocontrollerLevel()));

	
	//levelsMap.insert(std::pair<std::string, Level*>("TEST", loadLevelTest()));
	levelsMap.insert(std::pair<std::string, Level*>("ASTART", loadLevelSTART()));
	levelsMap.insert(std::pair<std::string, Level*>("BOHM", loadLevelRES0()));
	levelsMap.insert(std::pair<std::string, Level*>("KIR1", loadLevelKirchhoff()));
	levelsMap.insert(std::pair<std::string, Level*>("KIR2", loadLevelKirchhoff2()));
	//levelsMap.insert(std::pair<std::string, Level*>("ALED", loadLevelLED()));
	levelsMap.insert(std::pair<std::string, Level*>("RES1", loadLevelRES1()));
	levelsMap.insert(std::pair<std::string, Level*>("RES2", loadLevelRES2()));
	levelsMap.insert(std::pair<std::string, Level*>("RES3", loadLevelRES3()));

	levelsMap.insert(std::pair<std::string, Level*>("LED0", loadLevelLED0()));
	
	levelsMap.insert(std::pair<std::string, Level*>("CAP0", loadLevelCAP0()));
	levelsMap.insert(std::pair<std::string, Level*>("CAP1", loadLevelCAP1()));
	levelsMap.insert(std::pair<std::string, Level*>("CAP2", loadLevelCAP2()));
	levelsMap.insert(std::pair<std::string, Level*>("CAP3", loadLevelCAP3()));

	levelsMap.insert(std::pair<std::string, Level*>("IND0", loadLevelIND0()));

	levelsMap.insert(std::pair<std::string, Level*>("VOLTREG", loadLevelVOLREG()));
	levelsMap.insert(std::pair<std::string, Level*>("BITRAN0", loadLevelBITRAN0()));
	levelsMap.insert(std::pair<std::string, Level*>("MOSFET0", loadLevelMOSFET0()));
	levelsMap.insert(std::pair<std::string, Level*>("UC", loadLevelUC()));

	//levelsMap.insert(std::pair<std::string, Level*>("CAP0", loadLevelCAP0()));
	//levelsMap.insert(std::pair<std::string, Level*>("LED0", loadLevelLED0()));



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
	int maxVerticalButtons = 6;
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
		if(levelsMap[level.first]->canRealize(levelsMap))
			levelButtonsMap.insert(std::pair<std::string, Button*>(level.first, new Button(buttonSize, calcButtonPos(i), res->GetFont(), level.second->getName())));
		else
			levelButtonsMap.insert(std::pair<std::string, Button*>(level.first, new Button(buttonSize, calcButtonPos(i), res->GetFont(), level.second->getName(), sf::Color(87, 122, 96), sf::Color(79, 102, 85))));

		i++;
	}


	const Vector2i& windowSize = res->getConfig().getWindowDimension();
	levelInfoState = LevelInfoState::Hidden;
	levelInfoBox = new RectangleShape({ 7.f * windowSize.x / 16.f,8.f * windowSize.y / 9.f });
	levelInfoBox->setFillColor(sf::Color(90, 170, 90, 100));
	levelInfoBox->setPosition(windowSize.x - 7.f * windowSize.x / 16.f - 50.f, 80.f);

	int buttonOffset = 5;
	loadSaveButton = new Button(
		{ 2 * (levelInfoBox->getSize().x - (buttonOffset * 3)) / 5, 50 },
		{ levelInfoBox->getPosition().x + buttonOffset, levelInfoBox->getSize().y + levelInfoBox->getPosition().y - buttonOffset - 50 },
		res->GetFont(),
		L"Wczytaj zapis"
	);
	startNewSaveButton = new Button(
		{ 3 * (levelInfoBox->getSize().x - (buttonOffset * 3)) / 5, 50 },
		{ levelInfoBox->getPosition().x + loadSaveButton->getSize().x + 2 * buttonOffset, levelInfoBox->getSize().y + levelInfoBox->getPosition().y - buttonOffset - 50 },
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

	//Level::logRealizedLevels(levelsMap);
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
	{
		if(User::getInstance().isLoggedIn())
			User::getInstance().syncSavesFile();
		nextState = new StartMenu(res);
	}

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
			if (levelButton.second->GetButtonState() == ButtonStates::HOVER)
			{
				levelInfoState = LevelInfoState::Showed;
				levelTitle->SetString(levelsMap[levelButton.first]->getName());
				
				if (levelsMap[levelButton.first]->canRealize(levelsMap))
				{
					levelDesc->SetString(levelsMap[levelButton.first]->getShortDesc());
					hideStartNewButton = false;

					if (levelsMap[levelButton.first]->isRealized())
						hideLoadButton = false;
				}
				else
				{
					hideLoadButton = true;
					hideStartNewButton = true;
					sf::String str = L"Zrealizuj poprzednie poziomy, by móc ten poziom wykonaæ!\n\n\nWymagane poziomy:\n\n";
					auto& prevLevIds = levelsMap[levelButton.first]->getPrevLevelsIds();
					for (auto& prevLevId : prevLevIds)
					{
						str += levelsMap.find(prevLevId)->second->getName() + L"\n";
					}
					levelDesc->SetString(str);
				}
				
				break;
			}
			else if (levelButton.second->GetButtonState() == ButtonStates::PRESSED)
			{
				if (!levelsMap[levelButton.first]->canRealize(levelsMap))
					continue;
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
	if (User::getInstance().isLoggedIn())
	{
		drawUserProgress(target);
		drawUserLogin(target);
	}
	else
	{
		sf::String str = L"Zaloguj siê aby móc zapisaæ postêp!";
		sf::Text text(str, res->getFont(), 20);
		text.setPosition({ 10,10 });

		target->draw(text);
	}
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
		if(!hideLoadButton)
			loadSaveButton->Render(target);
		if (!hideStartNewButton)
			startNewSaveButton->Render(target);
		levelTitle->Render(target);
		levelDesc->Render(target);
	}


}

void LevelSelect::drawUserProgress(sf::RenderTarget* target)
{
	constexpr float progressBarWidth = 800.f;
	constexpr float progressBarHeight = 20.f;
	//constexpr float progressBarPosX = 300.f;
	constexpr float progressBarPosY = 40.f;

	sf::RectangleShape progressBar(sf::Vector2f(progressBarWidth, progressBarHeight));
	progressBar.setPosition(Vector2f((res->getConfig().getWindowDimension().x - progressBarWidth ) / 2.f, progressBarPosY));
	progressBar.setFillColor(sf::Color(13, 50, 13, 255));
	progressBar.setOutlineThickness(0.6f);
	progressBar.setOutlineColor(sf::Color::Black);

	int realizedLevels = Level::getRealizedLevelsCount(levelsMap);
	float progressWidth = (realizedLevels / static_cast<float>(levelsMap.size())) * progressBarWidth;
	
	sf::RectangleShape progress(sf::Vector2f(progressWidth, progressBarHeight));
	progress.setPosition(Vector2f(progressBar.getPosition().x, progressBarPosY));
	progress.setFillColor(Color(79, 199, 79, 255));

	sf::String str = L"Twój postêp: " + to_wstring(realizedLevels) + L" / " + to_wstring(levelsMap.size());
	sf::Text text(str, res->getFont(), 15);
	
	text.setPosition(sf::Vector2f(
		(progressBarWidth - (19.f * text.getCharacterSize()) / 2) / 2 + progressBar.getPosition().x,
		progressBarPosY
	));

	target->draw(progressBar);
	target->draw(progress);
	target->draw(text);
}

void LevelSelect::drawUserLogin(sf::RenderTarget* target)
{
	sf::String str = L"Witaj " + User::getInstance().getUserName() + L"!";
	sf::Text text(str, res->getFont(), 20);
	text.setPosition({ 10,10 });
	
	target->draw(text);
}

