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

		tileShape.setFillColor(sf::Color(255, 0, 0));
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
		return Vector2f(tileShape.getPosition().x+ tileShape.getPoint(point).x, tileShape.getPosition().y + tileShape.getPoint(point).y);
	}

	bool isMouseHover(Vector2i& mousePos)
	{
		auto containLine01 = [=](Vector2i mousePos) -> bool {
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

		auto containLine01 = [=](Vector2i mousePos) -> bool {
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
		}
	}
	void Render(RenderTarget* target)
	{
		sf::RectangleShape rec(Vector2f(100, 100));
		target->draw(rec);

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
	int length; 
	int width;  
	int layers; 

	Tile* boardTiles;

public:
	Board()
	{
		length = 0;
		width = 0;
		layers = 0;
		boardTiles = nullptr;
	}
	Board(int length, int width, int layers)
		:length(length), width(width), layers(layers)
	{
		boardTiles = new Tile[length * width];

		Vector2f startPos( 10 + TILE_LENGTH*(length-1), TILE_WIDTH+10  );
		Vector2f tmpPos = startPos;
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < length; i++)
			{
				boardTiles[j * length + i].SetPos(ScreenPos({j,i},{TILE_LENGTH, TILE_WIDTH}));
				boardTiles[j * length + i].Move({ 300.f, 15.f });
				tmpPos.x += TILE_LENGTH;
			}
			tmpPos.y += TILE_WIDTH / 2.f;
			tmpPos.x = startPos.x - TILE_LENGTH * (j) - TILE_LENGTH/2.f;
		}
	}
	~Board()
	{
		delete[] boardTiles;
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
	}
	void addRoute()
	{

	}
};