#include "stateSolveLevel.h"

SolveLevel::SolveLevel(Resources* res, Level* level, bool loadExistingLevel)
	:State(res)
{
	logger = new applogger::Logger("SolveLevel");
	
	GraphicManager::GetInstance().loadTileGraphic();

	view = new View({ 800,450 }, { 1600, 900 });
	origin = view->getCenter();
	oldOrigin = origin;

	currentLevel = level;
	//Load components from task
	level->load();

	GraphicManager::GetInstance().loadLevelSchematic(level->getSchematicPath());
	levelSchematic = GraphicManager::GetInstance().getLevelSchematic();
	showLevelSchematic = false;

	for (auto a = level->getComponentsDesc().begin(); a!= level->getComponentsDesc().end(); a++)
	{
		a->second.setSprite(GraphicManager::GetInstance().loadComponentDescription(a->second.getPath()));
	}

	components = level->getComponents();
	componentsCount = level->getComponentsCount();


	//Info section
	//selectedComponent = new TextBox({ 200.f, 100.f }, { 1200.f, 700.f }, res->GetFont(), L"Opis komponentu");

	//Load test components
	initRouteSection(res);
	initTaskSection(res);
	initComponentSection(res);
	initBoardSection();
	initInfoNearMouse(res);
	addComponent = nullptr;

	//componentDesc = new ComponentDesc(res);

	if (loadExistingLevel)
	{
		Level::extractRelizedLevel(currentLevel->getId());
		board = BoardSave::getInstance().loadBoard(currentLevel->getPathToSave(), currentLevel);
		if (board == nullptr)
		{
			logger->Info("Creating new save!");
			loadExistingLevel = !loadExistingLevel;
		}
	}
	else
	{
		//TODO load dimensions from level
		board = new Board(currentLevel->getBoardDimension().x, currentLevel->getBoardDimension().y, currentLevel->getBoardDimension().z);
		//init board
		level->initBoard(board);
	}

	FloatRect configValue = res->getConfig().getSectionConfig(Config::SectionConfig::MenuSection);
	auto calPos = [&configValue](int i)->float {
		return configValue.left + ((configValue.width - 20.f) / 3 + 10) * i;
		};

	menuButton = new Button(sf::Vector2f((configValue.width - 20.f) / 3, configValue.height), sf::Vector2f(calPos(2), 0.f), res->GetFont(), L"Powrót");
	helpButton = new Button(sf::Vector2f((configValue.width - 20.f) / 3, configValue.height), sf::Vector2f(calPos(1), 0.f), res->GetFont(), L"Pomoc");
	checkButton = new Button(sf::Vector2f((configValue.width - 20.f) / 3, configValue.height), sf::Vector2f(calPos(0), 0.f), res->GetFont(), L"SprawdŸ");

	Vector2f posSchematicButton = Vector2f(res->getConfig().getSectionConfig(Config::SectionConfig::TaskSection).getPosition().x+5,
		res->getConfig().getSectionConfig(Config::SectionConfig::TaskSection).getPosition().y + 
		res->getConfig().getSectionConfig(Config::SectionConfig::TaskSection).getSize().y - 45);

	showLevelSchematicButton = new Button(sf::Vector2f(configValue.width-10, 40), posSchematicButton, res->GetFont(), L"Zobacz schemat");

	simulationCheckResponse = new TextBox({ 300.f, 50.f }, { 700.f, 0.f }, res->GetFont(), L"", sf::Color(0,0,0,0));

	configValue = res->getConfig().getSectionConfig(Config::SectionConfig::ComponentSection);
	if (currentLevel->getComponentsCount() > 5)
	{
		moveAddComponentsRight = new Button({ 100.f,25.f }, { configValue.getSize().x + configValue.getPosition().x - 100.f,configValue.getPosition().y + configValue.getSize().y - 35.1f }, res->GetFont(), L"\n>\n\n");
		moveAddComponentsLeft = new Button({ 100.f,25.f }, { configValue.getPosition().x+0.f,configValue.getPosition().y + configValue.getSize().y - 35.1f }, res->GetFont(), L"\n<\n\n");
	}

	helpBox.setTexture(GraphicManager::GetInstance().loadHelp());
	helpBox.setPosition({ 10.f,10.f });
	showHelp = false;

	Resistor::resetComponentCounter();
	LightEmittingDiode::resetComponentCounter();
	Capacitor::resetComponentCounter();
	Microcontroller::resetComponentCounter();
	Voltmeter::resetComponentCounter();
	Amperemeter::resetComponentCounter();
}

