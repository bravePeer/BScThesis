#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "tile.h"
#include "board.h"
#include "level.h"

//Class saving and loading save file
class BoardSave
{
public:
	~BoardSave();

	static BoardSave& getInstance();

	void saveBoard(Board* board, const std::string& path);

	Board* loadBoard(const std::string& path, Level* level);


private:
	BoardSave();
	//static BoardSave* boardSave;
	
	applogger::Logger* logger;
};