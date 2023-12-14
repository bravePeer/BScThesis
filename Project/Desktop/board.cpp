#include "board.h"

Board::Board()
{
	logger = new Logger("Board");
	length = 0;
	width = 0;
	layers = 0;
	boardTiles = nullptr;
}

Board::Board(int length, int width, int layers)
	:length(length), width(width), layers(layers)
{
	logger = new Logger("Board");
	origin = { 0,0 };

	boardTiles = new Tile[length * width];
	//isComponentOnBoard = new bool[length * width];
	isComponentOnBoard = new Component * [length * width];

	for (int i = 0; i < length * width; i++)
		isComponentOnBoard[i] = nullptr;


	//Vector2f startPos(10 + TILE_LENGTH * (length - 1), TILE_WIDTH + 10);
	Vector2f startPos(0, 0);
	Vector2f tmpPos = startPos;
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < length; i++)
		{
			boardTiles[j * length + i].SetPos(ScreenPos({ i,j }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin);
			//boardTiles[j * length + i].Move(viewOrigin);
			tmpPos.x += TILE_LENGTH;
		}
		tmpPos.y += TILE_WIDTH / 2.f;
		tmpPos.x = startPos.x - TILE_LENGTH * (j)-TILE_LENGTH / 2.f;
	}

	ghostComponent = nullptr;
}

Board::~Board()
{
	delete logger;
	delete[] boardTiles;
	delete[] isComponentOnBoard;
	for (auto component : components)
	{
		delete component;
	}
}

void Board::Update(RenderWindow* window, Time* elapsed)
{
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < length; i++)
		{
			boardTiles[j * length + i].Update(window, elapsed);
		}
	}

	for (auto component : components)
	{
		component->Update(window, elapsed, viewOrigin);
	}
}

void Board::Render(RenderTarget* target)
{
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < length; i++)
		{
			boardTiles[j * length + i].Render(target);
		}
	}

	if (!hideComponents)
	{
		for (auto component : components)
		{
			component->Render(target);
		}
	}
}

Vector2i Board::getHoverTilePos(Vector2i& mouse)
{
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < length; i++)
		{
			if (boardTiles[j * length + i].isMouseHover(mouse))
				return Vector2i(i, j);
		}
	}
	throw sf::String(L"Poza granicami p�ytki");
}

Tile& Board::getTile(Vector2i pos)
{
	if (pos.x + pos.y * length > length * width)
		throw sf::String("Tile not exists!");

	return boardTiles[pos.x + pos.y * length];
}

const Vector2i Board::getBoardDimension()
{
	return Vector2i(length, width);
}

Vector2f& Board::getViewOrigin()
{
	return viewOrigin;
}

char Board::tileNighbourDirection(Vector2i pos0, Vector2i pos1)
{
	if (pos0.x == pos1.x)
		if (pos0.y > pos1.y)
			return Tile::RouteDirection::N;
		else
			return Tile::RouteDirection::S;

	if (pos0.y == pos1.y)
		if (pos0.x > pos1.x)
			return Tile::RouteDirection::W;
		else
			return Tile::RouteDirection::E;
}

void Board::addRoute(Vector2i pos0, Vector2i pos1)
{
	boardTiles[pos0.x + pos0.y * length].setRoute(tileNighbourDirection(pos0, pos1));
	boardTiles[pos1.x + pos1.y * length].setRoute(tileNighbourDirection(pos1, pos0));
}

void Board::setRoute(Vector2i pos0, Vector2i pos1)
{
	boardTiles[(pos0.x / 16) + (pos0.y/16) * length].setRoute(tileNighbourDirection(pos0, pos1));
}

void Board::removeRoute(Vector2i pos0, Vector2i pos1)
{
	boardTiles[pos0.x + pos0.y * length].removeRoute(tileNighbourDirection(pos0, pos1));
	boardTiles[pos1.x + pos1.y * length].removeRoute(tileNighbourDirection(pos1, pos0));
}

bool Board::canPlaceComponent(Component* component, Vector2i& pos)
{
	if (pos.x + component->getTileSize().x > length)
		return false;
	if (pos.y + component->getTileSize().y > width)
		return false;

	for (int j = 0; j < component->getTileSize().y; j++)
	{
		for (int i = 0; i < component->getTileSize().x; i++)
		{
			if (isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] != nullptr)
				return false;
		}
	}

	return true;
}

void Board::placeComponent(Component* component, Vector2i& pos)
{
	if (!canPlaceComponent(component, pos))
		throw sf::String(L"Can't place component! pos: ") + to_wstring(pos.x) + " " + to_wstring(pos.y);

	for (int j = 0; j < component->getTileSize().y; j++)
	{
		for (int i = 0; i < component->getTileSize().x; i++) //j * length + i
		{
			//isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = true;
			isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = component;
		}
	}

	Vector2i* padsPos = component->getPadsPos();
	for (int i = 0; i < component->getPadsCount(); i++)
	{
		Vector2i buf = component->getBoardPosition() + padsPos[i];

		if (component->getComponentTypePackage() == Component::ComponentTypePackage::SMD)
			boardTiles[buf.x + buf.y * length].setState(Tile::TileState::SMD_PAD);
		else if (component->getComponentTypePackage() == Component::ComponentTypePackage::THT)
			boardTiles[buf.x + buf.y * length].setState(Tile::TileState::THT_PAD);
	}

	component->setBoardPosition(pos);
	components.push_back(component);
	logger->Info("Added " + component->getName() + " pos:" + to_string(pos.x) + " " + to_string(pos.y));
}

