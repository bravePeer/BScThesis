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

	void LoadTestGraphic()
	{
		if (!testTexture.loadFromFile("Resources\\Images\\testsmdled.png"))
		{
			logger->Error("Cant load testsmdled.png");
			isTestGraphicsLoaded = false;
			return;
		}

		testSprite.setTexture(testTexture);
		logger->Info("Loaded testsmdled.png");
		isTestGraphicsLoaded = true;
	}
	sf::Sprite* GetSpriteTest()
	{
		return &testSprite;
	}
	
	bool IsGraphicLoaded()
	{
		return isTestGraphicsLoaded;
	}
private:
	Sprite testSprite;
	Texture testTexture;
	bool isTestGraphicsLoaded;

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

	Sprite testSprite;

	string mapPresetsPath = "Resources\\Presets\\";
};