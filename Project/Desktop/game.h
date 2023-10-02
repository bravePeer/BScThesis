#pragma once
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "resources.h"
#include "task.h"
#include "component.h"

/*

Aplikacja desktopowa pomagaj�ce w nauce elektroniki polega na uk�adaniu komponent�w elektronicznych wed�ug podanego zadania.
U�ytkownik dostaje zadanie wraz z ograniczon� pul� element�w, z kt�rych musi u�o�y� dzia�aj�cy uk�ad. 
Wraz z post�pem u�ytkownika trudno�� zada� ro�nie. 
Zadaniem u�ytkownika jest u�o�enie element�w na p�ytce w taki spos�b aby uk�ad dzia�a� w spos�b okre�lony przez zadanie.
Aplikacja wymaga logowania, w celu zapisania post�pu gracza.

*/


//Ka�de zadanie zawiera opis element�w oraz 

//Main game state
class MainGame: public State
{
public:
	MainGame(){}
	MainGame(Resources *res) {
		logger = new Logger("Game");

		//GraphicAll::GetInstance()->LoadTestGraphic();
		GraphicAll::GetInstance()->LoadGraphic();

		view = new View({ 800,450 }, { 1600, 900 });
		origin = view->getCenter();
		oldOrigin = origin;

		//Load components
		components = new Component*[2];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorSprite(), GraphicAll::GetInstance()->getResistorTexture()->getSize());
		components[1] = new Capacitor(L"Kondensator", L"Kumuluje �adunek elektryczny", Vector2i(1, 2), 2, tmp, GraphicAll::GetInstance()->getCapacitorSprite(), GraphicAll::GetInstance()->getCapacitorTexture()->getSize());
		delete[] tmp;
		selectionComponent = new TextBox({ 350.f, 50.f }, {10.f, 700.f}, res->GetFont(), L"Wybierz komponent elektroniczny");


		
		addRoute = new Button({100.f,50.f}, {10,750}, res->GetFont(), L"Po��cz");
		//addLedDiode = new Button({ 100.f,50.f }, { 120.f, 750.f }, res->GetFont(), L"LED");
		
		//Info section
		selectedComponent = new TextBox({ 200.f, 100.f }, {1200.f, 700.f}, res->GetFont(), L"Opis komponentu");
		
		//Load test components
		initTaskSection(res);
		initComponentSection(res);
		initBoardSection();

		addComponent = nullptr;


		menuButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1500.f, 0.f), res->GetFont(), L"MENU");
		helpButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1400.f, 0.f), res->GetFont(), L"Pomoc");


		mouseInfoBox = new TextBox({ 100,50 }, { 0,0 }, res->GetFont(),  "Inormacje dotycz�ce pola", Color(255, 0, 0, 0), Color(255, 0, 0, 0), Color(255, 0, 0, 0), 12);
	}
	~MainGame() 
	{
		destroyTaskSection();
		destroyComponentSection();
		destroyBoardSection();
		delete view;

		delete menuButton;
		delete helpButton;

		delete selectedComponent;
		delete addRoute;
		//delete addLedDiode;
		delete selectComponent;

		delete logger;
		delete mouseInfoBox;
	}
	void AddLetter(wchar_t s) { }
	State* IsStateChanged()
	{
		return nullptr;
	}
	void Update(RenderWindow* window, Time* elapsed)
	{
		board->Update(window, elapsed);
		helpButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
		menuButton->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));

		

		updateComponentSection(window, elapsed);

		updateInfoNearMouse(window, elapsed);
	}
	void Render(RenderTarget* target)
	{
		
		renderTaskSection(target);
		
		helpButton->Render(target);
		menuButton->Render(target);
		
		addRoute->Render(target);
		
		renderComponentSection(target);
		
		renderBoardSection(target);

		selectComponent->Render(target);
		renderInfoNearMouse(target);
	}
