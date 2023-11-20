#include "levelsToLoad.h"

Level* loadLevel0()
{
	Level* level = new Level(L"P0", L"Pocz�tki", L"Pocz�tkowy poziom, \nkt�ry s�u�y do zapoznania si� z aplikacj�.", false);
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD);
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD);

		delete[] tmp;
		
		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

		//if (componentsCount[Component::CompoenetType::led] != 1)
			//return false;

		return true;
		});

	//level->setCheckSimulation();

	return level;
}