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
	void setCheckSimulation(std::function<void(Board*)> checkSimulation);
	//getConditions

	void setCheckBoard(std::function<bool(Board*)> checkBoardFun);
	bool checkBoard(Board* board);

	//Loads components to memory
	void load();

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
	Component** components;
	int componentCount;

	std::function<void(Board*)> checkSimulation;
	std::function<bool(Board*)> checkBoardFun;

	std::string pathToSave;

	union DataRow
	{
		uint64_t data;
		char da[8];
	};
};