private:
	Logger* logger;

	View* view;
	Vector2f origin;
	Vector2f oldOrigin;

	// Menu section
	Button* menuButton;
	Button* helpButton;

	//-----------------------------------------
	// Task section
	//-----------------------------------------
	inline void initTaskSection(Resources* res)
	{
		//loadTask
		testTask = new Task(L"Zadanie testowe", L"Zadanie s�u��ce \ndo przetestowania dzia�ania");
		
		const Vector2f sectionPos = Config::getInstance()->getSectionConfig(Config::SectionConfig::TaskSection).getPosition();
		const Vector2f sectionSize = Config::getInstance()->getSectionConfig(Config::SectionConfig::TaskSection).getSize();
		
		taskName = new TextBox({ 400.f, 50.f }, sectionPos, res->GetFont(), testTask->getName());
		taskDescription = new TextBox(sectionSize - Vector2f(0, 60), sectionPos + Vector2f(0.f, 60.f), res->GetFont(), testTask->getDescription());
	}
	inline void destroyTaskSection()
	{
		delete testTask;
		delete taskName;
		delete taskDescription;
	}
	void updateTaskSection()
	{

	}
	void renderTaskSection(RenderTarget* target)
	{
		taskDescription->Render(target);
		taskName->Render(target);
	}

	Task* testTask;
	/*sf::Text* taskName;
	sf::Text* taskDescription;*/
	TextBox* taskName;
	TextBox* taskDescription;
	
	//-----------------------------------------
	// Route section - Bottom 
	//-----------------------------------------
	inline void initRouteSection()
	{

	}
	inline void destroyRouteSection()
	{

	}
	inline void updateRouteSection()
	{

	}
	inline void renderSection()
	{

	}

	Button* addRoute;
	unsigned short addRouteState;

	//-----------------------------------------
	// Component section - Bottom
	//-----------------------------------------
	inline void initComponentSection(Resources* res)
	{
		Vector2f sectionPos = Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getPosition();
		Vector2f sectionSize = Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getSize();

		addingComponents = new Button * [2];
		addingComponents[0] = new Button({ 150.f, 120.f }, { 0.f,0.f }, res->GetFont(), components[0]->getName());
		addingComponents[1] = new Button({ 150.f, 120.f }, { 200.f,0.f }, res->GetFont(), components[1]->getName());
		selectComponent = new SelectBox(
			Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getSize(),
			Config::getInstance()->getSectionConfig(Config::SectionConfig::ComponentSection).getPosition(),
			res->GetFont(),
			"Select Component",
			Color(255, 0, 0, 0),
			Color(255, 0, 0, 0),
			Color(255, 0, 0, 0),
			addingComponents,
			2);
	}
	inline void destroyComponentSection()
	{
		delete selectionComponent;
	}
	inline void updateComponentSection(RenderWindow* window, Time* elapsed)
	{
		Vector2i mousePos = sf::Mouse::getPosition(*window);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseMode == MouseMode::Route)
		{
			try
			{
				if (board->getHoverTilePos(mousePos) != lastTileHover)
					board->addRoute(lastTileHover, board->getHoverTilePos(mousePos));
			}
			catch (const sf::String& s)
			{

			}
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mouseMode == MouseMode::Route)
		{
			if (board->getHoverTilePos(mousePos) != lastTileHover)
				board->removeRoute(lastTileHover, board->getHoverTilePos(mousePos));
			//mouseMode = MouseMode::Idle;
		}

		try
		{
			lastTileHover = board->getHoverTilePos(mousePos);
		}
		catch (const sf::String&s)
		{

		}


		//Place component on board
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && addComponent != nullptr)
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
		}


		//Cancel adding element
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && addComponent != nullptr)
		{
			delete addComponent;
			addComponent = nullptr; // not sure czy potrzebne
		}



		addRoute->Update(sf::Vector2f(mousePos));
		if (addRoute->GetButtonState() == ButtonStates::PRESSED)
		{
			/*Vector2i* tmp = new Vector2i[2];
			tmp[0].x = 0;
			tmp[0].y = 0;
			tmp[1].x = 1;
			tmp[1].y = 0;
			addComponent = new Component(L"Dioda", L"�wiec�ca", { 2,1 }, 2, tmp, GraphicAll::GetInstance()->GetSpriteTest(), GraphicAll::GetInstance()->getTestTexture()->getSize());
			delete[] tmp;*/

			//Vector2i pos = { 3,3 };
			//board->placeComponent(addComponent, pos);
			mouseMode = MouseMode::Route;
			logger->Info("Button pressed");
			board->printRoutMap();
		}

		/*addLedDiode->Update(sf::Vector2f(mousePos));
		if(addLedDiode->GetButtonState() == ButtonStates::PRESSED)
		{
			Vector2i* tmp = new Vector2i[2];
			tmp[0].x = 0;
			tmp[0].y = 0;
			tmp[1].x = 1;
			tmp[1].y = 0;
			addComponent = new Component(L"Dioda", L"�wiec�ca", { 2,1 }, 2, tmp, GraphicAll::GetInstance()->GetSpriteTest(), GraphicAll::GetInstance()->getTestTexture()->getSize());
			delete[] tmp;
		}*/

		if (Mouse::getPosition(*window).y > 600.f)
		{
			selectComponent->Update(window);

			int selectedComponentId = selectComponent->GetSelected();
			if (selectedComponentId != -1 && addComponent == nullptr)
			{
				/*Vector2i* tmp = new Vector2i[2];
				tmp[0].x = 0;
				tmp[0].y = 0;
				tmp[1].x = 1;
				tmp[1].y = 0;
				addComponent = new Component(L"Dioda", L"�wiec�ca", { 2,1 }, 2, tmp, GraphicAll::GetInstance()->GetSpriteTest(), GraphicAll::GetInstance()->getTestTexture()->getSize());
				delete[] tmp;*/
				logger->Info("Selected " + to_string(selectedComponentId) + "component");
				addComponent = new Component(components[selectedComponentId]);
				selectComponent->ResetSelection();

				//addingComponents[selectedComponentId]->SetButtonState(ButtonStates::IDLE);
			}
		}

		if (addComponent != nullptr)
		{
			try
			{
				Vector2i mousePos = sf::Mouse::getPosition(*window);
				Vector2i hoveredTile = board->getHoverTilePos(mousePos);
				if (board->canPlaceComponent(addComponent, hoveredTile))
					addComponent->setBoardPosition(hoveredTile);
			}
			catch (const sf::String& e)
			{

			}
			addComponent->Update(window, elapsed, board->getViewOrigin());
		}
	}
	inline void renderComponentSection(RenderTarget* target)
	{
		selectionComponent->Render(target);
	}

	TextBox* selectionComponent;
	

	TextBox* selectedComponent;
	//Button* addLedDiode;
	SelectBox* selectComponent;
	Button** addingComponents;

	Component** components;

	enum MouseMode {
		Idle,
		Route,
		Place
	};
	MouseMode mouseMode = MouseMode::Idle;

	Vector2i lastTileHover;

	//-----------------------------------------
	// Board section
	//-----------------------------------------
	inline void initBoardSection()
	{
		board = new Board(16, 16, 1);
	}
	inline void destroyBoardSection()
	{
		delete board;
	}
	inline void updateBoardSection(RenderWindow* window, Time* elapsed)
	{

	}
	inline void renderBoardSection(RenderTarget* target)
	{
		board->Render(target);

		selectedComponent->Render(target);

		if (addComponent != nullptr)
			addComponent->Render(target);
	}

	Board* board;
	Component* addComponent = nullptr;

	//-----------------------------------------
	// INFO near mouse
	//-----------------------------------------
	inline void updateInfoNearMouse(RenderWindow* window, Time* elapsed)
	{
		//if in board section

		Vector2i mousePos = Mouse::getPosition(*window);
		sf::String str = L"";
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
	inline void renderInfoNearMouse(RenderTarget* target)
	{
		mouseInfoBox->Render(target);
	}

	TextBox* mouseInfoBox;

};