void Board::placeComponentForce(Component* component, Vector2i& pos)
{
	if (!canPlaceComponent(component, pos))
		throw sf::String(L"Can't place component! pos: ") + to_wstring(pos.x) + " " + to_wstring(pos.y);

	for (int j = 0; j < component->getTileSize().y; j++)
	{
		for (int i = 0; i < component->getTileSize().x; i++) //j * length + i
		{
			//isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = true;
			isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = component;
		}
	}

	component->setBoardPosition(pos);

	Vector2i* padsPos = component->getPadsPos();
	for (int i = 0; i < component->getPadsCount(); i++)
	{
		Vector2i buf = pos;

		if (component->getComponentTypePackage() == Component::ComponentTypePackage::SMD)
			boardTiles[buf.x + buf.y * length].setState(Tile::TileState::SMD_PAD);
		else if (component->getComponentTypePackage() == Component::ComponentTypePackage::THT)
			boardTiles[buf.x + buf.y * length].setState(Tile::TileState::THT_PAD);
	}

	components.push_back(component);
	logger->Info("Added " + component->getName() + " pos:" + to_string(pos.x) + " " + to_string(pos.y));
}

void Board::removeComponent(Vector2i pos)
{
	Component* toRemove = getComponentOnBoard(pos);
	if (!toRemove->isRemovable())
		throw sf::String("This component can not be removed");

	if (toRemove == nullptr)
		throw sf::String("No Component");

	vector<Component*> tmp;
	for (auto component : components)
	{
		if (component == toRemove)
		{
			continue;
		}
		tmp.push_back(component);
	}

	for (int j = 0; j < toRemove->getTileSize().y; j++)
	{
		for (int i = 0; i < toRemove->getTileSize().x; i++) //j * length + i
		{
			//isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = true;
			isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = nullptr;
		}
	}

	Vector2i* padsPos = toRemove->getPadsPos();
	for (int i = 0; i < toRemove->getPadsCount(); i++)
	{
		Vector2i buf = toRemove->getBoardPosition() + padsPos[i];
		boardTiles[buf.x + buf.y * length].setState(Tile::TileState::EMPTY);
	}

	components = tmp;
	delete toRemove;
}

Component* Board::getComponentOnBoard(Vector2i pos)
{

	return isComponentOnBoard[pos.x + pos.y * length];

	/*for (auto& component : components)
	{
		if (pos == component->getBoardPosition())
		{
			return component;
		}
	}*/
}

map<Component::CompoenetType, int> Board::getComponentsCount()
{
	map<Component::CompoenetType, int> componentsCount;

	for (Component* component : components)
	{
		Component::CompoenetType componentType = component->getComponentType();

		map<Component::CompoenetType, int>::iterator it = componentsCount.find(componentType);
		cout << (int)componentType << endl;
		if (it == componentsCount.end())
		{
			componentsCount[componentType] = 1;
		}
		else
		{
			componentsCount[componentType]++;
		}
		//cout << componentsCount[componentType] << endl;
	}


	return componentsCount;
}

void Board::setHideComponent(bool hide)
{
	hideComponents = hide;
}

bool Board::isHideComponent()
{
	return hideComponents;
}

void Board::saveBoard()
{
	logger->Info("Create save file");
	std::fstream file("save/save.asc", ios::out);
	if (!file.good())
		throw L"Can't create savefile!";

	file << "Version 4\n";
	file << "SHEET 1 " + to_string(length) + " " + to_string(width) + "\n"; // board dimension


	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < length; i++)
		{
			short route = boardTiles[j * length + i].getRoute();
			if (route & 1) // South
				file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16) << " " << to_string(j * 16 + 8) << "\n";

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
	for (Component* a : components)
	{
		if (a->getSimName() == "goldpin")
			continue;
		Vector2i boardPos = a->getBoardPosition();
		int rotation = a->getRotation();

		file << "SYMBOL " << a->getSimName() << " " << to_string(boardPos.x * 16) << " " << to_string(boardPos.y * 16) << " R" << to_string(rotation * 90) << "\n";
		file << "SYMATTR InstName " << a->getSimName() << "\n";
		file << "SYMATTR Value " << "1" << "\n";
	}


	//Simulation config
	file << "TEXT -28 -20 Left 2 !.tran 10\n";

	file.close();
}

void Board::loadBoard(const std::string& path)
{
	fstream file(path, ios::in);
	if (!file.good())
	{
		logger->Error("Cant open save file: " + path);
		return;
	}
	char buffer[128];

	file.getline(buffer, 128, '\n'); // Version
	file.getline(buffer, 128, '\n'); // Sheet

	//Load wires
	std::string wire;
	Vector2i pos0;
	Vector2i pos1;

	while (file >> wire)
	{
		if (wire != "WIRE")
			break;
		file >> pos0.x;
		file >> pos0.y;
		file >> pos1.x;
		file >> pos1.y;
		addRoute(pos0, pos1);
	}


	file.close();
}

void Board::printComponentsMap()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < length; j++)
		{
			cout << (isComponentOnBoard[j + i * length] != nullptr) ? '1' : '0';
		}
		cout << endl;
	}
}

void Board::printRoutMap()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < length; j++)
		{
			cout << boardTiles[j + i * length].getRoute() << " ";
		}
		cout << endl;
	}
}





