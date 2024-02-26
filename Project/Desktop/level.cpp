#include "level.h"

Level::Level()
{
	id = "";
	name = "";
	desc = "";

	realized = false;

	nextLevels = nullptr;
	nextLevelsCount = 0;

	prevLevels = nullptr;
	prevLevelsCount = 0;
	pathToSave = "save.asc";

	componentCount = 0;
	components = nullptr;

	initBoardFun = [](Board* board)->bool {
		return false;
		};

	schematicPath = "";
}

Level::Level(std::string id, sf::String name, sf::String desc, bool realized, sf::String shortDesc)
	:id(id), name(name), desc(desc), shortDesc(shortDesc), realized(realized), nextLevelsCount(0), prevLevelsCount(0)
{
	nextLevels = nullptr;
	prevLevels = nullptr;
	pathToSave = "";

	componentCount = 0;
	components = nullptr;

	boardDimnesion = { 12, 18, 1 };

	initBoardFun = [](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };

		Component* vcc = new Goldpin(L"Z³¹cze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Z³¹cze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;

		//Vcc
		board->placeComponentForce(vcc, pinPos);

		//GND
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);

		return true;
		};

	initComponentsDescFun = [](Component** c, const int& d) {
		return std::map<int, ComponentDesc>();
		};
	schematicPath = "";
}

Level::~Level()
{
	//delete[] nextLevels;
	//delete[] prevLevels;
}

std::string Level::getId()
{
	return id;
}

sf::String& Level::getName()
{
	return name;
}

sf::String& Level::getDesc()
{
	return desc;
}

sf::String& Level::getShortDesc()
{
	return shortDesc;
}

const bool Level::isRealized() const
{
	return realized;
}

bool Level::canRealize()
{
	for (int i = 0; i < prevLevelsCount; i++)
		if (!prevLevels[i]->isRealized())
			return false;
	
	return true;
}

bool Level::canRealize(std::map<std::string, Level*>&levels)
{
	for (auto& levelId : prevLevelsIds)
	{
		if (!levels[levelId]->isRealized())
			return false;
	}
	return true;
}

void Level::setPrevLevels(Level** levels, int levelsCount)
{
	prevLevels = levels;
	prevLevelsCount = levelsCount;
}

void Level::setPrevLevelsIds(std::vector<std::string> prevLevelsIds)
{
	this->prevLevelsIds = prevLevelsIds;
}

std::vector<std::string>& Level::getPrevLevelsIds()
{
	return this->prevLevelsIds;
}

void Level::setComponentsGraphicsToLoad(std::function<void()> componentsGraphicsToLoad)
{
	this->componentsGraphicsToLoad = componentsGraphicsToLoad;
}

void Level::setGenerateComponents(std::function<Component** (int*)> genComponents)
{
	this->genComponents = genComponents;
}

Component** Level::getComponents()
{
	return components;
}

int Level::getComponentsCount()
{
	return componentCount;
}

void Level::setCheckSimulation(std::function<bool(Board*, SimulationEngine*)> checkSimulation)
{
	this->checkSimulationFun = checkSimulation;
}

void Level::setCheckBoard(std::function<bool(Board*)> checkBoardFun)
{
	this->checkBoardFun = checkBoardFun;
}

bool Level::checkBoard(Board* board)
{
	return checkBoardFun(board);
}

bool Level::checkSimulation(Board* board, SimulationEngine* sim)
{
	return checkSimulationFun(board, sim);
}

void Level::load()
{
	componentsGraphicsToLoad();
	GraphicManager::GetInstance().loadComponentGraphics();
	components = genComponents(&componentCount);
	cout << componentsDesc.size();

	initComponentsDesc();
	cout << componentsDesc.size();

	//for (auto& a : componentsDesc)
	//{
	//	GraphicManager::GetInstance().loadComponentDescription(a.second.getPath());
	//}
}

void Level::setPathToSave(std::string path)
{
	pathToSave = path;
}

const std::string& Level::getPathToSave()
{
	return pathToSave;
}

