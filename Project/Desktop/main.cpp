#include <iostream>
#include "gameEngine.h"
#include <Windows.h>
//#include "world.h"
#include "Logger.h"
#include <SFML/Window.hpp>
//using namespace std;

int main()
{
	Game game;
	Logger logger("main");
	//Logger::InitializeFileLogging();

	sf::Clock clock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;
	float fps;

	while (game.IsRunning())
	{
		game.PollEvents();

		if (!game.HasFocus())
			continue;

		//update
		game.Update();

		//rysowanie
		game.Render();

		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		game.getWindow()->setTitle(L"Praca Inzynierska fps: " + to_wstring(static_cast<int>(fps)));
		previousTime = currentTime;
	}

	//Logger::EndFileLogging();
	return 0;
}