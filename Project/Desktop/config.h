#pragma once
#include <SFML/Graphics.hpp>

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

	const sf::Vector2i& getWindowDimension()
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

	const sf::FloatRect& getSectionConfig(SectionConfig configId)
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
		sectionRects = new sf::FloatRect[static_cast<int>(SectionConfig::Last)];

		sectionRects[static_cast<int>(SectionConfig::BoardSection)] =sf::FloatRect(0, 0, 1100, 700);
		sectionRects[static_cast<int>(SectionConfig::RouteSection)] =sf::FloatRect(0, 700, 200, 200);
		sectionRects[static_cast<int>(SectionConfig::ComponentSection)] =sf::FloatRect(220, 700, 870, 200);
		sectionRects[static_cast<int>(SectionConfig::InfoSection)] =sf::FloatRect(1200, 700, 400, 200);
		sectionRects[static_cast<int>(SectionConfig::TaskSection)] =sf::FloatRect(1100, 60, 500, 840);
		sectionRects[static_cast<int>(SectionConfig::MenuSection)] =sf::FloatRect(1100, 0, 500, 50);
	}

	const sf::Vector2i windowDimension = { 1600, 900 };
	sf::FloatRect* sectionRects;
};