SolveLevel::~SolveLevel()
{
	destroyTaskSection();
	destroyComponentSection();
	destroyBoardSection();
	destroyRouteSection();
	destroyInfoNearMouse();

	delete view;

	delete menuButton;
	delete helpButton;
	delete checkButton;

	delete currentLevel;

	//delete componentDesc;

	delete logger;

	GraphicManager::GetInstance().unloadGraphics();
}

void SolveLevel::Update(RenderWindow* window, Time* elapsed)
{
	board->Update(window, elapsed);
	helpButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
	if (helpButton->isButtonPressed())
	{
		showHelp = !showHelp;
	}

	showLevelSchematicButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
	if (showLevelSchematicButton->isButtonPressed())
	{
		showLevelSchematic = !showLevelSchematic;
		if (showLevelSchematic)
			showLevelSchematicButton->SetString(L"Ukryj schemat");
		else
			showLevelSchematicButton->SetString(L"Poka¿ schemat");
	}


	menuButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
	if (menuButton->isButtonPressed())
	{
		BoardSave::getInstance().saveBoard(board, "save.asc");
		if(User::getInstance().isLoggedIn())
			Level::saveRealizedLevel(currentLevel->getId(), 0);
		nextState = new LevelSelect(res);
	}

	updateRouteSection(window, elapsed);
	updateComponentSection(window, elapsed);
	(this->*updateBoardSection)(window, elapsed);

	updateInfoNearMouse(window, elapsed);

	checkButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
	if (checkButton->isButtonPressed())
	{
		try
		{
			if (!currentLevel->checkBoard(board))
			{
				logger->Info("Wrong connections or components number!");
				simulationCheckResponse->SetString(L"Z³a liczba wykorzystanych elementów!");
				showSimulationCheckResponse = true;
				return;
			}
			else
				logger->Info("Good components number");
		}
		catch (const std::string& info)
		{
			logger->Info(info);
		}
		
		BoardSave::getInstance().saveBoard(board, "save.asc");
		
		//if (simulationEngine != nullptr)
			//delete simulationEngine;
		if(simulationEngine == nullptr)
			simulationEngine = new SimulationEngine();
		simulationEngine->simulate();
		//simulationEngine->getComponentValue("led0id1");
		if (!currentLevel->checkSimulation(board, simulationEngine))
		{
			simulationCheckResponse->SetString(L"Z³e po³¹czenie elementów!");
			showSimulationCheckResponse = true;
		}
		else
		{
			simulationCheckResponse->SetString(L"Dobrze zrealizowany poziom!");
			showSimulationCheckResponse = true;
		}

	}

	const sf::Vector2f moveViewSpeed{50.f,50.f};
	sf::Vector2f viewOffset{ 0.f,0.f };
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		viewOffset.y = moveViewSpeed.y * elapsed->asSeconds();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		viewOffset.y = -moveViewSpeed.y * elapsed->asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		viewOffset.x = moveViewSpeed.x * elapsed->asSeconds();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		viewOffset.x = -moveViewSpeed.x * elapsed->asSeconds();
	
	if(viewOffset.x != 0.f || viewOffset.y != 0.f)
		board->moveViewOrigin(viewOffset);

	if (moveAddComponentsLeft)
	{
		moveAddComponentsLeft->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
		if (moveAddComponentsLeft->isButtonPressed())
			selectComponent->setPrevOffset();

		moveAddComponentsRight->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
		if (moveAddComponentsRight->isButtonPressed())
			selectComponent->setNextOffset();
	}
}

void SolveLevel::Render(RenderTarget* target)
{
	renderTaskSection(target);

	helpButton->Render(target);
	menuButton->Render(target);

	renderRouteSection(target);

	renderComponentSection(target);

	renderBoardSection(target);

	selectComponent->Render(target);

	renderInfoNearMouse(target);

	checkButton->Render(target);

	showLevelSchematicButton->Render(target);


	if (simulationCheckResponse)
		simulationCheckResponse->Render(target);

	if(componentDesc != nullptr)
		componentDesc->Render(target);

	if (showLevelSchematic)
		target->draw(levelSchematic);

	if (moveAddComponentsLeft)
	{
		moveAddComponentsLeft->Render(target);
		moveAddComponentsRight->Render(target);
	}

	if (showHelp)
	{
		target->draw(helpBox);
	}
}

