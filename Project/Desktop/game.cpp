#include "game.h"

MainGame::MainGame(Resources* res, Level* level, bool loadExistingLevel)
	:res(res)
{
	logger = new Logger("Game");
	
	GraphicAll::GetInstance()->LoadGraphic();

	view = new View({ 800,450 }, { 1600, 900 });
	origin = view->getCenter();
	oldOrigin = origin;

	currentLevel = level;
	//Load components from task
	level->load();
	components = level->getComponents();
	componentsCount = level->getComponentsCount();


	//Info section
	selectedComponent = new TextBox({ 200.f, 100.f }, { 1200.f, 700.f }, res->GetFont(), L"Opis komponentu");

	//Load test components
	initRouteSection(res);
	initTaskSection(res);
	initComponentSection(res);
	initBoardSection();
	initInfoNearMouse(res);
	addComponent = nullptr;


	if (loadExistingLevel)
	{
		board = BoardSave::getInstance()->loadBoard(currentLevel->getPathToSave(), currentLevel);
		if (board == nullptr)
		{
			logger->Info("Creating new save!");
			loadExistingLevel = !loadExistingLevel;
		}
	}

	if(!loadExistingLevel)
	{
		//TODO load dimensions from level
		board = new Board(12, 18, 1);
		//init board
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };

		Component* vcc = new Goldpin(L"Z³¹cze goldpin", L"", { 1,1 }, 1, tmp, GraphicAll::GetInstance()->getGoldpinTexture(), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Z³¹cze goldpin", L"", { 1,1 }, 1, tmp, GraphicAll::GetInstance()->getGoldpinTexture(), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;

		//Vcc
		board->placeComponentForce(vcc, pinPos);

		//GND
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
	}


	menuButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1500.f, 0.f), res->GetFont(), L"MENU");
	helpButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1400.f, 0.f), res->GetFont(), L"Pomoc");
	checkButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1200.f, 0.f), res->GetFont(), L"SprawdŸ");
}

MainGame::~MainGame()
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

	delete logger;
}

State* MainGame::IsStateChanged()
{
	return nullptr;
}

void MainGame::Update(RenderWindow* window, Time* elapsed)
{
	board->Update(window, elapsed);
	helpButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
	menuButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
	if (menuButton->isButtonPressed())
	{
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
				return;
			}
			else
				logger->Info("Good components number");
		}
		catch (const std::string& info)
		{
			logger->Info(info);
		}
		
		BoardSave::getInstance()->saveBoard(board, "save.asc");
		
		//if (simulationEngine != nullptr)
			//delete simulationEngine;
		if(simulationEngine == nullptr)
			simulationEngine = new SimulationEngine();
		simulationEngine->simulate();
	}
}

void MainGame::Render(RenderTarget* target)
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
}

inline void MainGame::initTaskSection(Resources* res)
{
	//loadTask
	const Vector2f sectionPos = Config::getInstance()->getSectionConfig(Config::SectionConfig::TaskSection).getPosition();
	const Vector2f sectionSize = Config::getInstance()->getSectionConfig(Config::SectionConfig::TaskSection).getSize();

	taskName = new TextBox({ 400.f, 50.f }, sectionPos, res->GetFont(), currentLevel->getName());
	taskDescription = new TextBox(sectionSize - Vector2f(0, 60), sectionPos + Vector2f(0.f, 60.f), res->GetFont(), currentLevel->getDesc());
}

inline void MainGame::destroyTaskSection()
{
	delete testTask;
	delete taskName;
	delete taskDescription;
}

void MainGame::renderTaskSection(RenderTarget* target)
{
	taskDescription->Render(target);
	taskName->Render(target);
}

inline void MainGame::initRouteSection(Resources* res)
{
	hideComponentsButton = new Button({ 100.f, 50.f }, { 115.f, 735.f }, res->GetFont(), L"Ukryj");

	addRouteButton = new Button({ 100.f,50.f }, { 10.f, 735.f }, res->GetFont(), L"Po³¹cz");
	moveComponentButotn = new Button({ 100.f, 50.f }, { 10.f, 790.f }, res->GetFont(), L"Przesuñ");
	removeComponentButton = new Button({ 100.f, 50.f }, { 10.f, 845.f }, res->GetFont(), L"Usuñ");
}

inline void MainGame::destroyRouteSection()
{
	delete addRouteButton;
	delete removeComponentButton;
	delete moveComponentButotn;
	delete hideComponentsButton;
}

