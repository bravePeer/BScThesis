#pragma once
#include "gui.h"

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
	Component(wstring name, wstring description, Vector2i tileSize, int padsCount, Vector2i* padsPos)
		:name(name), description(description), tileSize(tileSize), padsCount(padsCount)
	{
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

	//Rezystancje spadek napiêcia ect.
	void calculateValues()
	{

	}

private:
	wstring name;
	wstring description;

	Vector2i tileSize;
	int padsCount;
	Vector2i* padsPos; //local coords
	
};


