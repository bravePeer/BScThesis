#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"
#include <fstream>

//#define TILE_LENGTH 64
//#define TILE_WIDTH 32

class Tile
{
public:
	enum TileState //TODO s�aba nazwa
	{
		EMPTY,
		SMD_PAD,
		THT_PAD,
		ROUTED,
		VIA,
		CONNECTED // not sure is necessary
	};
	enum RouteDirection {
		S = 0x01,
		W = 0x02,
		N = 0x04,
		E = 0x08
	};

	Tile()
	{
		// 
		//
		//      1
		//     / \
		//  W /   \ N
		//   /     \
		//  0       2
		//   \     /
		//  S \   / E
		//     \ /
		//      3
		//

		float w = sqrtf((TILE_LENGTH * TILE_LENGTH) + (TILE_WIDTH * TILE_WIDTH));
		tileShape.setPointCount(4);
		tileShape.setPoint(0, Vector2f(0.f, TILE_WIDTH / 2.f));
		tileShape.setPoint(1, Vector2f(TILE_LENGTH / 2.f, 0.f));
		tileShape.setPoint(2, Vector2f(TILE_LENGTH, TILE_WIDTH / 2.f));
		tileShape.setPoint(3, Vector2f(TILE_LENGTH / 2.f, TILE_WIDTH));

		tileShape.setFillColor(defaultColor);
		//tileShape.setOutlineThickness(2.f);

		/*tileShape.setTexture(GraphicAll::GetInstance()->getTileTexture());
		tileShape.setTextureRect(IntRect(0,0,64,48));*/
		tileSprite = GraphicAll::GetInstance()->getTileSprite(0);
		tileRoute = GraphicAll::GetInstance()->getRouteGraphic();

		route = 0;
	}

	void SetPos(Vector2f pos)
	{
		tileShape.setPosition(pos);
	}
	void Move(Vector2f pos)
	{
		tileShape.move(pos);
	}

	Vector2f GetGlobalPointPos(size_t point)
	{
		return Vector2f(tileShape.getPosition().x + tileShape.getPoint(point).x, tileShape.getPosition().y + tileShape.getPoint(point).y);
	}

	bool isMouseHover(Vector2i& mousePos)
	{
		//
		//      1
		//     / \
		//    /   \
		//   /     \
		//  0       2
		//   \     /
		//    \   /
		//     \ /
		//      3
		//

		auto containLine01 = [=](Vector2i& mousePos) -> bool {
			float t = (GetGlobalPointPos(1).x - GetGlobalPointPos(0).x) *
				(mousePos.y - GetGlobalPointPos(0).y) - 
				(GetGlobalPointPos(1).y - GetGlobalPointPos(0).y) *
				(mousePos.x - GetGlobalPointPos(0).x);
			return t > 0 ? true : false;
		};
		
		auto containLine12 = [=](Vector2i& mousePos) -> bool {
			float t = (GetGlobalPointPos(1).x - GetGlobalPointPos(2).x) *
				(mousePos.y - GetGlobalPointPos(2).y) -
				(GetGlobalPointPos(1).y - GetGlobalPointPos(2).y) *
				(mousePos.x - GetGlobalPointPos(2).x);
			return t <= 0 ? true : false;
		};
		
		auto containLine23 = [=](Vector2i& mousePos) -> bool {
			float t = (GetGlobalPointPos(3).x - GetGlobalPointPos(2).x) *
				(mousePos.y - GetGlobalPointPos(2).y) -
				(GetGlobalPointPos(3).y - GetGlobalPointPos(2).y) *
				(mousePos.x - GetGlobalPointPos(2).x);
			return t >= 0 ? true : false;
		};
		
		auto containLine30 = [=](Vector2i& mousePos) -> bool {
			float t = (GetGlobalPointPos(3).x - GetGlobalPointPos(0).x) *
				(mousePos.y - GetGlobalPointPos(0).y) -
				(GetGlobalPointPos(3).y - GetGlobalPointPos(0).y) *
				(mousePos.x - GetGlobalPointPos(0).x);
			return t < 0 ? true : false;
		};

		if (containLine01(mousePos) && containLine23(mousePos)
			&& containLine12(mousePos) && containLine30(mousePos))
		{
			return true;
		}
		return false;
	}

