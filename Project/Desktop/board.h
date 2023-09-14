#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"

#define TILE_LENGTH 63
#define TILE_WIDTH 31

class Tile
{
public:
	enum TileState //TODO s³aba nazwa
	{
		EMPTY,
		ROUTED,
		VIA,
		CONNECTED // not sure is necessary
	};

 
	Tile()
	{
		// 
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

		float w = sqrtf((TILE_LENGTH * TILE_LENGTH) + (TILE_WIDTH * TILE_WIDTH));
		tileShape.setPointCount(4);
		tileShape.setPoint(0, Vector2f(0.f, TILE_WIDTH / 2.f));
		tileShape.setPoint(1, Vector2f(TILE_LENGTH / 2.f, 0.f));
		tileShape.setPoint(2, Vector2f(TILE_LENGTH, TILE_WIDTH / 2.f));
		tileShape.setPoint(3, Vector2f(TILE_LENGTH/ 2.f, TILE_WIDTH));

		tileShape.setFillColor(defaultColor);
		tileShape.setOutlineThickness(1.f);

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
			return t < 0 ? true : false;
		};
		
		auto containLine23 = [=](Vector2i& mousePos) -> bool {
			float t = (GetGlobalPointPos(3).x - GetGlobalPointPos(2).x) *
				(mousePos.y - GetGlobalPointPos(2).y) -
				(GetGlobalPointPos(3).y - GetGlobalPointPos(2).y) *
				(mousePos.x - GetGlobalPointPos(2).x);
			return t > 0 ? true : false;
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
		//cout << tileShape.getPosition().x << endl;

		/*auto containLine01 = [=](Vector2i mousePos) -> bool {
			float t = (mousePos.x - GetGlobalPointPos(0).x) *
				(GetGlobalPointPos(1).y - GetGlobalPointPos(0).y) -
				(mousePos.y - GetGlobalPointPos(0).y) *
				(GetGlobalPointPos(1).x - GetGlobalPointPos(0).x);
			return t < 0 ? true : false;
		};
		auto containLine23 = [=](Vector2i mousePos) -> bool {
			float t = (mousePos.x - GetGlobalPointPos(3).x) *
				(GetGlobalPointPos(2).y - GetGlobalPointPos(3).y) -
				(mousePos.y - GetGlobalPointPos(3).y) *
				(GetGlobalPointPos(2).x - GetGlobalPointPos(3).x);
			return t > 0 ? true : false;
		};

		auto containLine12 = [=](Vector2i mousePos) -> bool {
			float t = (mousePos.x - GetGlobalPointPos(2).x) *
				(GetGlobalPointPos(1).y - GetGlobalPointPos(2).y) -
				(mousePos.y - GetGlobalPointPos(2).y) *
				(GetGlobalPointPos(1).x - GetGlobalPointPos(2).x);
			return t > 0 ? true : false;
		};
		auto containLine30 = [=](Vector2i mousePos) -> bool {
			float t = (mousePos.x - GetGlobalPointPos(3).x) *
				(GetGlobalPointPos(0).y - GetGlobalPointPos(3).y) -
				(mousePos.y - GetGlobalPointPos(2).y) *
				(GetGlobalPointPos(0).x - GetGlobalPointPos(3).x);
			return t < 0 ? true : false;
		};

		if (containLine01(Mouse::getPosition(*window)) && containLine23(Mouse::getPosition(*window))
			&& containLine12(Mouse::getPosition(*window)) && containLine30(Mouse::getPosition(*window)))
		{
			tileShape.setFillColor(hoverColor);
		}
		else
		{
			tileShape.setFillColor(defaultColor);
		}*/
	}
	void Render(RenderTarget* target)
	{
		target->draw(tileShape);
	}
	void setState(TileState tileState)
	{
		this->tileState = tileState;
	}
	TileState getState()
	{
		return tileState;
	}

private:
		sf::ConvexShape tileShape;
		/*const float length = 60;
		const float width = 30;*/

		sf::Color defaultColor = sf::Color(50, 168, 56);
		sf::Color hoverColor = sf::Color(61, 224, 58);
		sf::Color routeColor = sf::Color(245, 241, 42);

		TileState tileState = TileState::EMPTY;

};


class Board
{
	Logger* logger;

	int length; 
	int width;  
	int layers; 
	Vector2f origin;

	Tile* boardTiles;
	vector<Component*> components; //Trzeba bêdzie sortowaæ po dodaniu nowego komponentu
	bool* isComponentOnBoard;
	Component* ghostComponent;

	//Vector2f viewOrigin = { 0.f, 0.f };
	Vector2f viewOrigin = { 300.f, 15.f };
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
		isComponentOnBoard = new bool[length * width];

		for (int i = 0; i < length * width; i++)
			isComponentOnBoard[i] = false;


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
				boardTiles[j*length + i].Render(target);
			}
		}

		for (auto component : components)
		{
			component->Render(target);
		}


	}
	void addRoute()
	{

	}
	Vector2i getHoverTile(Vector2i& mouse)
	{
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < length; i++)
			{
				if (boardTiles[j * length + i].isMouseHover(mouse))
					return Vector2i(i, j);
			}
		}
		throw sf::String(L"Poza granicami p³ytki");
	}
	const Vector2i getBoardDimension()
	{
		return Vector2i(length, width);
	}
	 Vector2f& getViewOrigin()
	{
		return viewOrigin;
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
				if (isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] == true)
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
				isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = true;
				
			}
		}

		component->setBoardPosition(pos);
		components.push_back(component);
		logger->Info("Added "+component->getName() + " pos:" + to_string(pos.x) + " " + to_string(pos.y));
	}
};