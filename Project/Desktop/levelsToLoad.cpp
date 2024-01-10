#include "levelsToLoad.h"

Level* loadLevelExampleOfImplemetation()
{

	Level* level = new Level(L"ID", L"NAME", L"DESC");
	
	level->setPathToSave("save.asc");//This will be usless


	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevel0()
{
	Level* level = new Level(L"P0", L"Pocz�tki", L"Pocz�tkowy poziom, \nkt�ry s�u�y do zapoznania si� z aplikacj�.", false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;
		
		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

		/*if (componentsCount[Component::CompoenetType::led] != 1)
			throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelSTART()
{
	Level* level = new Level(L"START", L"Pocz�tki", L"awd", true);
	level->setPrevLevelsIds({ });

	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelRES0()
{
	Level* level = new Level(L"RES0", L"Oporno��", L"Poznaj op�r!");
	level->setPrevLevelsIds({ "START" });

	level->setPathToSave("save.asc");

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelRES1()
{
	Level* level = new Level(L"RES1", L"Op�r po��czenia r�wnoleg�ego", L"Poznaj op�r w przypadku ��czenia opornik�w r�wnolegle!");
	level->setPrevLevelsIds({ "RES0" });

	level->setPathToSave("save.asc");

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelCAP0()
{
	Level* level = new Level(L"CAP0", L"Pojemno��", L"Poznaj pojemno��!");
	level->setPrevLevelsIds({ "RES0" });

	level->setPathToSave("save.asc");

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelLED0()
{
	Level* level = new Level(L"LED0", L"Dioda �wiec�ca!", L"Poznaj diod� LED!");
	level->setPrevLevelsIds({ "RES0" });

	level->setPathToSave("save.asc");

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance()->getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}