	void Update(RenderWindow* window, Time* elapsed)
	{
	}
	void Render(RenderTarget* target)
	{
		//if (route & 0x01) //South
		//	target->draw(*tileRoute[0]);
		//if (route & 0x02) //West
		//	target->draw(*tileRoute[1]);
		//if (route & 0x04) //North
		//	target->draw(*tileRoute[2]);
		//if (route & 0x08) //East
		//	target->draw(*tileRoute[3]);

		//Raczej niezbyt wydajne
		switch (route)
		{
		case 1: // South
			tileRoute->setTextureRect(sf::IntRect(66, 0, -66, 33));
			break;
		case 2: // West
			tileRoute->setTextureRect(sf::IntRect(66, 33, -66, -33));
			break;
		case 4: // North
			tileRoute->setTextureRect(sf::IntRect(0, 33, 66, -33));
			break;
		case 8: // East
			tileRoute->setTextureRect(sf::IntRect(0, 0, 66, 33));
			break;

		case 12: // NE
			tileRoute->setTextureRect(sf::IntRect(66, 0, 66, 33));
			break;
		case 3: //SW
			tileRoute->setTextureRect(sf::IntRect(132, 0, -66, 33));
			break;

		case 6: //NW
			tileRoute->setTextureRect(sf::IntRect(132, 33, 66, -33));
			break;
		case 9: //SE
			tileRoute->setTextureRect(sf::IntRect(132, 0, 66, 33));
			break;

		case 10: //WE
			tileRoute->setTextureRect(sf::IntRect(198, 0, 66, 33));
			break;
		case 5: //NS
			tileRoute->setTextureRect(sf::IntRect(264, 0, -66, 33));
			break;

		case 14: // not S
			tileRoute->setTextureRect(sf::IntRect(330, 33, -66, -33));
			break;
		case 13: // not W
			tileRoute->setTextureRect(sf::IntRect(330, 0, -66, 33));
			break;
		case 11: // not N
			tileRoute->setTextureRect(sf::IntRect(264, 0, 66, 33));
			break;
		case 7:  // not E
			tileRoute->setTextureRect(sf::IntRect(264, 33, 66, -33));
			break;

		case 15:
			tileRoute->setTextureRect(sf::IntRect(330, 0, 66, 33));
			break;

		default:
			break;
		}

		//tileSprite = GraphicAll::GetInstance()->getTileSprite(tileState);
		tileSprite->setPosition(tileShape.getPosition());
		target->draw(*tileSprite);
		target->draw(tileShape);

		if (route > 0)
		{
			tileRoute->setPosition(tileShape.getPosition());
			target->draw(*tileRoute);
		}
	}
	void setState(TileState tileState)
	{
		this->tileState = tileState;
		tileSprite = GraphicAll::GetInstance()->getTileSprite(tileState);
	}
	void setRoute(char route)
	{
		this->route |= route;
	}
	void removeRoute(char route)
	{
		this->route &= ~route;
	}
	// 4 lesast significant bits are SWNE 
	short getRoute()
	{
		return this->route;
	}
	TileState getState()
	{
		return tileState;
	}
	const sf::String getStateString()
	{
		switch (tileState)
		{
		case Tile::EMPTY:
			return sf::String("Empty");
			break;
		case Tile::SMD_PAD:
			return sf::String("SMD pad");
			break;
		case Tile::THT_PAD:
			return sf::String("THT pad");
			break;
		case Tile::ROUTED:
			return sf::String("Routed");
			break;
		case Tile::VIA:
			return sf::String("Via");
			break;
		case Tile::CONNECTED:
			return sf::String("Conntected");
			break;
		default:
			return sf::String("-");
			break;
		}
	}

private:
		sf::ConvexShape tileShape;
		/*const float length = 60;
		const float width = 30;*/

		sf::Color defaultColor = sf::Color(50, 168, 56, 0);
		//sf::Color hoverColor = sf::Color(61, 224, 58,125);
		//sf::Color routeColor = sf::Color(245, 241, 42);

		TileState tileState = TileState::EMPTY;
		Sprite* tileSprite;

		Sprite* tileRoute; 
		char route; // 4 lesast significant bits are SWNE 
};


class Board
{
	Logger* logger;

	int length; 
	int width;  
	int layers; 
	Vector2f origin;

	Tile* boardTiles;
	vector<Component*> components; //Trzeba b�dzie sortowa� po dodaniu nowego komponentu
	//bool* isComponentOnBoard;
	Component** isComponentOnBoard;
	Component* ghostComponent;

	//Vector2f viewOrigin = { 0.f, 0.f };
	Vector2f viewOrigin = { 500.f, 15.f };

