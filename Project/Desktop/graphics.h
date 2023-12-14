#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Logger.h"

//Dimension on screen
#define TILE_LENGTH 64
#define TILE_WIDTH 32

#define IMAGE_TILE_LENGTH 66
//#define TILE_WIDTH 32

//#define RESISTOR_GRAPHIC_PATH "Resources\\Images\\simpleresistor.png"
#define RESISTOR_GRAPHIC_PATH "Resources\\Images\\simpleresistorMirror.png"
#define CAPACITOR_GRAPHIC_PATH "Resources\\Images\\simplecapacitor.png"
#define DIODE_GRAPHIC_PATH "Resources\\Images\\simplediode.png"
#define ROUTE_GRAPHIC_PATH "Resources\\Images\\routes.png"
#define GOLDPIN_GRAPHIC_PATH "Resources\\Images\\goldpins.png"

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
		
		loadResistorGraphic();
		loadCapacitorGraphic();
		loadDiodeGraphic();
		loadRouteGraphic();
		loadGoldpinTexture();
	}

	const Texture* getTileTexture()
	{
		return &tilesTexture;
	}
	Sprite* getTileSprite(int i)
	{
		return &(tilesSprite[i]);
	}
	
	
	void LoadTestGraphic()
	{
		if (!testTexture.loadFromFile("Resources\\Images\\simpleresistor.png"))
		{
			logger->Error("Cant load testsmdled.png");
			isTestGraphicsLoaded = false;
			return;
		}

		testSprite.setTexture(testTexture);
		logger->Info("Loaded simpleresistor.png, size:" + to_string(testTexture.getSize().x) + " " + to_string(testTexture.getSize().y));
		isTestGraphicsLoaded = true;
	}
	const Texture* getTestTexture()
	{
		return &testTexture;
	}
	sf::Sprite* GetSpriteTest()
	{
		return &testSprite;
	}


	void loadResistorGraphic()
	{

		if (!resistorTexture.loadFromFile(RESISTOR_GRAPHIC_PATH)) {
			logger->Error("Cant load simpleresistor.png");
			//isTestGraphicsLoaded = false;
			return;
		}

		//resistorSprite.setTexture(resistorTexture);
		logger->Info("Loaded simpleresistor.png, size:" + to_string(resistorTexture.getSize().x) + " " + to_string(resistorTexture.getSize().y));
		//isTestGraphicsLoaded = true;
	}

	const sf::Texture& getResistorTexture()
	{
		return resistorTexture;
	}
	//sf::Sprite* getResistorSprite()
	//{
	//	return &resistorSprite;
	//}

	void loadCapacitorGraphic()
	{
		if (!capacitorTexture.loadFromFile(CAPACITOR_GRAPHIC_PATH))
		{
			logger->Error("Cant load simpleresistor.png");
			//isTestGraphicsLoaded = false;
			return;
		}

		//capacitorSprite.setTexture(capacitorTexture);
		logger->Info("Loaded simpleresistor.png, size:" + to_string(capacitorTexture.getSize().x) + " " + to_string(capacitorTexture.getSize().y));
		//isTestGraphicsLoaded = true;
	}
	const sf::Texture& getCapacitorTexture()
	{
		return capacitorTexture;
	}
	//sf::Sprite* getCapacitorSprite()
	//{
	//	return &capacitorSprite;
	//}


	void loadDiodeGraphic()
	{
		if (!diodeTexture.loadFromFile(DIODE_GRAPHIC_PATH))
		{
			logger->Error("Cant load simplediode.png");
			//isTestGraphicsLoaded = false;
			return;
		}

		//capacitorSprite.setTexture(capacitorTexture);
		logger->Info("Loaded simplediode.png, size:" + to_string(diodeTexture.getSize().x) + " " + to_string(diodeTexture.getSize().y));

	}
	const sf::Texture& getDiodeTexture()
	{
		return diodeTexture;
	}

	void loadRouteGraphic()
	{
		if (!routeTexture.loadFromFile(ROUTE_GRAPHIC_PATH))
		{
			logger->Error("Cant load: " ROUTE_GRAPHIC_PATH);
			return;
		}

		routeSprite.setTexture(routeTexture);
		routeSprite.setTextureRect(sf::IntRect({0,0}, {66,33}));
		logger->Info("Loaded: " ROUTE_GRAPHIC_PATH ", size:" + to_string(routeTexture.getSize().x) + " " + to_string(routeTexture.getSize().y));
	}
	sf::Sprite* getRouteGraphic()
	{
		return &routeSprite;
	}


	void loadGoldpinTexture()
	{
		if (!goldpinTexture.loadFromFile(GOLDPIN_GRAPHIC_PATH))
		{
			logger->Error("Cant load goldpinTexture.png");
			//isTestGraphicsLoaded = false;
			return;
		}

		//capacitorSprite.setTexture(capacitorTexture);
		logger->Info("Loaded goldpinTexture.png, size:" + to_string(goldpinTexture.getSize().x) + " " + to_string(goldpinTexture.getSize().y));

	}
	const sf::Texture& getGoldpinTexture()
	{
		return goldpinTexture;
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

	Texture resistorTexture;
	//Sprite resistorSprite;
	Texture capacitorTexture;
	//Sprite  capacitorSprite;
	Texture diodeTexture;

	Texture testTexture;
	Sprite testSprite;
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
			tilesSprite[i].setTextureRect(sf::IntRect(IMAGE_TILE_LENGTH * i, 0, 66, 48));
		}
	}

	Texture goldpinTexture;

	Texture routeTexture;
	Sprite routeSprite;

	Logger* logger;
};


//Zawiera ustawienia aplikacji np. wymiary poszczególnych sekcji i ich pozycje
class Config
{
public:
	~Config() 
	{
		delete[] sectionRects;
		delete logger;
	}
	Config(Config& other) = delete;
	void operator=(const Config&) = delete;
	static Config* getInstance()
	{
		if (appConfig == nullptr)
			appConfig = new Config;

		return appConfig;
	}

	const Vector2i& getWindowDimension()
	{
		return windowDimension;
	}

	enum class SectionConfig
	{
		BoardSection,
		RouteSection,
		ComponentSection,
		InfoSection,
		TaskSection,
		MenuSection,

		Last
	};

	const FloatRect& getSectionConfig(SectionConfig configId)
	{
		return sectionRects[static_cast<int>(configId)];
	}
private:
	Config() 
	{
		logger = new Logger("Config");
		loadSectionConfig();
	}
	inline void loadSectionConfig()
	{
		sectionRects = new FloatRect[static_cast<int>(SectionConfig::Last)];

		sectionRects[static_cast<int>(SectionConfig::BoardSection)] = FloatRect(0, 0, 1200, 800);
		sectionRects[static_cast<int>(SectionConfig::RouteSection)] = FloatRect(0, 700, 200, 200);
		sectionRects[static_cast<int>(SectionConfig::ComponentSection)] = FloatRect(220, 700, 970, 200);
		sectionRects[static_cast<int>(SectionConfig::InfoSection)] = FloatRect(1200, 700, 400, 200);
		sectionRects[static_cast<int>(SectionConfig::TaskSection)] = FloatRect(1200, 100, 400, 600);
		sectionRects[static_cast<int>(SectionConfig::MenuSection)] = FloatRect(1200, 0, 400, 100);
	}



	static Config* appConfig;
	Logger* logger;

	Vector2i windowDimension = { 1600, 900 };
	FloatRect* sectionRects;
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