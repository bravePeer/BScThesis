#pragma once
#include "gui.h"

#define TILE_LENGTH 63
#define TILE_WIDTH 31

//Electric compoenent
class Component
{
public:
	Component()
	{
		name = L"";
		description = L"";

		tileSize = { 0,0 };
		padsCount = 0;
		padsPos = nullptr;
	}
	Component(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite *sprite)
		:name(name), description(description), tileSize(tileSize), padsCount(padsCount), sprite(sprite)
	{
		globalPosition = { 0,0 };
		this->padsPos = new Vector2i[padsCount];
		for (int i = 0; i < padsCount; i++)
		{
			this->padsPos[i] = padsPos[i];
		}
	}
	~Component()
	{
		delete[] padsPos;
	}

	//, Vector2f& origin
	virtual void Update(RenderWindow* window, Time* elapsed, Vector2f& viewOrigin)
	{
		globalPosition = ScreenPos({ boardPosition.x, boardPosition.y }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin;
	}
	virtual void Render(RenderTarget* target)
	{
		sprite->setPosition(globalPosition);
		target->draw(*sprite);
	}
	
	//Rezystancje spadek napiêcia ect.
	virtual void calculateValues()
	{

	}

	Vector2i& getTileSize()
	{
		return tileSize;
	}
	void setPosition(Vector2f& pos)
	{
		globalPosition = pos;
	}
	Vector2i& getBoardPosition()
	{
		return boardPosition;
	}
	void setBoardPosition(Vector2i& pos)
	{
		boardPosition = pos;
	}
private:
	wstring name;
	wstring description;

	Vector2i tileSize; //dimension in tiles
	int padsCount;
	Vector2i* padsPos; //local coords

	Vector2i boardPosition;
	Vector2f globalPosition; //position on screen

	Sprite* sprite;
};


class LedDiode : public Component
{
public:
	LedDiode(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite)
		:Component(name, description, tileSize, padsCount, padsPos, sprite)
	{

	}
private:
	void Update(RenderWindow* window, Time* elapsed)
	{
	
	}
	void Render(RenderTarget* target)
	{
		
	}
};