void Level::loadRealizedLevels(map<string, Level*>& lev)
{
	std::ifstream file(".\\save\\saves", std::ios::in | std::ios::binary);
	if (!file.good())
		throw std::string("Error with save file");

	uint64_t pos = 0;

	char data[64];

	file.read(data, 64);
	if (file.gcount() != 64)
		throw  std::string("Bad save file! At pos: ") + to_string(pos);
	pos += 64;

	DataRow savedSavesCount = { 0 };
	savedSavesCount.da[3] = data[63];
	savedSavesCount.da[2] = data[62];
	savedSavesCount.da[1] = data[61];
	savedSavesCount.da[0] = data[60];

	if (savedSavesCount.data == 0)
		return;
	
	DataRow saveId = {0};
	DataRow saveFlags = {0};
	DataRow saveSize = {0};
	file.seekg(pos);


	for(uint64_t i = 0; i < savedSavesCount.data; i++)
	{
		file.read(saveId.da, 8);
		std::streamsize t = file.gcount();
		if (t != 8)
			throw  std::string("Bad save file! At pos: ") + to_string(pos);
		pos += 8;

		file.read(saveFlags.da, 1);
		if (file.gcount() != 1)
			throw  std::string("Bad save file! At pos: ") + to_string(pos);
		pos += 1;

		file.read(saveSize.da, 8);
		if (file.gcount() != 8)
			throw std::string("Bad save file! At pos: ") + to_string(pos);
		pos += 8;

		pos += saveSize.data;
		map<string, Level*>::iterator it = lev.find(string(saveId.da));
		
		file.seekg(pos);

		if (it == lev.end())
			continue;

		it->second->realized = 1;
	}

	file.close();
}

void Level::saveRealizedLevel(std::string levelId, uint8_t flags)
{
	std::fstream savesDB(".\\save\\saves",  std::ios::in | std::ios::binary);
	std::fstream saveL(".\\save\\save.asc", std::ios::in | std::ios::binary);
	std::fstream savesDBTemp(".\\save\\savesTemp", std::ios::out | std::ios::binary);
	
	if (!(savesDB.good() && saveL.good() && savesDBTemp.good()))
		throw std::string("Error with save file");

	unsigned int pos = 0;
	char data[64] = { 0 };

	savesDB.read(data, 64);
	if (savesDB.gcount() != 64)
		throw "Bad save file! At pos: " + to_string(pos);
	pos += 64;

	DataRow savedSavesCount = { 0 };
	savedSavesCount.da[3] = data[63];//not sure
	savedSavesCount.da[2] = data[62];
	savedSavesCount.da[1] = data[61];
	savedSavesCount.da[0] = data[60];

	savesDBTemp.write(data, 64);
	//savedSavesCount.data++;
	//savesDBTemp.write(savedSavesCount.da, 4);
	uint64_t forCounter = savedSavesCount.data;
	//copy levels
	for (uint64_t i = 0; i < forCounter; i++)
	{
		char dataId[8];
		savesDB.read(dataId, 8);
		//if (dataId == levelId.c_str()) // mayby add \n to dataId
		if(levelId.find(dataId, 0) != std::string::npos)
		{
			
			savedSavesCount.data--;
			continue;
		}

		char dataFlags[1];
		savesDB.read(dataFlags, 1);

		//char dataLevelSize[8];
		DataRow levelSize = { 0 };
		savesDB.read(levelSize.da, 8);

		savesDBTemp.write(dataId, 8);
		savesDBTemp.write(dataFlags, 1);
		savesDBTemp.write(levelSize.da, 8);

		char additionalBuffer[1024] = { 0 };
		for (size_t i = 0; i < levelSize.data; i+=1024)
		{
			savesDB.read(additionalBuffer, (levelSize.data < 1024) ? (levelSize.data % 1024) : 1024);
			savesDBTemp.write(additionalBuffer, savesDB.gcount());
		}
	}


	//add new level
	DataRow levelIdRaw = { 0 };
	for (int i = 0; i < 8; i++)
	{
		levelIdRaw.da[i] = (levelId.length() - i > 0) ? levelId.c_str()[i] : 0 ;
	}

	savesDBTemp.write(levelIdRaw.da, 8); // id
	char flagRaw[1] = { 0 };
	savesDBTemp.write(flagRaw, 1); // flag

	std::streampos savePos = 0;
	savePos = saveL.tellg();
	saveL.seekg(0, std::ios::end);
	savePos = saveL.tellg() - savePos;
	saveL.seekg(0, std::ios::beg);

	DataRow levelSizeRaw = { 0 };
	levelSizeRaw.data = savePos;
	savesDBTemp.write(levelSizeRaw.da, 8); // size
	//cout << levelSizeRaw.data;

	char buffer[1024] = { 0 };
	while (!saveL.eof())
	{
		saveL.read(buffer, 1024);
		savesDBTemp.write(buffer, saveL.gcount());
	}

	savedSavesCount.data++;
	savesDBTemp.seekp(60);
	savesDBTemp.write(savedSavesCount.da, 4);

	savesDB.close();
	savesDBTemp.close();
	saveL.close();


	//Copy to diffrent file
	savesDB.open(".\\save\\saves", std::ios::out | std::ios::binary);
	savesDBTemp.open(".\\save\\savesTemp", std::ios::in | std::ios::binary);

	while (!savesDBTemp.eof())
	{
		savesDBTemp.read(buffer, 1024);
		savesDB.write(buffer, savesDBTemp.gcount());
	}

	savesDB.close();
	savesDBTemp.close();

	//Clean unnecesary file
	savesDBTemp.open(".\\save\\savesTemp", std::ios::out | std::ios::binary);
	char cll[1] = { ' ' };
	savesDBTemp.write(cll, 1);
	savesDBTemp.close();
}

