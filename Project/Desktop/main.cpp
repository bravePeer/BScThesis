#include <iostream>
#include "appEngine.h"
#include <Windows.h>
//#include "world.h"
#include "Logger.h"
#include <SFML/Window.hpp>
//using namespace std;
using namespace applogger;

int main()
{
	AppEngine app;
	Logger logger("main");
	//Logger::InitializeFileLogging();

	sf::Clock clock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;
	float fps;

	while (app.IsRunning())
	{
		app.PollEvents();

		if (!app.HasFocus())
			continue;

		//update
		app.Update();

		//rysowanie
		app.Render();

		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		app.getWindow()->setTitle(L"Praca Inzynierska fps: " + to_wstring(static_cast<int>(fps)));
		previousTime = currentTime;
	}

	//Logger::EndFileLogging();
	return 0;
}