inline void SolveLevel::initTaskSection(Resources* res)
{
	//loadTask
	const Vector2f sectionPos = res->getConfig().getSectionConfig(Config::SectionConfig::TaskSection).getPosition();
	const Vector2f sectionSize = res->getConfig().getSectionConfig(Config::SectionConfig::TaskSection).getSize();

	taskName = new TextBox({ sectionSize.x, 50.f }, sectionPos, res->GetFont(), currentLevel->getName());
	taskDescription = new TextBox(
		sectionSize - Vector2f(0, 5), 
		sectionPos + Vector2f(0.f, 50.f), 
		res->GetFont(), 
		currentLevel->getDesc(), 
		Color(79, 199, 79, 200),  
		Color(38, 173, 38, 200), 
		Color(31, 146, 31, 200),
		16U);
}

inline void SolveLevel::destroyTaskSection()
{
	delete taskName;
	delete taskDescription;
}

void SolveLevel::renderTaskSection(RenderTarget* target)
{
	taskDescription->Render(target);
	taskName->Render(target);
}

inline void SolveLevel::initRouteSection(Resources* res)
{
	FloatRect configValue = res->getConfig().getSectionConfig(Config::SectionConfig::ComponentSection);

	auto calPos = [&configValue](int i)->float {
		return configValue.top + ((configValue.height - 30.f) / 4 + 10) * i;
		};
	
	hideComponentsButton = new Button({ configValue.left - 20.f,(configValue.height - 30.f) / 4 }, { 10.f, calPos(1)}, res->GetFont(), L"Ukryj");

	addRouteButton = new Button({ configValue.left - 20.f,(configValue.height - 40.f) / 4 }, { 10.f, calPos(0) }, res->GetFont(), L"Po³¹cz");
	moveComponentButotn = new Button({ configValue.left - 20.f, (configValue.height - 40.f) / 4 }, { 10.f, calPos(3) }, res->GetFont(), L"Przesuñ");
	removeComponentButton = new Button({ configValue.left - 20.f, (configValue.height - 40.f) / 4 }, { 10.f, calPos(2) }, res->GetFont(), L"Usuñ");
}

inline void SolveLevel::destroyRouteSection()
{
	delete addRouteButton;
	delete removeComponentButton;
	delete moveComponentButotn;
	delete hideComponentsButton;
}

inline void SolveLevel::updateRouteSection(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = sf::Vector2f(Mouse::getPosition(*window));

	addRouteButton->Update(mousePos);
	if (addRouteButton->GetButtonState() == ButtonStates::PRESSED)
	{
		mouseMode = MouseMode::Route;
		updateBoardSection = &SolveLevel::updateBoardSectionRoute;
		logger->Info("Button pressed");
	}

	removeComponentButton->Update(mousePos);
	if (removeComponentButton->GetButtonState() == ButtonStates::PRESSED)
	{
		mouseMode = MouseMode::Remove;
		updateBoardSection = &SolveLevel::updateBoardSectionRemoveComponent;
		logger->Info("Remove button pressed");
	}

	moveComponentButotn->Update(mousePos);
	//TODO

	hideComponentsButton->Update(mousePos);
	if (hideComponentsButton->GetButtonState() == ButtonStates::PRESSED)
	{
		if (board->isHideComponent())
		{
			hideComponentsButton->SetString(L"Ukryj");
			board->setHideComponent(false);
		}
		else
		{
			hideComponentsButton->SetString(L"Poka¿");
			board->setHideComponent(true);
		}
	}
}

inline void SolveLevel::renderRouteSection(RenderTarget* target)
{
	removeComponentButton->Render(target);
	addRouteButton->Render(target);
	moveComponentButotn->Render(target);
	hideComponentsButton->Render(target);
}

