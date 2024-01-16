#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"

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

		/*tileShape.setTexture(GraphicAll::GetInstance().getTileTexture());
		tileShape.setTextureRect(IntRect(0,0,64,48));*/
		tileSprite = GraphicAll::GetInstance().getTileSprite(0);
		tileRoute = GraphicAll::GetInstance().getRouteGraphic();

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

		//tileSprite = GraphicAll::GetInstance().getTileSprite(tileState);
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
		tileSprite = GraphicAll::GetInstance().getTileSprite(tileState);
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