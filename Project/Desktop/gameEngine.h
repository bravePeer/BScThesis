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
//typedef void (*addLetterF)(wchar_t w);
class Game
{
public:
	Game();
	Game(Game& game) = delete;
	~Game();

	//Aktualizowanie
	void Update();
	//Rysowanie
	void Render();

	bool IsRunning();

	RenderWindow* getWindow();
private:
	void PollEvents();
	

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

//static void (Game::* addLetter)(wchar_t w) = nullptr;