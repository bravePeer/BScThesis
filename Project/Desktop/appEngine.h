#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "utilities.h"
#include "stateSolveLevel.h"
#include "Logger.h"
#include "stateStartMenu.h"

using namespace sf;
using namespace applogger;

/*"Silnik" gry*/
//typedef void (*addLetterF)(wchar_t w);
class AppEngine
{
public:
	AppEngine();
	AppEngine(AppEngine& game) = delete;
	~AppEngine();

	//Aktualizowanie
	void Update();
	//Rysowanie
	void Render();

	bool IsRunning();
	bool HasFocus();

	RenderWindow* getWindow();
	void PollEvents();
private:
	

	RenderWindow* window = nullptr;
	//Event* events = nullptr;

	//Textury do menu

	Resources res;
	Clock clock;
	
	//Co ma byæ wyœwietlane w sensie ktore menu
	//short gameMenuState;
	State* gameState = nullptr;

	Logger* logger;
	bool hasFocus;
};

//static void (Game::* addLetter)(wchar_t w) = nullptr;