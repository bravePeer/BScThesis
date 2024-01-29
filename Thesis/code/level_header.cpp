class Level {
public:
	void setComponentsGraphicsToLoad(std::function<void()> componentsGraphicsToLoad);
	void setGenerateComponents(std::function<Component**(int*)> genComponents);
	Component** getComponents();
	int getComponentsCount();
	void setCheckSimulation(std::function<bool(Board*)> checkSimulation);
	void setCheckBoard(std::function<bool(Board*)> checkBoardFun);
	bool checkBoard(Board* board);
	bool checkSimulation(Board* board);
	void load();

	void setInitBoardFun(std::function<bool(Board*)> initBoardFun);
	bool initBoard(Board* board);
private:

	Level** nextLevels;
	int nextLevelsCount;

	Level** prevLevels;
	int prevLevelsCount;
	std::vector<std::string> prevLevelsIds;

	std::function<void()> componentsGraphicsToLoad;
	std::function<Component** (int*)> genComponents;
	std::function<bool(Board*)> initBoardFun;

	Component** components;
	int componentCount;

	std::function<bool(Board*)> checkSimulationFun;
	std::function<bool(Board*)> checkBoardFun;

	Vector3i boardDimnesion;

	std::string pathToSave;

	union DataRow {
		uint64_t data;
		char da[8];
	};
};
