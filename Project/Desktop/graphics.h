#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include "Logger.h"
#include "graphicPaths.h"

//Dimension on screen
#define TILE_LENGTH 64
#define TILE_WIDTH 32

#define IMAGE_TILE_LENGTH 66
//#define TILE_WIDTH 32



using namespace sf;
using namespace std;

class ComponentGraphics
{
public:
	ComponentGraphics()
		:loaded(false),  path("")
	{ }
	ComponentGraphics(std::string path)
		:loaded(false), path(path)
	{ }

	bool loadComponent()
	{
		return componentTexture.loadFromFile(path);
	}
	const Texture& getTexture()
	{
		return componentTexture;
	}
	const std::string getPath()
	{
		return path;
	}

private:
	sf::Texture componentTexture;
	bool loaded;
	std::string path;
};

/*Grafiki do gry*/
class GraphicManager
{
public:
	static GraphicManager& GetInstance()
	{
		static GraphicManager graphic;
		return graphic;
	}
	~GraphicManager()
	{
		delete logger;
	}
	GraphicManager(GraphicManager& other) = delete;
	void operator=(const GraphicManager&) = delete;

	void loadComponentGraphics()
	{
		for (auto& comp : componentsGraphics)
		{
			if (comp.second.loadComponent())
				logger->Info(std::string("Loaded component graphic: ") + comp.first + std::string(", size:") + to_string(comp.second.getTexture().getSize().x) + " " + to_string(comp.second.getTexture().getSize().y));
			else
				logger->Error(std::string("NOT laoded component graphic: ") + comp.first + std::string(", path: ") + comp.second.getPath());
		}
	}
	void addGraphicToLoad(std::string componentGraphicsId, std::string path)
	{
		componentsGraphics[componentGraphicsId] = ComponentGraphics(path);
	}
	const Texture& getComponentTexture(std::string componentGraphicsId)
	{
		return componentsGraphics[componentGraphicsId].getTexture();
	}

	void unloadGraphics()
	{
		componentsGraphics.clear();
		componentsDescTextures.clear();
	}

	void loadTileGraphic()
	{
		if (!loadTilesGraphics())
			logger->Error("Can NOT load tile graphics!");
		else
			logger->Info("Tiles loaded successfuly!");
		
		loadRouteGraphic();
	}

	sf::Texture& loadComponentDescription(std::string path)
	{
		std::map<std::string, sf::Texture>::iterator it = componentsDescTextures.find(path);
		if (it == componentsDescTextures.end())
		{
			sf::Texture tex;
			if (!tex.loadFromFile(path))
				logger->Error("Can't load component description");
			
			componentsDescTextures[path] = tex;
			it = componentsDescTextures.find(path);
			
		}
		logger->Info("Successfully loaded component description: " + path);

		return it->second;
	}

	sf::Texture& loadHelp()
	{
		help.loadFromFile(HELP_PATH);
		return help;
	}

	void loadLevelSchematic(std::string path)
	{
		if (!levelSchematicTexture.loadFromFile(path))
			logger->Error("Can't load level schematic file!");

		levelSchematicSprite.setTexture(levelSchematicTexture);
		levelSchematicSprite.setPosition({ 10.f,10.f });
	}
	sf::Sprite& getLevelSchematic()
	{
		return levelSchematicSprite;
	}

	const Texture* getTileTexture()
	{
		return &tilesTexture;
	}
	Sprite* getTileSprite(int i)
	{
		return &(tilesSprite[i]);
	}
	
	sf::Sprite* getRouteGraphic()
	{
		return &routeSprite;
	}

	bool IsGraphicLoaded()
	{
		return isTestGraphicsLoaded;
	}
private:
	GraphicManager()
	{
		logger = new applogger::Logger("Graphics");
	}

	std::map<std::string, ComponentGraphics> componentsGraphics;
	std::map<std::string, sf::Texture> componentsDescTextures;

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
		return true;
	}
	void loadRouteGraphic()
	{
		if (!routeTexture.loadFromFile(ROUTE_GRAPHIC_PATH))
		{
			logger->Error("Cant load: " ROUTE_GRAPHIC_PATH);
			return;
		}

		routeSprite.setTexture(routeTexture);
		routeSprite.setTextureRect(sf::IntRect({ 0,0 }, { 66,33 }));
		logger->Info("Loaded: " ROUTE_GRAPHIC_PATH ", size:" + to_string(routeTexture.getSize().x) + " " + to_string(routeTexture.getSize().y));
	}
	
	Texture goldpinTexture;

	Texture routeTexture;
	Sprite routeSprite;

	applogger::Logger* logger;

	//Level
	Texture levelSchematicTexture;
	Sprite levelSchematicSprite;

	//Help
	Texture help;
};


//Zawiera ustawienia aplikacji np. wymiary poszczególnych sekcji i ich pozycje
class Config
{
public:
	Config()
	{
		loadSectionConfig();
	}
	~Config() 
	{
		delete[] sectionRects;
	}
	Config(Config& other) = delete;
	void operator=(const Config&) = delete;
	/*static Config& getInstance()
	{
		static Config instance;
		return instance;
	}*/

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
	/*Config() 
	{
		logger = new Logger("Config");
		loadSectionConfig();
	}*/
	inline void loadSectionConfig()
	{
		sectionRects = new FloatRect[static_cast<int>(SectionConfig::Last)];

		sectionRects[static_cast<int>(SectionConfig::BoardSection)] = FloatRect(0, 0, 1100, 700);
		sectionRects[static_cast<int>(SectionConfig::RouteSection)] = FloatRect(0, 700, 200, 200);
		sectionRects[static_cast<int>(SectionConfig::ComponentSection)] = FloatRect(220, 700, 870, 200);
		sectionRects[static_cast<int>(SectionConfig::InfoSection)] = FloatRect(1200, 700, 400, 200);
		sectionRects[static_cast<int>(SectionConfig::TaskSection)] = FloatRect(1100, 60, 500, 840);
		sectionRects[static_cast<int>(SectionConfig::MenuSection)] = FloatRect(1100, 0, 500, 50);
	}

	const Vector2i windowDimension = { 1600, 900 };
	FloatRect* sectionRects;
};



/*Grafiki do menu i fonty*/
class Resources
{
public:
	Resources()
	{
		logger = new applogger::Logger("ResourceManager");
	}
	~Resources()
	{
		delete logger;
	}
	void LoadResources()
	{
		bool loaded = true;

		//Load fonts
		if (font.loadFromFile("Resources\\comic.ttf"))
			logger->Info("Successful loaded fonts");
		else
			logger->Error("CAN NOT load fonts ");

		if(demonstrationTexture.loadFromFile(DEMONSTRATION_DPAK8_GRAPHIC_PATH))
			logger->Info("Successful loaded demonstration texture");

		demonstrationSprite.setTexture(demonstrationTexture);
		demonstrationSprite.setPosition({ 600,150 });
	}

	Sprite& GetDemonstrationSprite()
	{
		return demonstrationSprite;
	}

	Font* GetFont()
	{
		return &font;
	}

	Font& getFont()
	{
		return font;
	}

	Config& getConfig()
	{
		return config;
	}

private:
	applogger::Logger* logger;
	Config config;
	
	Texture demonstrationTexture;
	Sprite demonstrationSprite;
	Font font;
};