void Level::extractRelizedLevel(std::string levelId)
{
	std::ifstream file(".\\save\\saves", std::ios::in | std::ios::binary);
	if (!file.good())
		throw std::string("Error with save file");

	unsigned int pos = 0;

	char data[64];

	file.read(data, 64);
	if (file.gcount() != 64)
		throw  std::string("Bad save file! At pos: ") + to_string(pos);
	pos += 64;

	DataRow savedSavesCount = { 0 };
	savedSavesCount.da[3] = data[63];
	savedSavesCount.da[2] = data[62];
	savedSavesCount.da[1] = data[61];
	savedSavesCount.da[0] = data[60];

	if (savedSavesCount.data == 0)
		return;

	DataRow saveId = { 0 };
	DataRow saveFlags = { 0 };
	DataRow saveSize = { 0 };
	file.seekg(pos);

	bool saveDataFound = false;

	for (uint64_t i = 0; i < savedSavesCount.data; i++)
	{
		file.read(saveId.da, 8);
		unsigned int t = file.gcount();
		if (t != 8)
			throw  std::string("Bad save file! At pos: ") + to_string(pos);
		pos += 8;

		file.read(saveFlags.da, 1);
		if (file.gcount() != 1)
			throw  std::string("Bad save file! At pos: ") + to_string(pos);
		pos += 1;

		file.read(saveSize.da, 8);
		if (file.gcount() != 8)
			throw std::string("Bad save file! At pos: ") + to_string(pos);
		pos += 8;

		if (levelId.find(saveId.da, 0) != std::string::npos)
		{
			saveDataFound = true;
			break;
		}

		pos += saveSize.data;

		file.seekg(pos);
	}

	if (!saveDataFound)
	{
		file.close();
		throw std::string("Can't find data to extract!");
	}
	
	std::fstream saveL(".\\save\\save.asc", std::ios::out | std::ios::binary);
	if (!saveL.good())
		throw std::string("Can't open save.asc file!");
	
	char* dataTem = new char[saveSize.data];

	file.read(dataTem, saveSize.data);
	saveL.write(dataTem, saveSize.data);


	delete[] dataTem;
	saveL.close();
	file.close();
}

int Level::getRealizedLevelsCount(std::map<string, Level*>& levelsMap)
{
	int realizedLevels = 0;
	for (auto& level : levelsMap)
		realizedLevels += level.second->isRealized();
	return realizedLevels;
}

void Level::logRealizedLevels(std::map<string, Level*>& lev)
{
	cout << "Leves:" << endl;
	for (auto& level : lev)
	{
		cout << level.first << " " << level.second->getId() << " " << level.second->isRealized() << endl;
	}
}

void Level::setBoardDimension(sf::Vector3i boardDimension)
{
	this->boardDimnesion = boardDimension;
}

Vector3i Level::getBoardDimension()
{
	return this->boardDimnesion;
}


void Level::setInitComponentsDesc(std::function<std::map<int, ComponentDesc>(Component** components, const int& componentCount)> initComponentsDescFun)
{
	this->initComponentsDescFun = initComponentsDescFun;
}

void Level::initComponentsDesc()
{
	componentsDesc = initComponentsDescFun(components, componentCount);
}

std::map<int, ComponentDesc>& Level::getComponentsDesc()
{
	return componentsDesc;
}

void Level::setInitBoardFun(std::function<bool(Board*)> initBoardFun)
{
	this->initBoardFun = initBoardFun;
}


bool Level::initBoard(Board* board)
{
	return initBoardFun(board);
}