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
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
	Level* level = new Level(L"Test0", L"Test", L"Poziom testuj�cy", false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 4;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
	const wchar_t* desc = LR"END(
Pocz�tkowy poziom, 
kt�ry s�u�y do zapoznania si� z aplikacj�.
)END"; 

	Level* level = new Level(L"P0", L"Pocz�tki", desc, false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
	const wchar_t* desc = LR"END(
Pocz�tkowy poziom, kt�ry s�u�y do zapoznania si� z aplikacj�
oraz z podstawowymi poj�ciami. 

Pr�dem elektrycznym nazywamy uporz�dkowany ruch �adunk�w
elektrycznych, czyli taki, w kt�rym �adunek elektryczny
przemieszcza si� w jednym kierunku. Przyj�te zosta�o, 
�e pr�d p�ynie od + do -. Ilo�� przep�ywaj�cego pr�du
to nat�enie oznaczane literk� 'i' lub 'I', jednost� 
jest Amper (A).

Napi�cie elektryczne to r�nica potencja��w, analogi�
jest r�nica wysoko�ci. Napi�cie jest oznaczane literk�
'u', jednost� jest Volt (V).

)END";
	Level* level = new Level(L"START", L"Pocz�tki", desc, true);
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
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznej w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("100R");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("QTLP690C");

		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();

		/*for (map<std::string, int>::iterator it  = componentsCount.begin(); it != componentsCount.end(); it++)
		{
			cout << it->first << ": " << it->second << endl;
		}*/

		if (componentsCount.find("id0")->second != 1)
			return false;

		if (componentsCount.find("id1")->second != 1)
			return false;

		return true;
		});

	level->setCheckSimulation([](Board* board)->bool {
		

		return true;
		});

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
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
		components[0] = new Resistor(L"Opornik", L"Zamienia cz�� energii elektrycznje w ciep�o", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
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