	bool hideComponents = false;
public:
	Board()
	{
		logger = new Logger("Board");
		length = 0;
		width = 0;
		layers = 0;
		boardTiles = nullptr;
	}
	Board(int length, int width, int layers)
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
		Vector2f startPos(0,0);
		Vector2f tmpPos = startPos;
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < length; i++)
			{
				boardTiles[j * length + i].SetPos(ScreenPos({i,j},{TILE_LENGTH, TILE_WIDTH}) + viewOrigin);
				//boardTiles[j * length + i].Move(viewOrigin);
				tmpPos.x += TILE_LENGTH;
			}
			tmpPos.y += TILE_WIDTH / 2.f;
			tmpPos.x = startPos.x - TILE_LENGTH * (j) - TILE_LENGTH/2.f;
		}

		ghostComponent = nullptr;
	}
	~Board()
	{
		delete logger;
		delete[] boardTiles;
		delete[] isComponentOnBoard;
		for (auto component: components)
		{
			delete component;
		}
	}

	void Update(RenderWindow* window, Time* elapsed)
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
	void Render(RenderTarget* target)
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
	Vector2i getHoverTilePos(Vector2i& mouse)
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
	Tile& getTile(Vector2i pos)
	{
		if (pos.x + pos.y * length > length * width)
			throw sf::String("Tile not exists!");

		return boardTiles[pos.x + pos.y * length];
	}
	const Vector2i getBoardDimension()
	{
		return Vector2i(length, width);
	}
	Vector2f& getViewOrigin()
	{
		return viewOrigin;
	}


	char tileNighbourDirection(Vector2i pos0, Vector2i pos1)
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
	void addRoute(Vector2i pos0, Vector2i pos1)
	{
		boardTiles[pos0.x + pos0.y * length].setRoute(tileNighbourDirection(pos0, pos1));
		boardTiles[pos1.x + pos1.y * length].setRoute(tileNighbourDirection(pos1, pos0));
	}
	void removeRoute(Vector2i pos0, Vector2i pos1)
	{
		boardTiles[pos0.x + pos0.y * length].removeRoute(tileNighbourDirection(pos0, pos1));
		boardTiles[pos1.x + pos1.y * length].removeRoute(tileNighbourDirection(pos1, pos0));
	}

	bool canPlaceComponent(Component* component, Vector2i& pos)
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
	void placeComponent(Component* component, Vector2i& pos)
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

			if(component->getComponentType() == Component::ComponentType::SMD)
				boardTiles[buf.x + buf.y * length].setState(Tile::TileState::SMD_PAD);
			else if (component->getComponentType() == Component::ComponentType::THT)
				boardTiles[buf.x + buf.y * length].setState(Tile::TileState::THT_PAD);
		} 

		component->setBoardPosition(pos);
		components.push_back(component);
		logger->Info("Added "+component->getName() + " pos:" + to_string(pos.x) + " " + to_string(pos.y));
	}
	void placeComponentForce(Component* component, Vector2i& pos)
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

			if (component->getComponentType() == Component::ComponentType::SMD)
				boardTiles[buf.x + buf.y * length].setState(Tile::TileState::SMD_PAD);
			else if (component->getComponentType() == Component::ComponentType::THT)
				boardTiles[buf.x + buf.y * length].setState(Tile::TileState::THT_PAD);
		}

		components.push_back(component);
		logger->Info("Added " + component->getName() + " pos:" + to_string(pos.x) + " " + to_string(pos.y));
	}
	void removeComponent(Vector2i pos)
	{
		Component* toRemove = getComponentOnBoard(pos);
		if(!toRemove->isRemovable())
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
	Component* getComponentOnBoard(Vector2i pos)
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
	
	void setHideComponent(bool hide)
	{
		hideComponents = hide;
	}
	bool isHideComponent()
	{
		return hideComponents;
	}

	//Saving board as asc file
	void saveBoard()
	{
		logger->Info("Create save file");
		std::fstream file("save.asc", ios::out);
		if (!file.good())
			throw L"Can't create savefile!";

		file << "Version 4\n";
		file << "SHEET 1 100 4\n";

		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < length; i++)
			{
				short route = boardTiles[j * length + i].getRoute();
				if (route & 1) // South
					file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16 ) << " " << to_string(j * 16 + 8) << "\n";

				if (route & 2) // West
					file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16 - 8) << " " << to_string(j * 16 ) << "\n";

				if (route & 4) // North
					file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16) << " " << to_string(j * 16 - 8) << "\n";

				if (route & 8)  // East
					file << "WIRE " << to_string(i * 16) << " " << to_string(j * 16) << " " << to_string(i * 16 + 8) << " " << to_string(j * 16 ) << "\n";
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
			file << "SYMATTR InstName " << a->getId() << "\n";
			file << "SYMATTR Value " << "1" << "\n";
		}


		//Simulation config
		file << "TEXT -28 -20 Left 2 !.tran 10\n";

		file.close();
	}

	//---------Testing---------
	void printComponentsMap()
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
	void printRoutMap()
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
};