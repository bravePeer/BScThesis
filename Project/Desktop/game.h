#pragma once
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "resources.h"
#include "task.h"

//Main game state
class MainGame: public State
{
public:
	MainGame(){}
	MainGame(Resources *res) {
		//test task
		
		testTask = new Task(L"Zadanie testowe", L"Zadanie s³u¿¹ce \ndo przetestowania dzia³ania");

		board = new Board(16, 16, 1);
		
		selectionComponent = new TextBox({ 350.f, 50.f }, {10.f, 700.f}, res->GetFont(), L"Wybierz komponent elektroniczny");

		addRoute = new Button({100.f,50.f}, {10,750}, res->GetFont(), L"Po³¹cz");
		
		selectedComponent = new TextBox({ 200.f, 100.f }, {1200.f, 700.f}, res->GetFont(), L"Opis komponentu");


		menuButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1500.f, 0.f), res->GetFont(), L"MENU");
		helpButton = new Button(sf::Vector2f(100.f, 50.f), sf::Vector2f(1400.f, 0.f), res->GetFont(), L"Pomoc");

		taskName = new TextBox({ 400.f, 50.f }, posTaskSection, res->GetFont(), testTask->getName());
		taskDescription = new TextBox({ 400.f, 400.f }, posTaskSection + Vector2f(0,70.f), res->GetFont(), testTask->getDescription());
		logger = new Logger("Game");

	}
	~MainGame() 
	{
		delete menuButton;
		delete helpButton;
		delete testTask;

		delete taskName;
		delete taskDescription;

		delete board;

		delete selectionComponent;
		delete selectedComponent;
		delete addRoute;

		delete logger;
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
		
		addRoute->Update(sf::Vector2f(sf::Mouse::getPosition(*window)));
		if(addRoute->GetButtonState() == ButtonStates::PRESSED)
			logger->Info("Button pressed");
	}
	void Render(RenderTarget* target)
	{
		renderSelectionComponent(target);
		renderSelectedComponent(target);

		board->Render(target);
		renderTaskSection(target);
		helpButton->Render(target);
		menuButton->Render(target);
		addRoute->Render(target);
	}
private:
	Logger* logger;
	

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



	Board* board;
	// ------ Update ------



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
	}

	inline void renderSelectedComponent(RenderTarget* target)
	{
		selectedComponent->Render(target);
	}

	//void renderMenu
};