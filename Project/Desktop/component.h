#pragma once
#include "gui.h"

//Electric compoenent mo¿e powinien zawieraæ strukture z napiêciem, pr¹dem i mon¹
class Component
{
public:
	enum class ComponentTypePackage {
		SMD, THT
	};
	enum class CompoenetType {
		compoenet, goldpin, resistor, led, capacitor, microcontroller, amperemeter, voltmeter
	};
	Component()
	{
		name = L"";
		description = L"";

		tileSize = { 0,0 };
		padsCount = 0;
		padsPos = nullptr;

	}
	Component(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos,const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "", std::string value= "")
		:name(name), description(description), tileSize(tileSize), padsCount(padsCount),componentTypePackage(type), removable(removable), id(id), simValue(value)
	{
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, getComponentLength(), texture.getSize().y));

		this->globalPosition = { 1,2 };
		this->padsPos = new Vector2i[padsCount];
		for (int i = 0; i < padsCount; i++)
		{
			this->padsPos[i] = padsPos[i];
		}
		simValue = "1";
	}
	Component(Component* component)
		:name(component->name), description(component->description), tileSize(component->tileSize), padsCount(component->padsCount), sprite(component->sprite),
		spriteSize(component->spriteSize), id(component->id), simValue(component->simValue)
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
		try
		{
			//delete[] padsPos;

		}
		catch (const std::exception&)
		{
			cout << "Error: delete padPos! " << endl;
		}
	}

	virtual Component* clone() const
	{
		return new Component(*this);
	}

	//, Vector2f& origin
	virtual void Update(RenderWindow* window, Time* elapsed, Vector2f& viewOrigin)
	{
		globalPosition = ScreenPos({ boardPosition.x, boardPosition.y }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin;// -Vector2f(spriteSize.y - TILE_WIDTH, 0.f);
		
		if (rotation % 2 == 1)
		{
			globalPosition.x -=  getComponentLength() - IMAGE_TILE_LENGTH;
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

		//cout << rotation << endl;

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
	void setSimValue(std::string value)
	{
		simValue = value;
	}
	std::string& getSimValue()
	{
		return simValue;
	}
	Vector2f& getGlobalPos()
	{
		return globalPosition;
	}
protected:
	enum Rotation
	{
		S, W, N, E
	};

	unsigned int getComponentLength()
	{
		int m = max(tileSize.x, tileSize.y);
		if (m == 1)
			return IMAGE_TILE_LENGTH;

		//return (m % 2 == 0) ? (m - 1) * IMAGE_TILE_LENGTH + IMAGE_TILE_LENGTH / 2 : (m - 1) * IMAGE_TILE_LENGTH;
			return ((tileSize.x % 2 == 1) || (tileSize.y % 2 == 1)) ? (m)*IMAGE_TILE_LENGTH - IMAGE_TILE_LENGTH / 2 : (m)*IMAGE_TILE_LENGTH;

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

	wstring name;
	wstring description;
	std::string simSymbol;
	std::string simName;
	std::string simValue;
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

	virtual Goldpin* clone() const override
	{
		return new Goldpin(*this);
	}
private:
};

class Diode:public Component
{

};

class LightEmittingDiode : public Component
{
public:
	LightEmittingDiode(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Dioda LED";
		if (description.length() == 0)
			this->description = L"Pod wp³ywem przep³ywu pr¹du œwieci. Posiada polaryzacjê - pr¹d p³ynie w jedn¹ stronê.";

		componentType = CompoenetType::led;
		simSymbol = "_app\\\\appled";
		simName = "led" + to_string(componentsCount) + id;
	}
	LightEmittingDiode(LightEmittingDiode* ledDiode)
		:Component(ledDiode)
	{
		componentType = CompoenetType::led;
		simSymbol = "_app\\\\appled";
		simName = "led" + to_string(componentsCount) + id;
		componentsCount++;
	}

	virtual LightEmittingDiode* clone() const override
	{
		componentsCount++;
		return new LightEmittingDiode(*this);
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
			this->description = L"Ogranicza p³yn¹cy pr¹d, zamienia czêœæ energii elektrycznje w ciep³o";

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
	Resistor(const Resistor& a)
		:Component(a)
	{
		componentType = CompoenetType::resistor;
		simSymbol = "_app\\\\appres";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	virtual Resistor* clone() const override
	{
		return new Resistor(*this);
	}

	static void resetComponentCounter()
	{
		componentsCount = 0;
	}
private:
	static int componentsCount;
};

class Capacitor : public Component
{
public:
	Capacitor(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Kondensator";
		if (description.length() == 0)
			this->description = L"Kumuluje ³adunek elektryczny";

		componentType = CompoenetType::capacitor;
		simSymbol = "_app\\\\appres"; // TODO
		simName = "res" + to_string(componentsCount) + this->id;
	}
	Capacitor(Capacitor* resistor)
		:Component(resistor)
	{
		componentType = CompoenetType::capacitor;
		simSymbol = "_app\\\\appres";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	virtual Capacitor* clone() const override
	{
		componentsCount++; 
		return new Capacitor(*this);
	}
	
	static void resetComponentCounter()
	{
		componentsCount = 0;
	}
private:
	static int componentsCount;
};

class Microcontroller : public Component
{
public:
	Microcontroller(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Kondensator";
		if (description.length() == 0)
			this->description = L"Kumuluje ³adunek elektryczny";

		componentType = CompoenetType::microcontroller;
		simSymbol = "_app\\\\appres"; // TODO
		simName = "res" + to_string(componentsCount) + this->id;
	}
	Microcontroller(Microcontroller* resistor)
		:Component(resistor)
	{
		componentType = CompoenetType::microcontroller;
		simSymbol = "_app\\\\appres";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	virtual Microcontroller* clone() const override
	{
		componentsCount++; 
		return new Microcontroller(*this);
	}

	static void resetComponentCounter()
	{
		componentsCount = 0;
	}

	//It shouldnt be like that
	void Update(RenderWindow* window, Time* elapsed, Vector2f& viewOrigin)
	{
		//globalPosition = ScreenPos({ boardPosition.x+1, boardPosition.y + 4}, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin;// -Vector2f(spriteSize.y - TILE_WIDTH, 0.f);

		//if (rotation % 2 == 1)
		//{
		//	globalPosition.x -= getComponentLength() - IMAGE_TILE_LENGTH;
		//}

		//int m = max(tileSize.x, tileSize.y);
		//globalPosition.y -= sprite.getTexture()->getSize().y - (TILE_WIDTH + TILE_WIDTH / 2);
		if(tileSize.x % 2 == 1)
			globalPosition = ScreenPos({ boardPosition.x , boardPosition.y + tileSize.y }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin - Vector2f(spriteSize.y - TILE_WIDTH, 0.f);// dla stabilizatora dzia³a
		else
			globalPosition = ScreenPos({ boardPosition.x , boardPosition.y + tileSize.y }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin - Vector2f(spriteSize.y - TILE_WIDTH , -TILE_WIDTH / 2.f);// dla stabilizatora dzia³a

		if (rotation % 2 == 1)
		{
			globalPosition.x -= getComponentLength() - IMAGE_TILE_LENGTH;
		}

		int m = max(tileSize.x, tileSize.y);
		globalPosition.y -= sprite.getTexture()->getSize().y - (TILE_WIDTH + TILE_WIDTH / 2);


		//globalPosition.y = globalPosition.y - (sprite->getTexture()->getSize().y - (m - 1) * TILE_WIDTH); //+(m % 2 == 0) ? TILE_WIDTH / 2 : 0);
	}
	void Render(RenderTarget* target)
	{
		
		sprite.setPosition(globalPosition);
		target->draw(sprite);
	}
private:
	static int componentsCount;
};

class Amperemeter : public Component {
public:
	Amperemeter(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Amperomierz";
		if (description.length() == 0)
			this->description = L"Mierzy p³yn¹cy ³adunek";

		componentType = CompoenetType::amperemeter;
		simSymbol = "_app\\\\appamperemeter"; // TODO
		simName = "res" + to_string(componentsCount) + this->id;
	}
	Amperemeter(Amperemeter* amperemeter)
		:Component(amperemeter)
	{
		componentType = CompoenetType::amperemeter;
		simSymbol = "_app\\\\appamperemeter";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	Amperemeter(const Amperemeter& amperemeter)
		:Component(amperemeter)
	{
		componentType = CompoenetType::amperemeter;
		simSymbol = "_app\\\\appamperemeter";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	virtual Amperemeter* clone() const override
	{
		componentsCount++;
		return new Amperemeter(*this);
	}

	static void resetComponentCounter()
	{
		componentsCount = 0;
	}

	//It shouldnt be like that
	void Update(RenderWindow* window, Time* elapsed, Vector2f& viewOrigin)
	{
		globalPosition = ScreenPos({ boardPosition.x , boardPosition.y + 1 }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin;// -Vector2f(spriteSize.y - TILE_WIDTH, 0.f);

		if (rotation % 2 == 1)
		{
			globalPosition.x -= getComponentLength() - IMAGE_TILE_LENGTH;
		}

		int m = max(tileSize.x, tileSize.y);
		globalPosition.y -= sprite.getTexture()->getSize().y - (TILE_WIDTH + TILE_WIDTH / 2);

		//globalPosition.y = globalPosition.y - (sprite->getTexture()->getSize().y - (m - 1) * TILE_WIDTH); //+(m % 2 == 0) ? TILE_WIDTH / 2 : 0);
	}
	void Render(RenderTarget* target)
	{

		sprite.setPosition(globalPosition);
		target->draw(sprite);
	}
private:
	static int componentsCount;
};

class Voltmeter : public Component{
public:
	Voltmeter(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos, const Texture& texture, ComponentTypePackage type, bool removable = true, std::string id = "")
		:Component(name, description, tileSize, padsCount, padsPos, texture, type, removable, id)
	{
		if (name.length() == 0)
			this->name = L"Amperomierz";
		if (description.length() == 0)
			this->description = L"Mierzy p³yn¹cy ³adunek";

		componentType = CompoenetType::voltmeter;
		simSymbol = "_app\\\\appvoltmeter"; // TODO
		simName = "res" + to_string(componentsCount) + this->id;
	}
	Voltmeter(Voltmeter* voltmeter)
		:Component(voltmeter)
	{
		componentType = CompoenetType::voltmeter;
		simSymbol = "_app\\\\appvoltmeter";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	Voltmeter(const Voltmeter& voltmeter)
		:Component(voltmeter)
	{
		componentType = CompoenetType::voltmeter;
		simSymbol = "_app\\\\appvoltmeter";
		simName = "res" + to_string(componentsCount) + this->id;
		componentsCount++;
	}
	virtual Voltmeter* clone() const override
	{
		componentsCount++; 
		return new Voltmeter(*this);
	}

	static void resetComponentCounter()
	{
		componentsCount = 0;
	}

	//It shouldnt be like that
	void Update(RenderWindow* window, Time* elapsed, Vector2f& viewOrigin) override
	{
		globalPosition = ScreenPos({ boardPosition.x , boardPosition.y + 1 }, { TILE_LENGTH, TILE_WIDTH }) + viewOrigin;// -Vector2f(spriteSize.y - TILE_WIDTH, 0.f);

		if (rotation % 2 == 1)
		{
			globalPosition.x -= getComponentLength() - IMAGE_TILE_LENGTH;
		}

		int m = max(tileSize.x, tileSize.y);
		globalPosition.y -= sprite.getTexture()->getSize().y - (TILE_WIDTH + TILE_WIDTH / 2);

		//globalPosition.y = globalPosition.y - (sprite->getTexture()->getSize().y - (m - 1) * TILE_WIDTH); //+(m % 2 == 0) ? TILE_WIDTH / 2 : 0);
	}
	void Render(RenderTarget* target)
	{

		sprite.setPosition(globalPosition);
		target->draw(sprite);
	}
private:
	static int componentsCount;
};