inline void SolveLevel::initComponentSection(Resources* res)
{
	Vector2f sectionPos = res->getConfig().getSectionConfig(Config::SectionConfig::ComponentSection).getPosition();
	Vector2f sectionSize = res->getConfig().getSectionConfig(Config::SectionConfig::ComponentSection).getSize();

	addingComponents = new Button * [componentsCount];
	for (int i = 0; i < componentsCount; i++)
	{
		addingComponents[i] = new Button({ 150.f, 120.f }, { 200.f,0.f }, res->GetFont(), components[i]->getName());
	}
	
	selectComponent = new SelectBox(
		res->getConfig().getSectionConfig(Config::SectionConfig::ComponentSection).getSize(),
		res->getConfig().getSectionConfig(Config::SectionConfig::ComponentSection).getPosition(),
		res->GetFont(),
		"Select Component",
		Color(46, 207, 73, 50),
		Color(255, 10, 80, 50),
		Color(255, 10, 80, 50),
		addingComponents,
		componentsCount);
}

inline void SolveLevel::destroyComponentSection()
{
	for (int i = 0; i < componentsCount; i++)
	{
		delete addingComponents[i];
	}
	delete[] addingComponents;

	//delete selectedComponent;
	delete selectComponent;
}

inline void SolveLevel::updateComponentSection(RenderWindow* window, Time* elapsed)
{
	Vector2i mousePos = Mouse::getPosition(*window);

	if (Mouse::getPosition(*window).y > 600.f)
	{
		selectComponent->Update(window);
		
		int selectedComponentId = selectComponent->GetSelected();
		if (selectedComponentId != -1 && addComponent == nullptr)
		{
			logger->Info("Selected " + to_string(selectedComponentId) + "component");
			addComponent = components[selectedComponentId]->clone();

			/*if (dynamic_cast<Resistor*>(components[selectedComponentId]))
				addComponent = new Resistor(dynamic_cast<Resistor*>(components[selectedComponentId]));
			else if (dynamic_cast<LightEmittingDiode*>(components[selectedComponentId]))
				addComponent = new LightEmittingDiode(dynamic_cast<LightEmittingDiode*>(components[selectedComponentId]));
			else if (dynamic_cast<Microcontroller*>(components[selectedComponentId]))
				addComponent = new Microcontroller(dynamic_cast<Microcontroller*>(components[selectedComponentId]));
			else
				addComponent = new Component(*components[selectedComponentId]);*/
			selectComponent->ResetSelection();
			updateBoardSection = &SolveLevel::updateBoardSectionPlaceComponent;

			//addingComponents[selectedComponentId]->SetButtonState(ButtonStates::IDLE);
		}

		short hoveredComponentId = selectComponent->GetHovered();
		if (hoveredComponentId != -1)
		{
			/*componentDesc->setComponent(components[hoveredComponentId]);
			componentDesc->setRender(true);*/
			try
			{
				componentDesc = &currentLevel->getComponentsDesc()[hoveredComponentId];
			}
			catch (const std::exception& )
			{
				logger->Error("Can't show component description");
			}
		}
		else
			componentDesc = nullptr;
			//componentDesc->setRender(false);
	}

	/*if (Keyboard::isKeyPressed(Keyboard::Key::N))
	{
		selectComponent->setButtonOffset(0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::M))
	{
		selectComponent->setButtonOffset(1);
	}*/
}

inline void SolveLevel::initBoardSection()
{
	/*board = new Board(12, 18, 1);*/

	updateBoardSection = &SolveLevel::updateBoardSectionIdle;
}

inline void SolveLevel::destroyBoardSection()
{
	delete board;
}

void SolveLevel::updateBoardSectionRoute(RenderWindow* window, Time* elapsed)
{
	Vector2i mousePos = sf::Mouse::getPosition(*window);
	
	try
	{
		sf::Vector2i tempTileHover = board->getHoverTilePos(mousePos);

		//Add route
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (tempTileHover != lastTileHover)
				board->addRoute(lastTileHover, tempTileHover);
		}

		//Delete route
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (tempTileHover != lastTileHover)
				board->removeRoute(lastTileHover, tempTileHover);
		}

		lastTileHover = tempTileHover;
	}
	catch (const sf::String& s)
	{
		s.getSize(); 
	}
}

