#include "appEngine.h"

AppEngine::AppEngine()
{
	logger = new Logger("\"Engine\"");

	const Vector2i &windowSize = Config::getInstance()->getWindowDimension();
	window = new RenderWindow(VideoMode(windowSize.x, windowSize.y, 64), "Praca Inzynierska", Style::Titlebar | Style::Close);
	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(60);

	//Settings::windowSize = window->getSize();

	res.LoadResources();

	//gameMenuState = STARTMENU;
	//events = new Event;

	gameState = new StartMenu(&res);
	//gameState = new MainGame(&res);

	logger->Info("App engine initialized");

	hasFocus = true;
}

AppEngine::~AppEngine()
{
	//delete startMenu;
	delete gameState;
	//delete events;
	delete window;

	logger->Info("App engine deleted");
	delete logger;
}

void AppEngine::Update()
{
	Time elapsed = clock.restart();

	gameState->Update(window, &elapsed);

	if (State* s = gameState->GetNextState())
	{
		delete gameState;
		gameState = s;
	}
}

void AppEngine::Render()
{
	window->clear(sf::Color(200, 200, 150, 255));

	gameState->Render(window);

	window->display();
}

bool AppEngine::IsRunning()
{
	return window->isOpen();
}

bool AppEngine::HasFocus()
{
	return hasFocus;
}

RenderWindow* AppEngine::getWindow()
{
	return window;
}

void AppEngine::PollEvents()
{
	Event events;
	while (window->pollEvent(events))
	{
		switch (events.type)
		{
		case Event::Closed:
			if (User::getInstance().isLoggedIn())
				User::getInstance().syncSavesFile();
			window->close();
			break;
		case Event::LostFocus:
			hasFocus = false;
			break;
		case Event::GainedFocus:
			hasFocus = true;
			break;
		case Event::KeyPressed:
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				if (User::getInstance().isLoggedIn())
					User::getInstance().syncSavesFile();
				window->close();
			}
			else if (Keyboard::isKeyPressed(Keyboard::F1))
			{
				User::getInstance().loginExistingUser("test1", "test1");
				User::getInstance().getSavesFile();
			}

			break;
		case Event::TextEntered:
			gameState->AddLetter(events.text.unicode);
			break;
		}
	}
}


