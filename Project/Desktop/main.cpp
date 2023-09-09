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

	while (game.IsRunning())
	{
		//update
		game.Update();

		//rysowanie
		game.Render();
	}

	//Logger::EndFileLogging();
	return 0;
}