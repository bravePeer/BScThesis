#include "gameEngine.h"

Game::Game()
{
	logger = new Logger("\"Engine\"");

	const Vector2i &windowSize = Config::getInstance()->getWindowDimension();
	window = new RenderWindow(VideoMode(windowSize.x, windowSize.y, 64), "Praca Inzynierska", Style::Titlebar | Style::Close);
	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(60);

	//Settings::windowSize = window->getSize();

	res.LoadResources();

	//gameMenuState = STARTMENU;
	events = new Event;

	gameState = new StartMenu(&res);
	//gameState = new MainGame(&res);

	logger->Info("Game engine initialized");
}

Game::~Game()
{
	//delete startMenu;
	delete gameState;
	delete events;
	delete window;

	logger->Info("Game engine deleted");
	delete logger;
}

void Game::Update()
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

void Game::Render()
{
	window->clear(sf::Color(200, 200, 150, 255));

	gameState->Render(window);

	window->display();
}

bool Game::IsRunning()
{
	return window->isOpen();
}

RenderWindow* Game::getWindow()
{
	return window;
}

void Game::PollEvents()
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
			break;
		}
	}
}