inline void MainGame::updateRouteSection(RenderWindow* window, Time* elapsed)
{
	Vector2f mousePos = sf::Vector2f(Mouse::getPosition(*window));

	addRouteButton->Update(mousePos);
	if (addRouteButton->GetButtonState() == ButtonStates::PRESSED)
	{
		mouseMode = MouseMode::Route;
		updateBoardSection = &MainGame::updateBoardSectionRoute;
		logger->Info("Button pressed");
		board->printRoutMap();
	}

	removeComponentButton->Update(mousePos);
	if (removeComponentButton->GetButtonState() == ButtonStates::PRESSED)
	{
		mouseMode = MouseMode::Remove;
		updateBoardSection = &MainGame::updateBoardSectionRemoveComponent;
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

inline void MainGame::renderRouteSection(RenderTarget* target)
{
	removeComponentButton->Render(target);
	addRouteButton->Render(target);
	moveComponentButotn->Render(target);
	hideComponentsButton->Render(target);
}

inline void MainGame::initComponentSection(Resources* res)
{
	Vector2f sectionPos = Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getPosition();
	Vector2f sectionSize = Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getSize();

	addingComponents = new Button * [componentsCount];
	for (int i = 0; i < componentsCount; i++)
	{
		addingComponents[i] = new Button({ 150.f, 120.f }, { 200.f,0.f }, res->GetFont(), components[i]->getName());
	}
	selectComponent = new SelectBox(
		Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getSize(),
		Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getPosition(),
		res->GetFont(),
		"Select Component",
		Color(255, 10, 80, 50),
		Color(255, 10, 80, 50),
		Color(255, 10, 80, 50),
		addingComponents,
		componentsCount);
}

inline void MainGame::destroyComponentSection()
{
	for (int i = 0; i < componentsCount; i++)
	{
		delete addingComponents[i];
	}
	delete[] addingComponents;

	delete selectedComponent;
	delete selectComponent;
}

inline void MainGame::updateComponentSection(RenderWindow* window, Time* elapsed)
{
	Vector2i mousePos = Mouse::getPosition(*window);

	if (Mouse::getPosition(*window).y > 600.f)
	{
		selectComponent->Update(window);

		int selectedComponentId = selectComponent->GetSelected();
		if (selectedComponentId != -1 && addComponent == nullptr)
		{
			logger->Info("Selected " + to_string(selectedComponentId) + "component");
			if (dynamic_cast<Resistor*>(components[selectedComponentId]))
				addComponent = new Resistor(dynamic_cast<Resistor*>(components[selectedComponentId]));
			else if (dynamic_cast<LedDiode*>(components[selectedComponentId]))
				addComponent = new LedDiode(dynamic_cast<LedDiode*>(components[selectedComponentId]));
			else
				addComponent = new Component(components[selectedComponentId]);
			selectComponent->ResetSelection();
			updateBoardSection = &MainGame::updateBoardSectionPlaceComponent;

			//addingComponents[selectedComponentId]->SetButtonState(ButtonStates::IDLE);
		}
	}
}

inline void MainGame::initBoardSection()
{
	/*board = new Board(12, 18, 1);*/

	updateBoardSection = &MainGame::updateBoardSectionIdle;
}

inline void MainGame::destroyBoardSection()
{
	delete board;
}

void MainGame::updateBoardSectionRoute(RenderWindow* window, Time* elapsed)
{
	Vector2i mousePos = sf::Mouse::getPosition(*window);

	//Add route
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		try
		{
			if (board->getHoverTilePos(mousePos) != lastTileHover)
				board->addRoute(lastTileHover, board->getHoverTilePos(mousePos));
		}
		catch (const sf::String& s)
		{
			s.getSize(); //Here was warning
		}
	}

	//Delete route
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (board->getHoverTilePos(mousePos) != lastTileHover)
			board->removeRoute(lastTileHover, board->getHoverTilePos(mousePos));
		//mouseMode = MouseMode::Idle;
	}

	try
	{
		lastTileHover = board->getHoverTilePos(mousePos);
	}
	catch (const sf::String& s)
	{
		s.getSize(); //Here was warning
	}
}

void MainGame::updateBoardSectionPlaceComponent(RenderWindow* window, Time* elapsed)
{
	if (addComponent == nullptr)
	{
		updateBoardSection = &MainGame::updateBoardSectionIdle;
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
		updateBoardSection = &MainGame::updateBoardSectionIdle;
	}
}

void MainGame::updateBoardSectionRemoveComponent(RenderWindow* window, Time* elapsed)
{
	Vector2i mousePos = sf::Mouse::getPosition(*window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		try
		{
			Vector2i componentPos = board->getHoverTilePos(mousePos);
			//board->getComponentOnBoard(componentPos);
			board->removeComponent(componentPos);
			updateBoardSection = &MainGame::updateBoardSectionIdle;
			logger->Info("Change to idle");
		}
		catch (const sf::String&)
		{

		}
	}
}

inline void MainGame::renderBoardSection(RenderTarget* target)
{
	board->Render(target);

	selectedComponent->Render(target);

	if (addComponent != nullptr)
		addComponent->Render(target);
}

inline void MainGame::initInfoNearMouse(Resources* res)
{
	mouseInfoBox = new TextBox({ 100,50 }, { 0,0 }, res->GetFont(), "Inormacje dotycz¹ce pola", Color(255, 0, 0, 0), Color(255, 0, 0, 0), Color(255, 0, 0, 0), 12);
}

inline void MainGame::destroyInfoNearMouse()
{
	delete mouseInfoBox;
}

inline void MainGame::updateInfoNearMouse(RenderWindow* window, Time* elapsed)
{
	//if in board section

	Vector2i mousePos = Mouse::getPosition(*window);
	sf::String str = L"a";
	try
	{
		Vector2i tmp = board->getHoverTilePos(mousePos);
		Tile& hoveredTile = board->getTile(tmp);
		str += L"Pos: ";
		str += L"x: " + to_wstring(tmp.x);
		str += L"y: " + to_wstring(tmp.y);
		str += L"\nIndeks: " + to_wstring(tmp.x + tmp.y * board->getBoardDimension().x);
		str += L"\nTile:" + hoveredTile.getStateString();
		Component* component = board->getComponentOnBoard(tmp);
		if (component != nullptr)
			str += L"\nKomponent: " + component->getName();
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

inline void MainGame::renderInfoNearMouse(RenderTarget* target)
{
	mouseInfoBox->Render(target);
}




