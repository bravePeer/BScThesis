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
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

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

Level* loadLevelTest()
{
	Level* level = new Level(L"Test0", L"Test", L"Poziom testuj¹cy", false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 4;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");
		components[2] = new Capacitor(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getCapacitorTexture(), Component::ComponentTypePackage::SMD, true, "id2");
		delete[] tmp;
		tmp = new Vector2i[8];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		tmp[2].x = 2; tmp[2].y = 0;
		tmp[3].x = 3; tmp[3].y = 0;

		tmp[4].x = 0; tmp[4].y = 3;
		tmp[5].x = 1; tmp[5].y = 3;
		tmp[6].x = 2; tmp[6].y = 3;
		tmp[7].x = 3; tmp[7].y = 3;

		components[3] = new Microcontroller(L"uc", L"awd", Vector2i(4, 4), 8, tmp, GraphicAll::GetInstance().getMicrocontrollerTexture(), Component::ComponentTypePackage::SMD, true, "id3");
		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		if (componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

		if (componentsCount[Component::CompoenetType::led] != 1)
			throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);

		return true;
		});

	level->setCheckSimulation([](Board* board)->bool {
		return true;
		});

	return level;
}

Level* loadLevel0()
{
	Level* level = new Level(L"P0", L"Pocz¹tki", L"Pocz¹tkowy poziom, \nktóry s³u¿y do zapoznania siê z aplikacj¹.", false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;
		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

		if (componentsCount[Component::CompoenetType::led] != 1)
			throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);

		return true;
		});

	level->setCheckSimulation([](Board* board)->bool {
		return true;
		});

	return level;
}

Level* loadLevelSTART()
{
	Level* level = new Level(L"START", L"Pocz¹tki", L"awd", true);
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
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

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
	Level* level = new Level(L"RES0", L"Opornoœæ", L"Poznaj opór!");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

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
	Level* level = new Level(L"RES1", L"Opór po³¹czenia równoleg³ego", L"Poznaj opór w przypadku ³¹czenia oporników równolegle!");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

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
	Level* level = new Level(L"CAP0", L"Pojemnoœæ", L"Poznaj pojemnoœæ!");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

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
	Level* level = new Level(L"LED0", L"Dioda œwiec¹ca!", L"Poznaj diodê LED!");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia czêœæ energii elektrycznje w ciep³o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");

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
