#pragma once
#include "gui.h"

#define TILE_LENGTH 64
#define TILE_WIDTH 32

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
	Component(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite, Vector2u spriteSize)
		:name(name), description(description), tileSize(tileSize), padsCount(padsCount), sprite(sprite), spriteSize(spriteSize)
	{
		this->globalPosition = { 0,0 };
		this->padsPos = new Vector2i[padsCount];
		for (int i = 0; i < padsCount; i++)
		{
			this->padsPos[i] = padsPos[i];
		}
	}
	Component(Component* component)
		:name(component->name), description(component->description), tileSize(component->tileSize), padsCount(component->padsCount), sprite(component->sprite),
		spriteSize(component->spriteSize)
	{
		this->globalPosition = { 0,0 };
		this->padsPos = new Vector2i[component->padsCount];
		for (int i = 0; i < component->padsCount; i++)
		{
			this->padsPos[i] = component->padsPos[i];
		}
	}
	~Component()
	{
		delete[] padsPos;
	}

	//, Vector2f& origin
	virtual void Update(RenderWindow* window, Time* elapsed, Vector2f& viewOrigin)
	{
		globalPosition = ScreenPos({ boardPosition.x, boardPosition.y }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin;// -Vector2f(spriteSize.y - TILE_WIDTH, 0.f);
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
	const wstring& getName()
	{
		return name;
	}
	const int getPadsCount()
	{
		return padsCount;
	}
	Vector2i* getPadsPos()
	{
		return padsPos;
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
	Vector2i spriteSize; //Future use to set bigger image
};


class LedDiode : public Component
{
public:
	LedDiode(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite, Vector2u spriteSize)
		:Component(name, description, tileSize, padsCount, padsPos, sprite, spriteSize)
	{

	}
private:
};


class Resistor : public Component
{
public:
	Resistor(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite, Vector2u spriteSize)
		:Component(name, description, tileSize, padsCount, padsPos, sprite, spriteSize)
	{

	}
private:
};

class Capacitor : public Component
{
public:
	Capacitor(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite, Vector2u spriteSize)
		:Component(name, description, tileSize, padsCount, padsPos, sprite, spriteSize)
	{

	}
private:
};