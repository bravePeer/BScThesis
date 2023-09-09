#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
//#include "settings.h"
#include "gameStates.h"
#include "utilities.h"
#include "game.h"
#include "Logger.h"

using namespace sf;

/*"Silnik" gry*/
class Game
{
public:
	Game()
	{
		logger = new Logger("\"Engine\"");

		window = new RenderWindow(VideoMode(1600, 900, 64), "Praca Inzynierska", Style::Titlebar | Style::Close);
		window->setVerticalSyncEnabled(true);
		window->setFramerateLimit(60);
		
		//Settings::windowSize = window->getSize();

		res.LoadResources();

		//gameMenuState = STARTMENU;
		events = new Event;

		//gameState = new StartMenu;
		gameState = new MainGame(&res);

		logger->Info("Game engine initialized");
	}
	~Game()
	{
		//delete startMenu;
		delete gameState;
		delete events;
		delete window;

		logger->Info("Game engine deleted");
		delete logger;
	}

	//Aktualizowanie
	void Update()
	{
		Time elapsed = clock.restart();
		
		PollEvents();

		gameState->Update(window, &elapsed);

		if (State* s = gameState->IsStateChanged())
		{
			delete gameState;
			gameState = s;
		}
	}

	//Rysowanie
	void Render()
	{
		window->clear();

		gameState->Render(window);

		window->display();
	}

	bool IsRunning()
	{
		return window->isOpen();
	}
private:
	void PollEvents()
	{
		while (window->pollEvent(*events))
		{
			switch (events->type)
			{
			case Event::Closed:
				window->close();
				break;
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Escape))
					window->close();
				
					break;
			case Event::TextEntered:
				 
				gameState->AddLetter(events->text.unicode);
				 
				//if (dynamic_cast<LoginMenu*>(gameState))
				//	dynamic_cast<LoginMenu*>(gameState)->AddLetter(events->text.unicode);
				//else if(dynamic_cast<RegisterMenu*>(gameState))
				//	dynamic_cast<RegisterMenu*>(gameState)->AddLetter(events->text.unicode);
				//break;
			}

		}
	}

	RenderWindow* window = nullptr;
	Event* events = nullptr;

	//Textury do menu

	Resources res;
	Clock clock;

	
	//Co ma byæ wyœwietlane w sensie ktore menu
	short gameMenuState;
	State* gameState = nullptr;

	Logger* logger;
};