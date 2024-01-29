#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"
#include "graphics.h"
#include "gui.h"

#define WIDTH 700


class ComponentDesc
{
public:
	ComponentDesc() = delete;
	ComponentDesc(Resources*res)
	{ 
		sf::FloatRect dump = res->getConfig().getSectionConfig(Config::SectionConfig::BoardSection);
		
		shape.setFillColor(sf::Color(120, 200, 120, 200));
		shape.setSize({ dump.getSize().x-20, dump.getSize().y-20 } );
		shape.setPosition({ 10,10 });

		shouldRender = false;
		name.setFont(res->getFont());
		name.setCharacterSize(40);

		desc.setFont(res->getFont());
		desc.setCharacterSize(20);

		type.setFont(res->getFont());
		//desc.setCharacterSize(40);
		mountType.setFont(res->getFont());
		mountType.setCharacterSize(18);
		value.setFont(res->getFont());
		value.setCharacterSize(18);

		infoSchematic.setFont(res->getFont());
		infoSchematic.setCharacterSize(16);

		infoHowToCalc.setFont(res->getFont());
		infoHowToCalc.setCharacterSize(16);

		infoTexture.loadFromFile("Resources\\Images\\ComponentInfoGraphics\\resistorInfoGraphic.png");
		infoSprite.setTexture(infoTexture);
		infoSprite.scale({ 2,2 });
		infoSprite.setPosition({
			shape.getPosition().x+shape.getSize().x - infoTexture.getSize().x*2-10,
			shape.getPosition().y + shape.getSize().y - infoTexture.getSize().y*2-20
			});
		infoHowToCalc.setPosition({ infoSprite.getPosition().x, infoSprite.getPosition().y - 45 });
		infoHowToCalc.setString(L"Ogólny sposób odczytywania\nwartości z tego elementu\nw obudowie SMD:");

		schematicTexture.loadFromFile("Resources\\Images\\Schematics\\resistor.png");
		schematicSprite.setTexture(schematicTexture);
		//schematicSprite.scale({ 2,2 });
		schematicSprite.setPosition({
			shape.getPosition().x + 10,
			shape.getPosition().y + shape.getSize().y - schematicTexture.getSize().y - 20
			});
		infoSchematic.setPosition({ schematicSprite.getPosition().x+10, schematicSprite.getPosition().y - 70 });
		infoSchematic.setString(L"Symbol opornika, \npo lewej symbol europejski\npo prawej symbol amerykański:");

	}

	void setComponent(Component* component)
	{
		sf::String tmp = component->getName();
		tmp.replace(L"\n", L" ");
		name.setString(tmp);
		name.setPosition({ (shape.getSize().x / 2.f) - (name.getString().getSize() / 2.f) * 20 - 100, shape.getPosition().y+10});
		
		mountType.setString(LR"(Charakterystyka tego elementu: 
Typ montowania: powierzchniowe (SMD))"+
sf::String("\nOpór tego elementu: ") + component->getSimValue().substr(0, component->getSimValue().size() - 1) + L"Ω");

		mountType.setPosition({ (shape.getPosition().x+200), shape.getPosition().y + 70 });

		//value.setString();
		//value.setPosition({ (shape.getSize().x / 2.f) - (value.getString().getSize() / 2.f) * 20, shape.getPosition().y + 100 });
		desc.setString(
LR"(Opis:
Rezystor (opornik) jest biernym elementem elektronicznym 
odpowiadającym najczęściej za ograniczanie płynącego prądu. 
Ogranicza natężenie prądu płynącego w gałęzi. Wartość napięcia
między nóżkami oraz natężenie prądu przepływające przez rezystor
jest bespośrednio związana z prawem Ohma. Natomiast nadmiar prądu
zamieniany jest na energię cieplną. 
Występuje w obudowach lutowania powierzchniowego (SMD) oraz
w obudowach do lutowania przewlekanego (THT). Jest elementem
nie spolaryzowanym to znaczy, że nie ma znaczenia w którą
stronę zostanie podłączony.
)");

		desc.setPosition({ (shape.getPosition().x + 120), shape.getPosition().y + 175 });
	}

	void setRender(bool shouldRender)
	{
		this->shouldRender = shouldRender;
	}

	void Render(RenderTarget* target)
	{
		if (!shouldRender)
			return;

		target->draw(shape);
		target->draw(name);
		target->draw(infoSprite);
		target->draw(mountType);
		target->draw(value);
		target->draw(desc);

		target->draw(schematicSprite);
		target->draw(infoSchematic);
		target->draw(infoHowToCalc);
	}

private:
	sf::RectangleShape shape;
	sf::Text name;
	sf::Text value;
	sf::Text desc;
	sf::Text mountType;
	sf::Text type;
	
	sf::Text infoSchematic;
	sf::Text infoHowToCalc;



	sf::Texture infoTexture;
	sf::Sprite infoSprite;

	sf::Texture schematicTexture;
	sf::Sprite  schematicSprite;

	sf::FloatRect infoSize;

	bool shouldRender;
};