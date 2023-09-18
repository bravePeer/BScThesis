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
	static GraphicAll* GetInstance()
	{
		if (graphicAll == nullptr)
			graphicAll = new GraphicAll();

		return graphicAll;
	}
	~GraphicAll()
	{
		delete logger;
	}
	GraphicAll(GraphicAll& other) = delete;
	void operator=(const GraphicAll&) = delete;

	void LoadGraphic()
	{
		if (!loadTilesGraphics())
			logger->Error("Can NOT load tile graphics!");
		else
			logger->Info("Tiles loaded successfuly!");

	}
	void LoadTestGraphic()
	{
		if (!testTexture.loadFromFile("Resources\\Images\\testsmdled.png"))
		{
			logger->Error("Cant load testsmdled.png");
			isTestGraphicsLoaded = false;
			return;
		}

		testSprite.setTexture(testTexture);
		logger->Info("Loaded testsmdled.png, size:" + to_string(testTexture.getSize().x) + " " + to_string(testTexture.getSize().y));
		isTestGraphicsLoaded = true;
	}
	sf::Sprite* GetSpriteTest()
	{
		return &testSprite;
	}
	const Texture* getTileTexture()
	{
		return &tilesTexture;
	}
	Sprite* getTileSprite(int i)
	{
		return &(tilesSprite[i]);
	}
	const Texture* getTestTexture()
	{
		return &testTexture;
	}

	bool IsGraphicLoaded()
	{
		return isTestGraphicsLoaded;
	}
private:
	GraphicAll()
	{
		logger = new Logger("Graphics");
	}

	static GraphicAll* graphicAll;

	Sprite testSprite;
	Texture testTexture;
	bool isTestGraphicsLoaded;

	//48 x 512
	Texture tilesTexture;
	Sprite tilesSprite[8];
	bool loadTilesGraphics()
	{
		if (!tilesTexture.loadFromFile("Resources\\Images\\tiles.png"))
			return false;

		for (int i = 0; i < 8; i++)
		{
			tilesSprite[i].setTexture(tilesTexture);
			tilesSprite[i].setTextureRect(sf::IntRect(65 * i, 0, 66, 48));
		}
	}


	Logger* logger;
};

GraphicAll* GraphicAll::graphicAll = nullptr;

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

	Sprite testSprite;

	string mapPresetsPath = "Resources\\Presets\\";
};