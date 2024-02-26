#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "component.h"
#include "board.h"
#include "SimulationEngine.h"
#include "componentDesc.h"

class Level
{
public:
	Level();
	Level(std::string id, sf::String name, sf::String desc, bool realized = false, sf::String shortDesc = L"");
	~Level();
	
	std::string getId();
	sf::String& getName();
	sf::String& getDesc();
	sf::String& getShortDesc();
	
	const bool isRealized() const ;
	bool canRealize();

	bool canRealize(std::map<std::string, Level*>& levels);

	//void setNextLevels(Level** levels); //moze nie potrzebne
	void setPrevLevels(Level** levels, int levelsCount);
	void setPrevLevelsIds(std::vector<std::string> prevLevelsIds);
	std::vector<std::string>& getPrevLevelsIds();

	void setComponentsGraphicsToLoad(std::function<void()> componentsGraphicsToLoad);
	void setGenerateComponents(std::function<Component**(int*)> genComponents);
	Component** getComponents();
	int getComponentsCount();

	//Add simulation
	void setCheckSimulation(std::function<bool(Board*, SimulationEngine*)> checkSimulation);
	//getConditions

	void setCheckBoard(std::function<bool(Board*)> checkBoardFun);
	bool checkBoard(Board* board);
	bool checkSimulation(Board* board, SimulationEngine* sim);

	//Loads components to memory
	void load();

	void setBoardDimension(sf::Vector3i boardDimension);
	Vector3i getBoardDimension();

	void setInitComponentsDesc(std::function<std::map<int, ComponentDesc> (Component** components, const int& componentCount)> initComponentsDescFun);
	void initComponentsDesc();
	std::map<int, ComponentDesc>& getComponentsDesc();

	void setInitBoardFun(std::function<bool(Board*)> initBoardFun);
	bool initBoard(Board* board);

	void setPathToSave(std::string path);
	const std::string& getPathToSave();

	static void loadRealizedLevels(map<string, Level*>& lev);
	static void saveRealizedLevel(std::string levelId, uint8_t flags);
	static void extractRelizedLevel(std::string levelId);

	static int getRealizedLevelsCount(std::map<string, Level*>& lev);
	static void logRealizedLevels(std::map<string, Level*>& lev);

	void setSchematicPath(std::string path)
	{
		schematicPath = path;
	}
	std::string& getSchematicPath()
	{
		return schematicPath;
	}
private:
	sf::String id;
	sf::String name;
	sf::String desc;

	sf::String shortDesc; // Description in level select
	std::string schematicPath;

	bool realized;

	Level** nextLevels;
	int nextLevelsCount;

	Level** prevLevels;
	int prevLevelsCount;
	std::vector<std::string> prevLevelsIds;

	std::function<void()> componentsGraphicsToLoad;
	std::function<Component** (int*)> genComponents;
	std::function<bool(Board*)> initBoardFun;

	std::function<std::map<int, ComponentDesc> (Component** components,const int& componentCount)> initComponentsDescFun;
	std::map<int, ComponentDesc> componentsDesc;

	Component** components;
	int componentCount;

	std::function<bool(Board*, SimulationEngine*)> checkSimulationFun;
	std::function<bool(Board*)> checkBoardFun;

	Vector3i boardDimnesion;

	std::string pathToSave;

	union DataRow
	{
		uint64_t data;
		char da[8];
	};
};