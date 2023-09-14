#pragma once
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "resources.h"
#include "task.h"
#include "component.h"

//Main game state
class MainGame: public State
{
public:
	MainGame(){}
	MainGame(Resources *res) {
		//test task
		
		graphics.LoadTestGraphic();

		view = new View({ 800,450 }, { 1600, 900 });
		origin = view->getCenter();
		oldOrigin = origin;


		testTask = new Task(L"Zadanie testowe", L"Zadanie s³u¿¹ce \ndo przetestowania dzia³ania");

		board = new Board(16, 16, 1);
		
		selectionComponent = new TextBox({ 350.f, 50.f }, {10.f, 700.f}, res->GetFont(), L"Wybierz komponent elektroniczny");

		addRoute = new Button({100.f,50.f}, {10,750}, res->GetFont(), L"Po³¹cz");
		addLedDiode = new Button({ 100.f,50.f }, { 120.f, 750.f }, res->GetFont(), L"LED");
		
		selectedComponent = new TextBox({ 200.f, 100.f }, {1200.f, 700.f}, res->GetFont(), L"Opis komponentu");

		menuButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1500.f, 0.f), res->GetFont(), L"MENU");
		helpButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1400.f, 0.f), res->GetFont(), L"Pomoc");

		taskName = new TextBox({ 400.f, 50.f }, posTaskSection, res->GetFont(), testTask->getName());
		taskDescription = new TextBox({ 400.f, 400.f }, posTaskSection + Vector2f(0,70.f), res->GetFont(), testTask->getDescription());
		logger = new Logger("Game");


		mouseInfoBox = new TextBox({ 100,50 }, { 0,0 }, res->GetFont(),  "Inormacje dotycz¹ce pola", Color(255, 0, 0, 0), Color(255, 0, 0, 0), Color(255, 0, 0, 0), 12);
	}
	~MainGame() 
	{
		delete view;

		delete menuButton;
		delete helpButton;
		delete testTask;

		delete taskName;
		delete taskDescription;

		delete board;

		delete selectionComponent;
		delete selectedComponent;
		delete addRoute;
		delete addLedDiode;

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

		updateSelectionComponent(window, elapsed);

		updateInfoNearMouse(window, elapsed);
	}
	void Render(RenderTarget* target)
	{
		board->Render(target);
		renderTaskSection(target);
		helpButton->Render(target);
		menuButton->Render(target);
		addRoute->Render(target);
		
		renderSelectionComponent(target);
		renderSelectedComponent(target);

		renderInfoNearMouse(target);
	}
private:
	Logger* logger;
	GraphicAll graphics;

	View* view;
	Vector2f origin;
	Vector2f oldOrigin;

	// Menu section
	Button* menuButton;
	Button* helpButton;

	// Task section
	const Vector2f posTaskSection = { 1200.f, 100.f };
	const Vector2f sizeTaskSection = { 400.f, 200.f };

	Task* testTask;
	/*sf::Text* taskName;
	sf::Text* taskDescription;*/
	TextBox* taskName;
	TextBox* taskDescription;


	// Select component section
	TextBox* selectionComponent;
	Button* addRoute;
	unsigned short addRouteState;

	// Info component section
	TextBox* selectedComponent;
	Button* addLedDiode;

	// INFO near mouse
	TextBox* mouseInfoBox;
	inline void updateInfoNearMouse(RenderWindow* window, Time* elapsed)
	{
		//if in board section

		Vector2i mousePos = Mouse::getPosition(*window);
		sf::String str = L"";

		try
		{
			Vector2i tmp = board->getHoverTile(mousePos);
			str += L"Pos: ";
			str += L"x: " + to_wstring(tmp.x);
			str += L"y: " + to_wstring(tmp.y);
			str += L"\nIndex: " + to_wstring(tmp.x + tmp.y * board->getBoardDimension().x);
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




	Board* board;

	Component* addComponent = nullptr;

	// ------ Update ------
	inline void updateSelectionComponent(RenderWindow* window, Time* elapsed)
	{
		Vector2i mousePos = sf::Mouse::getPosition(*window);

		//Place component on board
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && addComponent != nullptr)
		{
			try
			{
				Vector2i hoveredTile = board->getHoverTile(mousePos);
				board->placeComponent(addComponent, hoveredTile);
			}
			catch (const sf::String&e)
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
			Vector2i* tmp = new Vector2i[2];
			tmp[0].x = 0;
			tmp[0].y = 0;
			tmp[1].x = 1;
			tmp[1].y = 0;
			addComponent = new Component(L"Dioda", L"Œwiec¹ca", { 2,1 }, 2, tmp, graphics.GetSpriteTest());
			delete[] tmp;

			Vector2i pos = { 3,3 };
			board->placeComponent(addComponent, pos);
			logger->Info("Button pressed");
		}

		addLedDiode->Update(sf::Vector2f(mousePos));
		if(addLedDiode->GetButtonState() == ButtonStates::PRESSED)
		{
			Vector2i* tmp = new Vector2i[2];
			tmp[0].x = 0;
			tmp[0].y = 0;
			tmp[1].x = 1;
			tmp[1].y = 0;
			addComponent = new Component(L"Dioda", L"Œwiec¹ca", { 2,1 }, 2, tmp, graphics.GetSpriteTest());
			delete[] tmp;

		}


		if (addComponent != nullptr)
		{
			try
			{
				Vector2i mousePos = sf::Mouse::getPosition(*window);
				Vector2i hoveredTile = board->getHoverTile(mousePos);
				if (board->canPlaceComponent(addComponent, hoveredTile))
					addComponent->setBoardPosition(hoveredTile);
			}
			catch (const sf::String& e)
			{

			}
			addComponent->Update(window, elapsed, board->getViewOrigin());
		}

	}


	
	// ------ Render ------
	inline void renderBoardSection()
	{

	}

	inline void renderTaskSection(RenderTarget* target)
	{
		taskDescription->Render(target);
		taskName->Render(target);
	}

	inline void renderSelectionComponent(RenderTarget* target)
	{
		selectionComponent->Render(target);
		addLedDiode->Render(target);
	}

	inline void renderSelectedComponent(RenderTarget* target)
	{
		selectedComponent->Render(target);

		if (addComponent != nullptr)
			addComponent->Render(target);
	}

	//void renderMenu
};