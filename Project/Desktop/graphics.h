#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Logger.h"

using namespace sf;
using namespace std;

/*Grafiki do gry*/
class GraphicAll
{
public:
	GraphicAll()
	{
		logger = new Logger("Graphics");
	}
	~GraphicAll()
	{
		delete logger;
	}

	void LoadGroundGraphic(short limit = 4)
	{
		if (!tileTexture.loadFromFile("Resources\\Textures\\Ground\\grounds.png"))
		{
			isTileGraphicLoaded = false;
			logger->Error("CAN NOT load Ground Textures");
			return;
		}

		for (int i = 0; i < limit; i++)
		{
			tileSprite[i].setTexture(tileTexture);
			tileSprite[i].setTextureRect(IntRect(Vector2i(256 * i, 0), Vector2i(256, 256)));
		}
		isTileGraphicLoaded = true;

		logger->Info("Successful loaded Ground Textures");
	}
	void LoadBuildingGraphic()
	{
		if (!buildingTexture.loadFromFile("Resources\\Textures\\Building\\Buildings.png") || !buildingTexture2.loadFromFile("Resources\\Textures\\Building\\Buildings.png"))
		{
			logger->Error("CAN NOT load Ground Textures");
			isBuildingGraphicLoaded = false;
			return;
		}

		for (int i = 0; i < 5; i++)
		{
			buildingsSprite[i].setTexture(buildingTexture);
			buildingsSprite[i].setTextureRect(IntRect(Vector2i(256 * i, 0), Vector2i(256, 256)));
			buildingsSpriteOnTile[i].setTexture(buildingTexture2);
			buildingsSpriteOnTile[i].setTextureRect(IntRect(Vector2i(256 * i, 0), Vector2i(256, 256)));
		}

		isBuildingGraphicLoaded = true;
		logger->Info("Successful loaded Building Textures");
	}
	void LoadUnitGraphic()
	{
		if (!unitTexture.loadFromFile("Resources\\Textures\\Unit\\units.png"))
		{
			logger->Error("CAN NOT load Unit Textures");
			isUnitGraphicLoaded = false;
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			unitsSprite[i].setTexture(unitTexture);
			unitsSprite[i].setTextureRect(IntRect(Vector2i(256 * i, 0), Vector2i(256, 256)));
		}

		isUnitGraphicLoaded = true;
		logger->Info("Successful loaded Unit Textures");
	}

	void LoadOtherGraphic()
	{
		if (!mouseOnTileTexture.loadFromFile("Resources\\Textures\\Gui\\ramka.png"))
		{
			isOtherGraphicsLoaded = false;
			logger->Error("CAN NOT load Other Textures");
			return;
		}
		mouseOnTileSprite.setTexture(mouseOnTileTexture);
		isOtherGraphicsLoaded = true;
		logger->Info("Successful loaded Other Textures");
	}

	bool IsGraphicLoaded()
	{
		return isBuildingGraphicLoaded && isUnitGraphicLoaded && isTileGraphicLoaded;
	}
	Sprite* GetSpriteGround(unsigned short id)
	{
		return &tileSprite[id];
	}
	Sprite* GetAllSpritesGround()
	{
		return tileSprite;
	}

	Sprite* GetSpriteBuilding(unsigned short id)
	{
		return &buildingsSprite[id];
	}
	Sprite* GetSpriteBuildingOnTile(unsigned short id)
	{
		return &buildingsSpriteOnTile[id];
	}
	Sprite* GetSpriteUnit(unsigned short id = 0)
	{
		return &unitsSprite[id];
	}

	Sprite* GetSpriteMouseOnTile()
	{
		return &mouseOnTileSprite;
	}

private:
	bool isTileGraphicLoaded;
	Texture tileTexture;
	Sprite tileSprite[4];

	bool isBuildingGraphicLoaded;
	Texture buildingTexture;
	Texture buildingTexture2;
	Sprite buildingsSprite[5];
	Sprite buildingsSpriteOnTile[5];

	bool isUnitGraphicLoaded;
	Texture unitTexture;
	Sprite unitsSprite[4];

	bool isGuiGraphicLoaded;
	Texture guiTexture;

	bool isOtherGraphicsLoaded;
	Texture mouseOnTileTexture;
	Sprite mouseOnTileSprite;

	Logger* logger;
};

/*Grafiki do menu i fonty*/
class Resources
{
public:
	Resources()
	{
		logger = new Logger("ResourceManager");
	}
	~Resources()
	{
		delete logger;
	}
	void LoadResources()
	{
		bool loaded = true;

		//Load gui graphics
		/*if (!texboxTexture.loadFromFile("Resources\\Textures\\Gui\\resourcesTextBoxTexture.png"))
			loaded = false;
		if (!buttonTexture.loadFromFile("Resources\\Textures\\Gui\\Frame.png"))
			loaded = false;
		if (!buttonSpecificTexture.loadFromFile("Resources\\Textures\\Gui\\menu_nextRoundTexture.png"))
			loaded = false;
		if (!mainTextBox.loadFromFile("Resources\\Textures\\Gui\\mainTextBox.png"))
			loaded = false;

		if (loaded)
			logger->Info("Successful loaded gui textures");
		else
			logger->Error("CAN NOT load gui textures");*/

		//Load fonts
		if (font.loadFromFile("Resources\\comic.ttf"))
			logger->Info("Successful loaded fonts");
		else
			logger->Error("CAN NOT load fonts ");
	}


	Texture* GetTextBoxTexture()
	{
		return &texboxTexture;
	}

	Texture* GetButtonTexture()
	{
		return &buttonTexture;
	}
	Texture* GetButtonSpecificTexture()
	{
		return &buttonSpecificTexture;
	}
	Texture* GetMainTextBoxTexture()
	{
		return &mainTextBox;
	}

	Font* GetFont()
	{
		return &font;
	}
	Font& getFont()
	{
		return font;
	}


	string GetMapPresetPath()
	{
		return mapPresetsPath;
	}

private:
	Logger* logger;

	Texture texboxTexture;
	Texture buttonTexture;
	Texture buttonSpecificTexture;
	Texture mainTextBox;
	Font font;

	string mapPresetsPath = "Resources\\Presets\\";
};