void SolveLevel::updateBoardSectionPlaceComponent(RenderWindow* window, Time* elapsed)
{
	if (addComponent == nullptr)
	{
		updateBoardSection = &SolveLevel::updateBoardSectionIdle;
		return;
	}

	//Rotate component
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		rotated = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !rotated)
	{
		rotated = true;
		addComponent->rotate();
	}

	//Move mouse
	Vector2i mousePos = sf::Mouse::getPosition(*window);
	try
	{
		Vector2i hoveredTile = board->getHoverTilePos(mousePos);
		if (board->canPlaceComponent(addComponent, hoveredTile))
			addComponent->setBoardPosition(hoveredTile);
	}
	catch (const sf::String& s)
	{
		s.getSize(); //Here was warning
	}
	addComponent->Update(window, elapsed, board->getViewOrigin());

	//Cancel placeing component
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		delete addComponent;
		addComponent = nullptr; // not sure czy potrzebne
		return;
	}

	//Place component on board
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		try
		{
			Vector2i hoveredTile = board->getHoverTilePos(mousePos);
			board->placeComponent(addComponent, hoveredTile);
		}
		catch (const sf::String& e)
		{
			logger->Info(e);
			delete addComponent;
		}
		addComponent = nullptr;
		updateBoardSection = &SolveLevel::updateBoardSectionIdle;
	}
}

void SolveLevel::updateBoardSectionRemoveComponent(RenderWindow* window, Time* elapsed)
{
	Vector2i mousePos = sf::Mouse::getPosition(*window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		try
		{
			Vector2i componentPos = board->getHoverTilePos(mousePos);
			//board->getComponentOnBoard(componentPos);
			board->removeComponent(componentPos);
			updateBoardSection = &SolveLevel::updateBoardSectionIdle;
			logger->Info("Change to idle");
		}
		catch (const sf::String&)
		{

		}
	}
}

inline void SolveLevel::renderBoardSection(RenderTarget* target)
{
	board->Render(target);

	//selectedComponent->Render(target);

	if (addComponent != nullptr)
		addComponent->Render(target);
}

inline void SolveLevel::initInfoNearMouse(Resources* res)
{
	mouseInfoBox = new TextBox({ 100,50 }, { 0,0 }, res->GetFont(), "Inormacje dotycz¹ce pola", Color(255, 0, 0, 0), Color(255, 0, 0, 0), Color(255, 0, 0, 0), 15);
}

inline void SolveLevel::destroyInfoNearMouse()
{
	delete mouseInfoBox;
}

inline void SolveLevel::updateInfoNearMouse(RenderWindow* window, Time* elapsed)
{
	//if in board section
	shouldRenderInfoNearMouse = false;

	Vector2i mousePos = Mouse::getPosition(*window);
	if (!res->getConfig().getSectionConfig(Config::SectionConfig::BoardSection).contains(static_cast<Vector2f>(mousePos)))
		return;

	shouldRenderInfoNearMouse = true;
	
	sf::String str = L"";
	try
	{
		Vector2i tmp = board->getHoverTilePos(mousePos);
		Tile& hoveredTile = board->getTile(tmp);
		str += L"Pos: ";
		str += L"x: " + to_wstring(tmp.x);
		str += L"y: " + to_wstring(tmp.y);
		//str += L"\nIndeks: " + to_wstring(tmp.x + tmp.y * board->getBoardDimension().x);
		str += L"\nKafelek:" + hoveredTile.getStateString();
		Component* component = board->getComponentOnBoard(tmp);
		if (component != nullptr)
		{
			str += L"\nKomponent: " + component->getName();
			if (simulationEngine)
			{
				if (component->getComponentType() == Component::CompoenetType::amperemeter)
				{
					try
					{
						str += L"\nNatê¿enie pr¹du: " + to_wstring(abs(simulationEngine->getComponentValue(component->getSimName()))) + L"A";
					}
					catch (const  std::string&)
					{

					}
				}
				else if (component->getComponentType() == Component::CompoenetType::voltmeter)
				{
					try
					{
						str += L"\nNapiêcie: " + to_wstring(abs( simulationEngine->getComponentValue(component->getSimName()) * stof(component->getSimValue().substr(0, component->getSimValue().length()-1)))) + L"V";
					}
					catch (const  std::string&)
					{

					}
				}
			}
		}
		
	}
	catch (const sf::String& e)
	{
		str = e;
	}

	mouseInfoBox->SetString(str);
	mouseInfoBox->SetPostition({
		static_cast<float>(Mouse::getPosition(*window).x) + origin.x - window->getSize().x / 2 + 50,
		static_cast<float>(Mouse::getPosition(*window).y + origin.y - window->getSize().y / 2) });
	mouseInfoBox->Update();
}

inline void SolveLevel::renderInfoNearMouse(RenderTarget* target)
{
	if (!shouldRenderInfoNearMouse)
		return;
	mouseInfoBox->Render(target);
}




