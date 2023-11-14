#pragma once
#include "gui.h"

//Electric compoenent mo¿e powinien zawieraæ strukture z napiêciem, pr¹dem i mon¹
class Component
{
public:
	enum class ComponentType {
		SMD, THT
	};
	Component()
	{
		name = L"";
		description = L"";

		tileSize = { 0,0 };
		padsCount = 0;
		padsPos = nullptr;
	}
	Component(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos,const Texture& texture, ComponentType type, bool removable = true)
		:name(name), description(description), tileSize(tileSize), padsCount(padsCount),componentType(type), removable(removable)
	{

		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, getComponentLength(), texture.getSize().y));

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
		
		if (rotation % 2 == 1)
		{
			globalPosition.x -=  getComponentLength()-IMAGE_TILE_LENGTH;
		}

		int m = max(tileSize.x, tileSize.y);
		globalPosition.y -= sprite.getTexture()->getSize().y - (TILE_WIDTH + TILE_WIDTH / 2);
		//globalPosition.y = globalPosition.y - (sprite->getTexture()->getSize().y - (m - 1) * TILE_WIDTH); //+(m % 2 == 0) ? TILE_WIDTH / 2 : 0);
	}
	virtual void Render(RenderTarget* target)
	{
		sprite.setPosition(globalPosition);
		target->draw(sprite);
	}
	virtual void incrementId()
	{ }

	//Rezystancje spadek napiêcia ect.
	virtual void calculateValues()
	{

	}

	void rotate()
	{
		IntRect rect = sprite.getTextureRect();
		rect.left = (rect.left + getComponentLength() <= sprite.getTexture()->getSize().x) ? rect.left + getComponentLength() : 0;
		sprite.setTextureRect(rect);

		rotation++;
		if (rotation > Rotation::E)
			rotation = 0;

		cout << rotation << endl;

		//NOT SURE nwm czy zadzia³a w przypadku niesymetrycznych elementów
		swap(tileSize.x, tileSize.y);

		for (int i = 0; i < padsCount; i++)
		{
			swap(padsPos[i].x, padsPos[i].y);
		}
	}
	int getRotation()
	{
		return rotation;
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

	bool isRemovable()
	{
		return removable;
	}
	ComponentType getComponentType() 
	{
		return componentType;
	}
	std::string& getSimName()
	{
		return componentSimName;
	}
	std::string& getId()
	{
		return componentId;
	}
private:
	enum Rotation
	{
		S, W, N, E
	};

	unsigned int getComponentLength()
	{
		int m = max(tileSize.x, tileSize.y);
		if (m == 1)
			return IMAGE_TILE_LENGTH;
		return (m % 2 == 0) ? (m - 1) * IMAGE_TILE_LENGTH + IMAGE_TILE_LENGTH / 2 : (m - 1) * IMAGE_TILE_LENGTH;
	}

	wstring name;
	wstring description;

	Vector2i tileSize; //dimension in tiles
	int padsCount;
	Vector2i* padsPos; //local coords

	Vector2i boardPosition;
	Vector2f globalPosition; //position on screen

	Sprite sprite;
	Vector2i spriteSize; //Future use to set bigger image
	int rotation = Rotation::S;

	bool removable;
	ComponentType componentType;

	protected:
	std::string componentSimName;
	std::string componentId;
};

class Goldpin : public Component
{
public:
	Goldpin(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentType type, bool removable = true)
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable)
	{
		componentSimName = "goldpin";
	}
private:
};

//class LedDiode : public Component
//{
//public:
//	LedDiode(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite, Vector2u spriteSize)
//		:Component(name, description, tileSize, padsCount, padsPos, sprite, spriteSize)
//	{
//
//	}
//private:
//};

//Change to static in class
static int componentsCount = 0;
class Resistor : public Component
{
public:
	Resistor(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos,const Texture& texture, ComponentType type, bool removable = true)
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable)
	{
		componentSimName = "_app\\\\appres";
		componentId = "res" + to_string(componentsCount);
	}
	Resistor(Resistor* resistor)
		:Component(resistor)
	{
		componentSimName = "_app\\\\appres";
		componentId = "res" + to_string(componentsCount);
		componentsCount++;
	}
	
private:
};

//class Capacitor : public Component
//{
//public:
//	Capacitor(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, Sprite* sprite, Vector2u spriteSize)
//		:Component(name, description, tileSize, padsCount, padsPos, sprite, spriteSize)
//	{
//
//	}
//private:
//};