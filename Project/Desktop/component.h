#pragma once
#include "gui.h"

//Electric compoenent mo�e powinien zawiera� strukture z napi�ciem, pr�dem i mon�
class Component
{
public:
	enum class ComponentTypePackage {
		SMD, THT
	};
	enum class CompoenetType {
		compoenet, goldpin, resistor, led
	};
	Component()
	{
		name = L"";
		description = L"";

		tileSize = { 0,0 };
		padsCount = 0;
		padsPos = nullptr;
	}
	Component(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos,const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:name(name), description(description), tileSize(tileSize), padsCount(padsCount),componentTypePackage(type), removable(removable), id(id)
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
		spriteSize(component->spriteSize), id(component->id)
	{
		this->globalPosition = { 0,0 };
		this->padsPos = new Vector2i[component->padsCount];
		for (int i = 0; i < component->padsCount; i++)
		{
			this->padsPos[i] = component->padsPos[i];
		}
		this->removable = component->removable;
		this->componentType = component->componentType;
		this->componentTypePackage = component->componentTypePackage;
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

	//Rezystancje spadek napi�cia ect.
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

		//cout << rotation << endl;

		//NOT SURE nwm czy zadzia�a w przypadku niesymetrycznych element�w
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

	CompoenetType getComponentType()
	{
		return componentType;
	}

	bool isRemovable()
	{
		return removable;
	}
	ComponentTypePackage getComponentTypePackage() 
	{
		return componentTypePackage;
	}
	std::string& getSimSymbol()
	{
		return simSymbol;
	}
	std::string& getSimName()
	{
		return simName;
	}
	std::string& getId()
	{
		return id;
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

	Vector2i tileSize; //dimension in tiles
	int padsCount;
	Vector2i* padsPos; //local coords

	Vector2i boardPosition;
	Vector2f globalPosition; //position on screen

	Sprite sprite;
	Vector2i spriteSize; //Future use to set bigger image
	int rotation = Rotation::S;

	bool removable;
	ComponentTypePackage componentTypePackage;

protected:
	wstring name;
	wstring description;
	std::string simSymbol;
	std::string simName;
	std::string id;

	CompoenetType componentType;
};

class Goldpin : public Component
{
public:
	Goldpin(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		simSymbol = "goldpin";
		componentType = CompoenetType::goldpin;
	}
private:
};

class LedDiode : public Component
{
public:
	LedDiode(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Dioda LED";
		if (description.length() == 0)
			this->description = L"Pod wp�ywem przep�ywu pr�du �wieci. Posiada polaryzacj� - pr�d p�ynie w jedn� stron�.";

		componentType = CompoenetType::led;
		simSymbol = "_app\\\\appled";
		simName = "led" + to_string(componentsCount) + id;
	}
	LedDiode(LedDiode* ledDiode)
		:Component(ledDiode)
	{
		componentType = CompoenetType::led;
		simSymbol = "_app\\\\appled";
		simName = "led" + to_string(componentsCount) + id;
		componentsCount++;
	}

	static void resetComponentCounter()
	{
		componentsCount = 0;
	}
private:
	static int componentsCount;
};

//Change to static in class

class Resistor : public Component
{
public:
	Resistor(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos,const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Opornik";
		if (description.length() == 0)
			this->description = L"Ogranicza p�yn�cy pr�d, zamienia cz�� energii elektrycznje w ciep�o";

		componentType = CompoenetType::resistor;
		simSymbol = "_app\\\\appres";
		simName = "res" + to_string(componentsCount) + this->id;
	}
	Resistor(Resistor* resistor)
		:Component(resistor)
	{
		componentType = CompoenetType::resistor;
		simSymbol = "_app\\\\appres";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	
	static void resetComponentCounter()
	{
		componentsCount = 0;
	}
private:
	static int componentsCount;
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