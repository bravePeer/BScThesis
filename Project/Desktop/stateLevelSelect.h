#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "gui.h"
#include "board.h"
#include "level.h"

#include "state.h"
#include "stateCloseMenu.h"
#include "stateLevelSelect.h"
#include "stateLoginMenu.h"
#include "stateRegisterMenu.h"
#include "stateStartMenu.h"
#include "stateSolveLevel.h"
#include "levelsToLoad.h"
#include "user.h"

class LevelSelect : public State
{
public:
	LevelSelect() = delete;
	LevelSelect(Resources* res);
	LevelSelect(LevelSelect&) = delete;
	~LevelSelect();

	void Update(sf::RenderWindow* window, sf::Time* elapsed);
	void Render(sf::RenderTarget* target);
	//void AddLetter(wchar_t s) {}
private:

	//Level** levels;
	//int levelsCount;

	Button* backButton;
	//Button* resetViewButton;

	//Level info box
	Button* loadSaveButton;
	Button* startNewSaveButton;
	sf::RectangleShape* levelInfoBox;
	TextBox* levelTitle;
	TextBox* levelDesc;

	//Button** levelButtons;
	Level* startLevel;
	
	std::map<std::string, Level*> levelsMap;
	std::map<std::string, Button*> levelButtonsMap;
	std::map<std::string, RectangleShape*> levelButtonsHoverFilterMap;

	enum class LevelInfoState
	{
		Hidden,
		Showed,
		Active
	};
	LevelInfoState levelInfoState;
};