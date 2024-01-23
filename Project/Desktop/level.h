#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "component.h"
#include "board.h"

class Level
{
public:
	Level();
	Level(sf::String id, sf::String name, sf::String desc, bool realized = false);
	~Level();
	
	sf::String& getId();
	sf::String& getName();
	sf::String& getDesc();
	
	const bool isRealized() const ;
	bool canRealize();

	bool canRealize(std::map<std::string, Level*>& levels);

	//void setNextLevels(Level** levels); //moze nie potrzebne
	void setPrevLevels(Level** levels, int levelsCount);
	void setPrevLevelsIds(std::vector<std::string> prevLevelsIds);


	void setGenerateComponents(std::function<Component**(int*)> genComponents);
	Component** getComponents();
	int getComponentsCount();

	//Add simulation
	void setCheckSimulation(std::function<bool(Board*)> checkSimulation);
	//getConditions

	void setCheckBoard(std::function<bool(Board*)> checkBoardFun);
	bool checkBoard(Board* board);
	bool checkSimulation(Board* board);

	//Loads components to memory
	void load();

	void setBoardDimension(sf::Vector3i boardDimension);
	Vector3i getBoardDimension();

	void setInitBoardFun(std::function<bool(Board*)> initBoardFun);
	bool initBoard(Board* board);

	void setPathToSave(std::string path);
	const std::string& getPathToSave();

	static void loadRealizedLevels(map<string, Level*>& lev);
	static void saveRealizedLevel(std::string levelId, uint8_t flags);
	static void extractRelizedLevel(std::string levelId);
private:
	sf::String id;
	sf::String name;
	sf::String desc;

	bool realized;

	Level** nextLevels;
	int nextLevelsCount;

	Level** prevLevels;
	int prevLevelsCount;
	std::vector<std::string> prevLevelsIds;

	std::function<Component** (int*)> genComponents;
	std::function<bool(Board*)> initBoardFun;

	Component** components;
	int componentCount;

	std::function<bool(Board*)> checkSimulationFun;
	std::function<bool(Board*)> checkBoardFun;

	Vector3i boardDimnesion;

	std::string pathToSave;

	union DataRow
	{
		uint64_t data;
		char da[8];
	};
};