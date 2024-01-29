#include "boardSave.h"

//BoardSave* BoardSave::boardSave = nullptr;


BoardSave::BoardSave()
{
	logger = new Logger("BoardSave");
}

BoardSave::~BoardSave()
{
	delete logger;
}

BoardSave& BoardSave::getInstance()
{
	/*if (boardSave == nullptr)
		boardSave = new BoardSave();
	return boardSave;*/
	static BoardSave instance;
	return instance;
}

void BoardSave::saveBoard(Board* board, const std::string& path)
{
	std::fstream file("save/"+path, ios::out);
	if (!file.good())
	{
		logger->Error("Can't create savefile!");
		throw "Can't create savefile!";
	}

	file << "Version 4\n";
	file << "SHEET 1 " + to_string(board->length) + " " + to_string(board->width) + "\n"; // board dimension

	for (int j = 0; j < board->width; j++)
	{
		for (int i = 0; i < board->length; i++)
		{
			short route = board->boardTiles[j * board->length + i].getRoute();
			if (route & 1) // South
				file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16) << " " << to_string(j * 16  + 8) << "\n";

			if (route & 2) // West
				file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16 - 8) << " " << to_string(j * 16) << "\n";

			if (route & 4) // North
				file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16) << " " << to_string(j * 16 - 8) << "\n";

			if (route & 8)  // East
				file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16 + 8) << " " << to_string(j * 16) << "\n";
		}
	}

	//Add voltage
	file << "FLAG 16 0 0\n"; // GND
	file << "SYMBOL _app\\myvoltage 0 0 R270\n";
	file << "SYMATTR InstName V1\n";
	file << "SYMATTR Value 12\n";

	//Add components
	for (Component* a : board->components)
	{
		//cout << "id: " << a->getId() << " sym:" << a->getSimSymbol() << endl;
		if (a->getSimSymbol() == "goldpin")
			continue;
		Vector2i boardPos = a->getBoardPosition();
		int rotation = a->getRotation();

		file << "SYMBOL " << a->getSimSymbol() << " " << to_string(boardPos.x * 16) << " " << to_string(boardPos.y * 16) << " R" << to_string(rotation * 90) << "\n";
		file << "SYMATTR InstName " << a->getSimName() << "\n";
		file << "SYMATTR Value " << a->getSimValue() << "\n";
	}

	//Simulation config
	file << "TEXT -28 -20 Left 2 !.tran 10\n";

	file.close();
	logger->Info("Created save file: " + path);
}

Board* BoardSave::loadBoard(const std::string& path, Level* level)
{
	fstream file("save/" + path, ios::in);
	if (!file.good())
	{
		logger->Error("Cant open save file: " + path);
		return nullptr;
	}
	char buffer[512];
	//Load wires
	std::string strBuffer;
	

	file.getline(buffer, 512, '\n'); // Version
	//file.getline(buffer, 128, '\n'); // Sheet
	file >> strBuffer;
	if (strBuffer != "SHEET")
	{
		logger->Error("Bad file!");
		return nullptr;
	}
	
	Vector2i boardSize;
	file >> boardSize.x; // layers (now dump)
	file >> boardSize.x;
	file >> boardSize.y;

	logger->Info("Board dimension: " + to_string(boardSize.x) + " " + to_string(boardSize.y));

	Board* board = new Board(boardSize.x, boardSize.y, 1);
	
	Vector2i pos0;
	Vector2i pos1;

	//while (file >> strBuffer)
	//{
	//	if (strBuffer != "WIRE")
	//		break;
	//	file >> pos0.x;
	//	file >> pos0.y;
	//	file >> pos1.x;
	//	file >> pos1.y;
	//	//board->addRoute(pos0, pos1);
	//	board->setRoute(pos0, pos1);
	//}

	//file.getline(buffer, 512, '\n'); // Gnd flag
	std::string ss;
	do
	{
		file >> ss;

		if (ss == "WIRE")
		{
			file >> pos0.x;
			file >> pos0.y;
			file >> pos1.x;
			file >> pos1.y;

			//pos0.x = pos0.x / 16;
			//pos0.y = pos0.y / 16;
			//pos1.x = pos1.x / 16;
			//pos1.y = pos1.y / 16;

			board->setRoute(pos0, pos1);
		}
	} while (ss != "FLAG" && !file.eof());

	file.getline(buffer, 512, '\n'); // Vcc and gnd
	file.getline(buffer, 512, '\n'); // Vcc and gnd
	file.getline(buffer, 512, '\n'); // Vcc and gnd


	//Add Vcc
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


	Component** levelComponents = level->getComponents();

	while (!file.eof())
	{
		file >> ss;
		if (ss == "TEXT")
			break;
		else if (ss == "SYMBOL")
		{
			std::string simSymbol;
			std::string simName;
			std::string rotation;
			Vector2i pos;
			char rotate = 0;
			int value = 0;

			file >> simSymbol; //simName
			file >> pos.x; //pos x
			file >> pos.y; //pos y

			file >> rotation; //rotate

			file >> ss; //symattr
			file >> ss; //instname
			file >> simName; //


			file >> ss; //symattr
			file >> ss; //value text
			file >> value; //value

			//if(simName == "_app\\appres")
			//cout << simName << endl;
			std::string id = simName.substr(simName.find("id"));
			for (int i = 0; i < level->getComponentsCount(); i++)
			{
				if (levelComponents[i]->getId() == id)
				{
					Component* addComponent;
					if (dynamic_cast<Resistor*>(levelComponents[i]))
						addComponent = new Resistor(dynamic_cast<Resistor*>(levelComponents[i]));
					else if (dynamic_cast<LightEmittingDiode*>(levelComponents[i]))
						addComponent = new LightEmittingDiode(dynamic_cast<LightEmittingDiode*>(levelComponents[i]));
					else
						addComponent = new Component(levelComponents[i]);

					pos.x = pos.x / 16;
					pos.y = pos.y / 16;
					
					for (int i = 0; i < stoi(rotation.substr(1)) / 90; i++)
						addComponent->rotate();
					
					board->placeComponent(addComponent, pos);
					//place compoenent
				}
			}
		}
	}

	file.close();

	return board;
}